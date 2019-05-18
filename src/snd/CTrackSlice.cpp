//
// Created by ddl_blue on 25.8.18.
//

#include <cstring>
#include "../../include/snd/CTrackSlice.hpp"
#include "../../include/msc/CMaintainer.hpp"
#include "../../include/msc/CLogger.hpp"

using namespace NSnd;

// Static members
NMsc::CLocklessQue<CTrackSlice *> CTrackSlice::m_newUnusedSlices;
std::atomic_int CTrackSlice::m_newUnusedSliceCnt;
NMsc::CLocklessQue<CTrackSlice *> CTrackSlice::m_unusedSlices;
SND_DATA_TYPE CTrackSlice::m_trashBuffer[NSnd::TRACK_SLICE_BUFFER_LEN];
uint64_t CTrackSlice::m_uniqueIdCounter;
std::map<uint64_t, CTrackSlice *> CTrackSlice::m_sliceDatabase;
int CTrackSlice::m_workerId = 0;

/*----------------------------------------------------------------------*/
CTrackSlice::~CTrackSlice() {
    // Free buffer
    if (m_buffer != m_trashBuffer)
        delete[] m_buffer;

    // Unregister
    if (m_sliceDatabase.find(m_id) != m_sliceDatabase.end())
        m_sliceDatabase.erase(m_id);
}

/*----------------------------------------------------------------------*/
uint64_t CTrackSlice::GetId() {
    return m_id;
}

/*----------------------------------------------------------------------*/
SND_DATA_TYPE *CTrackSlice::GetBuffer() {
    return m_buffer;
}

/*----------------------------------------------------------------------*/
void CTrackSlice::Delete() {
    m_unusedSlices.Push(this);
}

/*----------------------------------------------------------------------*/
CTrackSlice *CTrackSlice::GetSlice(int32_t id) {
    if (m_sliceDatabase.find(id) != m_sliceDatabase.end())
        return m_sliceDatabase[id];
    else
        return nullptr;

}

/*----------------------------------------------------------------------*/
CTrackSlice *CTrackSlice::GetNewSlice() {
    CTrackSlice *slice = nullptr;

    // Reuse deleted slices.
    if (!m_unusedSlices.Empty()) {
        slice = m_unusedSlices.Pop();
    }

    // New pre-allocated slices.
    if (!slice && !m_newUnusedSlices.Empty()) {
        slice = m_newUnusedSlices.Pop();
        --m_newUnusedSliceCnt;
    }

    if (slice)
        return slice;


    // Not enough pre-allocated slices. This should never happen.
    NMsc::CLogger::Log(NMsc::ELogType::RT_ERROR, "CTrackSlice: No free slices allocated. Is worker working?");
    slice = new CTrackSlice();
    slice->m_id = ++m_uniqueIdCounter;
    m_sliceDatabase[m_uniqueIdCounter] = slice;
    return slice;
}

/*----------------------------------------------------------------------*/
void CTrackSlice::DeleteAllSlices() {

    // Deleted buffers
    m_unusedSlices.Clear(); //todo memory leak
//    while (!m_unusedSlices.Empty())
//        delete m_unusedSlices.Pop();

    // Pre-allocated buffers
    m_newUnusedSlices.Clear(); //todo memory leak?
//    while (!m_newUnusedSlices.Empty())
//        delete m_newUnusedSlices.Pop();
    m_newUnusedSliceCnt = 0;

    // All buffers
    NMsc::CLogger::Log(NMsc::ELogType::NOTE, "CTrackSlice: Deleting all % slices.", m_sliceDatabase.size());

    std::list<CTrackSlice *> allSlices;
    for (auto &&slice : m_sliceDatabase) {
        allSlices.push_back(slice.second);
    }

    // Delete all the slices.
    for (const auto &slice : allSlices) {
        delete slice;
    }

    if (!m_sliceDatabase.empty()) {
        NMsc::CLogger::Log(NMsc::ELogType::ERROR, "CTrackSlice: % slices left after DeleteAllSlices.",
                           m_sliceDatabase.size());

        m_sliceDatabase.clear();
    }

}

/*----------------------------------------------------------------------*/
CTrackSlice::CTrackSlice() {
    m_id = 0;
    m_buffer = new SND_DATA_TYPE[NSnd::TRACK_SLICE_BUFFER_LEN];
}

/*----------------------------------------------------------------------*/
void CTrackSlice::StartAutomaticAllocation() {
    NMsc::CLogger::Log(NMsc::ELogType::NOTE, "CTrackSlice: Starting automatic allocation... (%)", m_workerId);
    if (m_workerId)
        return;

    NMsc::CMaintainer &maintainer = NMsc::CMaintainer::GetInstance();
    m_workerId = maintainer.RegisterTask([&]() {

        // pre-alloc new slices
        while (m_newUnusedSliceCnt < 64) {
            for (int i = 0; i < 32; ++i) {
                CTrackSlice *slice = new CTrackSlice();
                slice->m_id = ++m_uniqueIdCounter;
                // Register
                m_sliceDatabase[m_uniqueIdCounter] = slice;

                m_newUnusedSlices.Push(slice);
                ++m_newUnusedSliceCnt;
            }

            // int sliceCnt = m_newUnusedSliceCnt;
            // NMsc::CLogger::Log(NMsc::ELogType::TMP_DEBUG, "New slice allocated. Count= %", sliceCnt);
        }

    });

    NMsc::CLogger::Log(NMsc::ELogType::NOTE, "CTrackSlice: Automatic allocation started. (%)", m_workerId);
}

/*----------------------------------------------------------------------*/
void CTrackSlice::StopAutomaticAllocation() {
    if (m_workerId > 0)
        NMsc::CMaintainer::GetInstance().UnregisterTask(m_workerId);
}

/*----------------------------------------------------------------------*/
void CTrackSlice::ClearSample() {
    std::memset(m_buffer, NSnd::TRACK_SLICE_LEN, sizeof(SND_DATA_TYPE));
}

/*----------------------------------------------------------------------*/
CTrackSlice *CTrackSlice::Clone() {
    CTrackSlice *rtrn = GetNewSlice();
    memcpy(rtrn->m_buffer, m_buffer, sizeof(SND_DATA_TYPE) * NSnd::TRACK_SLICE_BUFFER_LEN);
    return rtrn;
}

/*----------------------------------------------------------------------*/
void CTrackSlice::SerializeAllUsedSlices(std::ostream &output) {

    union IdNumberUni {
        uint64_t ID;
        char data[sizeof(ID)];
    };

    union SoundDataUni {
        SND_DATA_TYPE sound[NSnd::TRACK_SLICE_BUFFER_LEN];
        char data[sizeof(SND_DATA_TYPE) * NSnd::TRACK_SLICE_BUFFER_LEN];
    };

    // Save ID counter
    IdNumberUni maxId;
    maxId.ID = m_uniqueIdCounter;
    output.write(maxId.data, sizeof(maxId));

    for (const auto &slice : m_sliceDatabase) {
        IdNumberUni id;
        SoundDataUni sound;

        id.ID = slice.first;
        std::memcpy(sound.sound, slice.second->m_buffer, sizeof(SoundDataUni));

        // Write to file
        output.write(id.data, sizeof(id));
        output.write(sound.data, sizeof(sound));

        if (!output.good()) {
            NMsc::CLogger::Log(NMsc::ELogType::ERROR, "CTrackSlice: Serialization failed, output is not \"good\".");
            return;
        }
    }
}

/*----------------------------------------------------------------------*/
void CTrackSlice::DeserializeSlices(std::istream &input) {
    union IdNumberUni {
        uint64_t ID;
        char data[sizeof(ID)];
    };

    union SoundDataUni {
        SND_DATA_TYPE sound[NSnd::TRACK_SLICE_BUFFER_LEN];
        char data[sizeof(SND_DATA_TYPE) * NSnd::TRACK_SLICE_BUFFER_LEN];
    };

    DeleteAllSlices();

    // Get ID counter
    IdNumberUni maxId;
    input.read(maxId.data, sizeof(maxId));
    m_uniqueIdCounter = maxId.ID;

    while (input.good()) {
        IdNumberUni id;
        SoundDataUni sound;

        input.read(id.data, sizeof(id));
        input.read(sound.data, sizeof(sound));

        if (!input.good())
            break;

        CTrackSlice *newSlice = new CTrackSlice();

        newSlice->m_id = id.ID;
        std::memcpy(newSlice->m_buffer, sound.sound, sizeof(SoundDataUni));

//        NMsc::CLogger::Log(NMsc::ELogType::TMP_DEBUG, "CTrackSlice: Deserialized slice %", id.ID);

        m_sliceDatabase.insert(std::make_pair(id.ID, newSlice));
    }
}