//
// Created by ddl_blue on 26.8.18.
//

#include <cmath>
#include "../../include/snd/CTrack.hpp"
#include "../../include/msc/CMaintainer.hpp"
#include "../../include/msc/Common.hpp"
#include "../../include/msc/CLogger.hpp"
#include "../../include/msc/CDebugInfo.hpp"

using namespace NSnd;

static const unsigned int TRACK_REALLOCATION_THRESHOLD = 32;
static const unsigned int TRACK_REALLOCATION_RATIO = 2;


/*----------------------------------------------------------------------*/
CTrack::CTrack() : m_playbackPosition(0), m_farestPlaybackSample(0), m_volume(1), m_isRecording(false),
                   m_recordingEmergencyLen(0),
                   m_undoRecordingStartSamplePosition(0) {

    // Reserve some initial space in track (must be nonzero, otherwise multiplication by TRACK_REALLOCATION_RATIO would do nothing)
    m_trackData.resize(8, nullptr);

    // Start CTrackSlice allocation
    CTrackSlice::StartAutomaticAllocation();

    m_workerId = NMsc::CMaintainer::GetInstance().RegisterTask([&]() {
        // nothing to do, no need to lock anything

        if (m_quedTrackManipulations.Empty() && !m_recordingEmergencyLen &&
            ((m_trackData.size() - m_farestPlaybackSample) > TRACK_REALLOCATION_THRESHOLD))
            return;

        // RT thread holds the lock, try it next time
        if (!m_recordingManipulationLock.TryLockBlue())
            return;

        // Alloc more space
        // todo change this while to if - calculate the final multiplication instead of iterating
        // todo this might also create an endless loop when RPI runs out of RAM, right?
        while ((m_trackData.size() - m_farestPlaybackSample) < TRACK_REALLOCATION_THRESHOLD) {
            m_trackData.resize(m_trackData.size() * TRACK_REALLOCATION_RATIO, nullptr);

#ifdef DEBUG
            NMsc::CDebugInfo::m_sndTrackSize = m_trackData.size();
#endif

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

#ifdef DEBUG
    NMsc::CDebugInfo::m_sndPositionDisplacement -= m_playbackPosition;
#endif

#ifdef DEBUG
    float preBuffer = input[0];
    for (int j = 0; j < 6; ++j) {
        NMsc::CDebugInfo::m_sndLastTrackCall[j] = '-';
    }
#endif



    if (m_recordingManipulationLock.TryLockRed()) {

        // Cursor in buffers from arguments.
        int copyCursor = 0;

        while (len > 0) {

#ifdef DEBUG
            NMsc::CDebugInfo::m_sndLastTrackCall[0] = 'W';
#endif

            // Cursors in track slices.
            int sampleCursor = PositionToSampleNumber(m_playbackPosition);
            int offsetCursor = PositionToSampleOffset(m_playbackPosition);

            // For extension of the track slice container by maintainer.
            m_farestPlaybackSample = MAX(PositionToSampleNumber((uint32_t) m_playbackPosition),
                                         (uint32_t) m_farestPlaybackSample);

            // Maximum samples to be recorded/played in actual slice.
            int limit = MIN(len, TRACK_SLICE_LEN - offsetCursor);

            // Make sure there is a slice for recording
            if (m_isRecording) {
                if (m_trackData.size() < sampleCursor + 1) {
                    NMsc::CLogger::Log(NMsc::ELogType::RT_WARNING,
                                       "CTrack: Recording - Track had to be resized in RT thread! Original size=%, sampleCursor=%",
                                       m_trackData.size(), sampleCursor);
                    m_trackData.resize(MAX(m_trackData.size(), sampleCursor + 32), nullptr);
                }

                // Fill missing track slice
#ifdef DEBUG
                NMsc::CDebugInfo::m_sndLastTrackCall[1] = '0';
#endif
                if (!m_trackData[sampleCursor]) {
#ifdef DEBUG
                    NMsc::CDebugInfo::m_sndLastTrackCall[1] = 'S';
#endif
                    CTrackSlice *newSlice = CTrackSlice::GetNewSlice();
                    m_trackData[sampleCursor] = newSlice;
                    // todo optimise - move this action out of RT thread?
                    newSlice->ClearSample();
                }
            }

#ifdef DEBUG
            if (m_trackData.size() <= sampleCursor)
                NMsc::CDebugInfo::m_sndLastTrackCall[0] = 'S';
            else if (!m_trackData[sampleCursor])
                NMsc::CDebugInfo::m_sndLastTrackCall[0] = 'D';
#endif


            if (m_trackData.size() > sampleCursor && m_trackData[sampleCursor]) {
                // Get a slice buffer
                SND_DATA_TYPE *sliceBuffer = m_trackData[sampleCursor]->GetBuffer();
                if (!sliceBuffer)
                    NMsc::CLogger::Log(NMsc::ELogType::RT_ERROR,
                                       "CTrack: sliceBuffer at position % is null. trackData size= %, offsetCursor=%",
                                       sampleCursor, m_trackData.size(), offsetCursor);

                if (m_isRecording) {
                    // Record

#ifdef DEBUG
                    NMsc::CDebugInfo::m_sndLastTrackCall[2] = 'R';
#endif

                    for (int i = 0; i < limit; ++i) {
                        // L
                        buffer[copyCursor] += sliceBuffer[offsetCursor << 1] * m_volume;
                        sliceBuffer[offsetCursor << 1] += input[copyCursor++];
                        // R
                        buffer[copyCursor] += sliceBuffer[(offsetCursor << 1) + 1] * m_volume;
                        sliceBuffer[(offsetCursor << 1) + 1] += input[copyCursor++];

#ifdef DEBUG
                        NMsc::CDebugInfo::m_sndLastTrackCall[3] = fabs(buffer[copyCursor - 1]) > 0.01 ? '1' : '0';
                        NMsc::CDebugInfo::m_sndLastTrackCall[4] =
                                fabs(sliceBuffer[(offsetCursor << 1) + 1]) > 0.01 ? '1' : '0';
#endif

                        offsetCursor += 1;
                    }
                } else {
                    // Play

#ifdef DEBUG
                    NMsc::CDebugInfo::m_sndLastTrackCall[2] = 'P';
#endif

                    for (int i = 0; i < limit; ++i) {
                        // L
                        buffer[copyCursor++] += sliceBuffer[offsetCursor << 1] * m_volume;
                        // R
                        buffer[copyCursor++] += sliceBuffer[(offsetCursor << 1) + 1] * m_volume;

#ifdef DEBUG
                        NMsc::CDebugInfo::m_sndLastTrackCall[3] = fabs(buffer[copyCursor - 1]) > 0.01 ? '1' : '0';
#endif

                        offsetCursor += 1;
                    }
                }
            } else {
                if (m_isRecording)
                    NMsc::CLogger::Log(NMsc::ELogType::RT_ERROR,
                                       "CTrack: SliceBuffer does not contain a slice at position %. trackData size= %, offsetCursor=%",
                                       sampleCursor, m_trackData.size(), offsetCursor);
            }

            // Progress in a track.
            m_playbackPosition += limit;
            len -= limit;
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

#ifdef DEBUG
    NMsc::CDebugInfo::m_sndLastTrackOutput = preBuffer - buffer[0];
    NMsc::CDebugInfo::m_sndLastTrackManagerOutput = buffer[0];
#endif


    /* ------------------------- */
//
//    if (m_recordingManipulationLock.TryLockRed()) {
//
//        // todo optimise
//        // record
//        if (m_isRecording) {
//            //NMsc::CLogger::Log(NMsc::ELogType::TMP_DEBUG, "CTrack: Recording.... Sample %, offset %", PositionToSampleNumber(m_playbackPosition), PositionToSampleOffset(m_playbackPosition));
//            int copyCursor = 0;
//
//            while (len > 0) {
//                int sampleCursor = PositionToSampleNumber(m_playbackPosition);
//                int offsetCursor = PositionToSampleOffset(m_playbackPosition);
//
//                int limit = MIN(len, TRACK_SLICE_LEN - offsetCursor);
//
//
//                // Make sure there is a slice for recording
//                if (m_trackData.size() < sampleCursor + 2) {
//                    NMsc::CLogger::Log(NMsc::ELogType::RT_WARNING,
//                                       "CTrack: Recording - Track had to be resized in RT thread! Original size=%, sampleCursor=%",
//                                       m_trackData.size(), sampleCursor);
//                    m_trackData.resize(m_trackData.size() + 8, nullptr);
//                }
//
//                // Fill missing track slice
//                if (!m_trackData[sampleCursor]) {
//                    CTrackSlice *newSlice = CTrackSlice::GetNewSlice();
//                    m_trackData[sampleCursor] = newSlice;
//                    // todo optimise - move this action out of RT thread?
//                    newSlice->ClearSample();
//                }
//
//                SND_DATA_TYPE *sliceBuffer = m_trackData[sampleCursor]->GetBuffer();
//
//                if (!sliceBuffer)
//                    NMsc::CLogger::Log(NMsc::ELogType::RT_ERROR,
//                                       "CTrack: Recording: sliceBuffer at position % is null. trackData size= %, offsetCursor=%",
//                                       sampleCursor, m_trackData.size(), offsetCursor);
//
//                for (int i = 0; i < limit; ++i) {
//                    // L
//                    buffer[copyCursor] = sliceBuffer[offsetCursor << 1] * m_volume;
//                    sliceBuffer[offsetCursor << 1] += input[copyCursor++];
//
//                    // R
//                    buffer[copyCursor] = sliceBuffer[(offsetCursor << 1) + 1] * m_volume;
//                    sliceBuffer[(offsetCursor << 1) + 1] += input[copyCursor++];
//
//                    offsetCursor += 1;
//                }
//
//                m_playbackPosition += limit;
//                len -= limit;
//                m_farestPlaybackSample = MAX(PositionToSampleNumber((uint32_t) m_playbackPosition),
//                                             (uint32_t) m_farestPlaybackSample);
//            }
//
//            // play
//        } else {
//            // NMsc::CLogger::Log(NMsc::ELogType::TMP_DEBUG, "CTrack: Playing....");
//            int copyCursor = 0;
//
//            while (len > 0) {
//                int sampleCursor = PositionToSampleNumber(m_playbackPosition);
//                int offsetCursor = PositionToSampleOffset(m_playbackPosition);
//
//                int limit = MIN(len, TRACK_SLICE_LEN - offsetCursor);
//                // Make sure there is a slice for recording
////                if (m_trackData.size()<sampleCursor-2)
////                    m_trackData.push_back(CTrackSlice::GetNewSlice());
//
//                if (m_trackData.size() > sampleCursor && m_trackData[sampleCursor]) {
//
//                    SND_DATA_TYPE *sliceBuffer = m_trackData[sampleCursor]->GetBuffer();
//                    if (!sliceBuffer)
//                        NMsc::CLogger::Log(NMsc::ELogType::RT_ERROR,
//                                           "CTrack: Playing: sliceBuffer at position % is null. trackData size= %, offsetCursor=%",
//                                           sampleCursor, m_trackData.size(), offsetCursor);
//
//                    for (int i = 0; i < limit; ++i) {
//                        // L
//                        buffer[copyCursor++] = sliceBuffer[offsetCursor << 1] * m_volume;
////                    sliceBuffer[offsetCursor<<1] += input[copyCursor++];
//
//                        // R
//                        buffer[copyCursor++] = sliceBuffer[(offsetCursor << 1) + 1] * m_volume;
////                    sliceBuffer[(offsetCursor<<1)+1] += input[copyCursor++];
//
//                        offsetCursor += 1;
//                    }
//                }
//// todo undo
//                m_playbackPosition += limit;
//                len -= limit;
//                m_farestPlaybackSample = MAX(PositionToSampleNumber((uint32_t) m_playbackPosition),
//                                             (uint32_t) m_farestPlaybackSample);
//            }
//        }
//
//        m_recordingManipulationLock.Unlock();
//    } else {
//        if (m_isRecording) {
//            if (!m_recordingEmergencyLen) {
//                // todo emergency record
//                NMsc::CLogger::Log(NMsc::ELogType::RT_ERROR, "CTrack: Emergency recording not implemented yet");
//            } else {
//                NMsc::CLogger::Log(NMsc::ELogType::RT_ERROR,
//                                   "CTrack: Recording slice failed. Emergency buffer is already full.");
//                // todo log fail (or just add more to the buffer?)
//            }
//        }
//    }


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
//        NMsc::CLogger::Log(NMsc::ELogType::TMP_DEBUG, "CTrack: Recording started.");
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













