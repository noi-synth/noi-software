//
// Created by ddl_blue on 25.8.18.
//

#include "../../include/snd/CTrackSlice.hpp"
#include "../../include/msc/CMaintainer.hpp"

using namespace NSnd;

// Static members
NMsc::CLocklessQue<CTrackSlice *> CTrackSlice::m_newUnusedSlices;
std::atomic_int CTrackSlice::m_newUnusedSliceCnt;
NMsc::CLocklessQue<CTrackSlice *> CTrackSlice::m_unusedSlices;
SND_DATA_TYPE CTrackSlice::m_trashBuffer[NSnd::TRACK_SLICE_BUFFER_LEN];
uint32_t CTrackSlice::m_uniqueIdCounter;
std::map<uint32_t, CTrackSlice *> CTrackSlice::m_sliceDatabase;
int CTrackSlice::m_workerId;

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
uint32_t CTrackSlice::GetId() {
    return m_id;
}

/*----------------------------------------------------------------------*/
SND_DATA_TYPE *CTrackSlice::GetBuffer() {
    return m_buffer;
}

/*----------------------------------------------------------------------*/
void CTrackSlice::Delete() {
    if (m_sliceDatabase.find(m_id) != m_sliceDatabase.end()) {
        m_unusedSlices.Push(this);
        m_sliceDatabase.erase(m_id);
        m_id = 0;
    }
}

/*----------------------------------------------------------------------*/
CTrackSlice *CTrackSlice::GetSlice(int32_t id) {
    return m_sliceDatabase[id];
}

/*----------------------------------------------------------------------*/
CTrackSlice *CTrackSlice::GetNewSlice() {
    CTrackSlice *slice = nullptr;

    // Reuse slices
    if (!m_unusedSlices.Empty()) {
        slice = m_unusedSlices.Pop();
    }

    // Pre-allocated slices
    if (!slice && !m_newUnusedSlices.Empty()) {
        slice = m_newUnusedSlices.Pop();
        --m_newUnusedSliceCnt;
    }

    if (slice) {
        slice->m_id = ++m_uniqueIdCounter;
        // Register
        m_sliceDatabase[m_uniqueIdCounter] = slice;
        return slice;
    }

    // Not enough pre-allocated slices. This should never happen.
    // TODO RT error log, (is worker working?)
    slice = new CTrackSlice();
    slice->m_id = ++m_uniqueIdCounter;
    m_sliceDatabase[m_uniqueIdCounter] = slice;
    return slice;
}

/*----------------------------------------------------------------------*/
void CTrackSlice::DeleteAllSlices() {

    // Reserved unused buffers
    while (!m_unusedSlices.Empty())
        delete m_unusedSlices.Pop();

    // Pre-allocated buffers
    while (!m_newUnusedSlices.Empty())
        delete m_unusedSlices.Pop();
    m_newUnusedSliceCnt = 0;

    // Used buffers
    for (const auto &slice : m_sliceDatabase) {
        delete slice.second;
    }
    m_sliceDatabase.clear();
}

/*----------------------------------------------------------------------*/
CTrackSlice::CTrackSlice() {
    m_id = 0;
    m_buffer = new SND_DATA_TYPE[NSnd::TRACK_SLICE_LEN];
}

/*----------------------------------------------------------------------*/
void CTrackSlice::StartAutomaticAllocation() {
    if (m_workerId <= 0)
        return;

    NMsc::CMaintainer &maintainer = NMsc::CMaintainer::GetInstance();
    m_workerId = maintainer.RegisterTask([&]() {

        // pre-alloc new slices
        while (m_newUnusedSliceCnt < 15) {
            m_newUnusedSlices.Push(new CTrackSlice());
            ++m_newUnusedSliceCnt;
        }

    });
}

/*----------------------------------------------------------------------*/
void CTrackSlice::StopAutomaticAllocation() {
    if (m_workerId > 0)
        NMsc::CMaintainer::GetInstance().UnregisterTask(m_workerId);
}