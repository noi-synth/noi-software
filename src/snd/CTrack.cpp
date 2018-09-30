//
// Created by ddl_blue on 26.8.18.
//

#include "../../include/snd/CTrack.hpp"
#include "../../include/msc/CMaintainer.hpp"
#include "../../include/msc/Common.hpp"
#include "../../include/msc/CLogger.hpp"

using namespace NSnd;

static const unsigned int TRACK_REALLOCATION_THRESHOLD = 10;
static const unsigned int TRACK_REALLOCATION_AMOUNT = 100;


/*----------------------------------------------------------------------*/
CTrack::CTrack() : m_playbackPosition(0), m_volume(1), m_isRecording(false), m_recordingEmergencyLen(0),
                   m_undoRecordingStartSamplePosition(0) {
    m_workerId = NMsc::CMaintainer::GetInstance().RegisterTask([&]() {
        // nothing to do, no need to lock anything
        if (m_quedTrackManipulations.Empty() && !m_recordingEmergencyLen &&
            (m_trackData.capacity() - m_trackData.size()) >= TRACK_REALLOCATION_THRESHOLD)
            return;

        // RT thread holds the lock, try it next time
        if (!m_recordingManipulationLock.TryLockBlue())
            return;

        // Alloc more space
        if ((m_trackData.capacity() - m_trackData.size()) < TRACK_REALLOCATION_THRESHOLD) {
            m_trackData.reserve(m_trackData.capacity() + TRACK_REALLOCATION_AMOUNT);
        }

        if (m_recordingEmergencyLen) {
            // todo emergency recording
        }

        while (!m_quedTrackManipulations.Empty()) {
            // Pop function and CALL it.
            m_quedTrackManipulations.Pop()();
        }

        m_recordingManipulationLock.Unlock();

    });
}

/*----------------------------------------------------------------------*/
CTrack::~CTrack() {
    NMsc::CMaintainer::GetInstance().UnregisterTask(m_workerId);
}

/*----------------------------------------------------------------------*/
void CTrack::ProcessBuffer(SND_DATA_TYPE *input, SND_DATA_TYPE *buffer, unsigned long int len) {
    if (m_recordingManipulationLock.TryLockRed()) {

        // todo optimise

        // play

        // record
        if (m_isRecording) {
            int copyCursor = 0;

            while (len > 0) {
                int sampleCursor = PositionToSampleNumber(m_playbackPosition);
                int offsetCursor = PositionToSampleOffset(m_playbackPosition);

                int limit = MIN(len, TRACK_SLICE_LEN - offsetCursor);
                // Make sure there is a slice for recording
                if (m_trackData.size() < sampleCursor - 2)
                    m_trackData.push_back(CTrackSlice::GetNewSlice());

                SND_DATA_TYPE *sliceBuffer = m_trackData[sampleCursor]->GetBuffer();
                for (int i = 0; i < limit; ++i) {
                    // L
                    buffer[copyCursor] = sliceBuffer[offsetCursor << 1] * m_volume;
                    sliceBuffer[offsetCursor << 1] += input[copyCursor++];

                    // R
                    buffer[copyCursor] = sliceBuffer[(offsetCursor << 1) + 1] * m_volume;
                    sliceBuffer[(offsetCursor << 1) + 1] += input[copyCursor++];

                    offsetCursor += 1;
                }

                m_playbackPosition += limit;
                len -= limit;
            }

        } else {
            int copyCursor = 0;

            while (len > 0) {
                int sampleCursor = PositionToSampleNumber(m_playbackPosition);
                int offsetCursor = PositionToSampleOffset(m_playbackPosition);

                int limit = MIN(len, TRACK_SLICE_LEN - offsetCursor);
                // Make sure there is a slice for recording
//                if (m_trackData.size()<sampleCursor-2)
//                    m_trackData.push_back(CTrackSlice::GetNewSlice());

                if (m_trackData.size() >= sampleCursor) {
                    SND_DATA_TYPE *sliceBuffer = m_trackData[sampleCursor]->GetBuffer();
                    for (int i = 0; i < limit; ++i) {
                        // L
                        buffer[copyCursor] = sliceBuffer[offsetCursor << 1] * m_volume;
//                    sliceBuffer[offsetCursor<<1] += input[copyCursor++];

                        // R
                        buffer[copyCursor] = sliceBuffer[(offsetCursor << 1) + 1] * m_volume;
//                    sliceBuffer[(offsetCursor<<1)+1] += input[copyCursor++];

                        offsetCursor += 1;
                    }
                }
// todo undo
                m_playbackPosition += limit;
                len -= limit;
            }
        }

        m_recordingManipulationLock.Unlock();
    } else {
        if (m_isRecording) {
            if (!m_recordingEmergencyLen) {
                // todo emergency record
                NMsc::CLogger::Log(NMsc::ELogType::RT_ERROR, "CTrack: Emergency recording not implemented yet");
            } else {
                NMsc::CLogger::Log(NMsc::ELogType::RT_ERROR,
                                   "CTrack: Recording slice failed. Emergency buffer is already full.");
                // todo log fail (or just add more to the buffer?)
            }
        }
    }
}

/*----------------------------------------------------------------------*/
void CTrack::SetPosition(uint32_t position) {
    m_quedTrackManipulations.Push([&, position]() {
        ClearUndoUnsafe();
        m_playbackPosition = position;
    });
}

/*----------------------------------------------------------------------*/
void CTrack::ClearUndo() {
    m_quedTrackManipulations.Push([&]() {
        ClearUndoUnsafe();
    });
}

/*----------------------------------------------------------------------*/
void CTrack::StartRecording() {
    m_quedTrackManipulations.Push([&]() {
        ClearUndoUnsafe();
        m_undoRecordingStartSamplePosition = PositionToSampleNumber(m_playbackPosition);
        m_isRecording = true;
    });
}

/*----------------------------------------------------------------------*/
void CTrack::StopRecording() {
    m_quedTrackManipulations.Push([&]() {
        m_isRecording = false;
    });
}

/*----------------------------------------------------------------------*/
void CTrack::UndoRecording() {
    // todo
}

/*----------------------------------------------------------------------*/
uint32_t CTrack::PositionToSampleNumber(uint32_t position) {
    return position >> TRACK_SLICE_LEN_EXPONENT;
}

/*----------------------------------------------------------------------*/
uint32_t CTrack::PositionToSampleOffset(uint32_t position) {
    return position & (TRACK_SLICE_LEN - 1);
}

/*----------------------------------------------------------------------*/
uint32_t CTrack::SampleNumberOffsetToPosition(uint32_t sampleNumber, uint32_t sampleOffset) {
    return (sampleNumber << TRACK_SLICE_LEN_EXPONENT) + sampleOffset;
}

/*----------------------------------------------------------------------*/
void CTrack::ClearUndoUnsafe() {
    for (const auto &undoSlice : m_undoRecording) {
        undoSlice->Delete();
    }
    m_undoRecording.clear();
    m_undoRecordingStartSamplePosition = 0;
}













