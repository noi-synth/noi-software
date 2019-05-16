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
CTrack::CTrack() : m_playbackPosition(0), m_farthestPlaybackSample(0), m_shouldUpdatePosition(false), m_volume(1),
                   m_isRecording(false),
                   m_undoRecordingStartSamplePosition(0), m_undoRecordingEndSamplePosition(0),
                   m_recordingEmergencyLen(0) {

    // Reserve some initial space in track (must be nonzero, otherwise multiplication by TRACK_REALLOCATION_RATIO would do nothing)
    m_trackData.resize(8, nullptr);
    m_undoRecording.resize(8, nullptr);

    // Start CTrackSlice allocation
    CTrackSlice::StartAutomaticAllocation();

    RegisterWorkerTask();

}

/*----------------------------------------------------------------------*/
CTrack::CTrack(NMsc::ASerializationNode node) : m_playbackPosition(0), m_farthestPlaybackSample(0),
                                                m_shouldUpdatePosition(false), m_volume(1),
                                                m_isRecording(false),
                                                m_undoRecordingStartSamplePosition(0),
                                                m_undoRecordingEndSamplePosition(0),
                                                m_recordingEmergencyLen(0) {

    m_undoRecording.resize(8, nullptr);

    std::vector<int64_t> track = node->GetIntArray("slices");
    m_trackData.reserve(track.size());

    // Load all slices
    if (track.size()) {
        for (auto item : track) {
            if (item) {
                CTrackSlice *slice = CTrackSlice::GetSlice(item);
                if (slice)
                    m_trackData.push_back(slice);
                else {
                    NMsc::CLogger::Log(NMsc::ELogType::ERROR, "CTrack: Slice ID % not found!", item);
                    m_trackData.push_back(nullptr);
                }
            }
        }
    } else {
        m_trackData.resize(8, nullptr);
        NMsc::CLogger::Log(NMsc::ELogType::WARNING, "CTrack: Loaded track with 0 slices.");
    }

    m_farthestPlaybackSample = m_trackData.size() + 10;

    // Start CTrackSlice allocation
    CTrackSlice::StartAutomaticAllocation();

    RegisterWorkerTask();
}

/*----------------------------------------------------------------------*/
CTrack::~CTrack() {
    NMsc::CMaintainer::GetInstance().UnregisterTask(m_workerId);
}

/*----------------------------------------------------------------------*/
void CTrack::ProcessBuffer(SND_DATA_TYPE *input, SND_DATA_TYPE *buffer, unsigned long int len, std::uint32_t position) {

    // todo optimise this

#ifdef DEBUG
    NMsc::CDebugInfo::m_sndPositionDisplacement -= m_playbackPosition;
#endif

    // Sync playback
    if (m_shouldUpdatePosition) {
        m_playbackPosition = position;

        // If it's recording (this also means that recording just started), set beginning of the undo.
        if (m_isRecording) {
            m_undoRecordingStartSamplePosition = PositionToSampleNumber(m_playbackPosition);
            m_undoRecordingEndSamplePosition = m_undoRecordingStartSamplePosition + 1;
        }
        m_shouldUpdatePosition = false;
    }

    if (m_recordingManipulationLock.TryLockRed()) {

        // Cursor in buffers from arguments.
        int copyCursor = 0;

        // Serves each track slice separately, until whole buffer is processed.
        while (len > 0) {

            // Cursors in track slices.
            int sampleCursor = PositionToSampleNumber(m_playbackPosition);
            int offsetCursor = PositionToSampleOffset(m_playbackPosition);

            // For extension of the track slice container by maintainer.
            m_farthestPlaybackSample = MAX(PositionToSampleNumber((uint32_t) m_playbackPosition),
                                           (uint32_t) m_farthestPlaybackSample);

            // Maximum samples to be recorded/played in actual slice.
            int limit = MIN(len, TRACK_SLICE_LEN - offsetCursor);

            // Make sure there is a slice for recording + undo.
            if (m_isRecording) {
                if (m_trackData.size() < sampleCursor + 1) {
                    NMsc::CLogger::Log(NMsc::ELogType::RT_WARNING,
                                       "CTrack: Recording - Track had to be resized in RT thread! Original size=%, sampleCursor=%",
                                       m_trackData.size(), sampleCursor);
                    m_trackData.resize(MAX(m_trackData.size(), sampleCursor + 32), nullptr);
                }

                // Undo the slice if this is a first attempt to write to this slice.
                if (!offsetCursor) {


                    // Make sure there is enough space. It should be.
                    if (m_undoRecording.size() <
                        m_undoRecordingEndSamplePosition - m_undoRecordingStartSamplePosition + 1) {
                        NMsc::CLogger::Log(NMsc::ELogType::RT_WARNING,
                                           "CTrack: Recording - Undo buffer had to be resized in RT thread! Original size=%, requested size=%",
                                           m_undoRecording.size(),
                                           m_undoRecordingEndSamplePosition - m_undoRecordingStartSamplePosition);
                        m_undoRecording.resize(MAX(m_undoRecording.size(), m_undoRecordingEndSamplePosition -
                                                                           m_undoRecordingStartSamplePosition + 32),
                                               nullptr);
                    }

                    // Save the copy of the slice.
                    int slicePosition = sampleCursor - m_undoRecordingStartSamplePosition;
                    if (m_undoRecordingEndSamplePosition != sampleCursor) {
                        int tmp = m_undoRecordingEndSamplePosition;
                        NMsc::CLogger::Log(NMsc::ELogType::RT_WARNING,
                                           "CTrack: Recording - m_undoRecordingEndSamplePosition (%) != sampleCursor (%)!",
                                           tmp, sampleCursor);
                    }

                    m_undoRecording[slicePosition] = m_trackData[sampleCursor] ? m_trackData[sampleCursor]->Clone()
                                                                               : nullptr;
                    m_undoRecordingEndSamplePosition = sampleCursor + 1;
//                    int endUndo = m_undoRecordingEndSamplePosition;
//                    NMsc::CLogger::Log(NMsc::ELogType::TMP_DEBUG, "CTrack: offsetCursor = 0, m_undoRecordingEndSamplePosition set to %.", endUndo);
                }

                // Fill missing track slice
                if (!m_trackData[sampleCursor]) {
                    CTrackSlice *newSlice = CTrackSlice::GetNewSlice();
                    m_trackData[sampleCursor] = newSlice;
                    // todo optimise - move this action out of RT thread?
                    newSlice->ClearSample();
                }
            }

            if (m_trackData.size() > sampleCursor && m_trackData[sampleCursor]) {
                // Get a slice buffer
                SND_DATA_TYPE *sliceBuffer = m_trackData[sampleCursor]->GetBuffer();
                if (!sliceBuffer)
                    NMsc::CLogger::Log(NMsc::ELogType::RT_ERROR,
                                       "CTrack: sliceBuffer at position % is null. trackData size= %, offsetCursor=%",
                                       sampleCursor, m_trackData.size(), offsetCursor);

                if (m_isRecording) {
                    // Record

                    for (int i = 0; i < limit; ++i) {
                        // L
                        buffer[copyCursor] +=
                                sliceBuffer[offsetCursor << 1] * m_volume; // todo should not I divide instead?
                        sliceBuffer[offsetCursor << 1] += input[copyCursor++];
                        // R
                        buffer[copyCursor] += sliceBuffer[(offsetCursor << 1) + 1] * m_volume;
                        sliceBuffer[(offsetCursor << 1) + 1] += input[copyCursor++];

                        offsetCursor += 1;
                    }
                } else {
                    // Play
                    for (int i = 0; i < limit; ++i) {
                        // L
                        buffer[copyCursor++] += sliceBuffer[offsetCursor << 1] * m_volume;
                        // R
                        buffer[copyCursor++] += sliceBuffer[(offsetCursor << 1) + 1] * m_volume;

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


}

/*----------------------------------------------------------------------*/
void CTrack::SetPosition() {
    m_quedTrackManipulations.Push([&]() {
//        NMsc::CLogger::Log(NMsc::ELogType::TMP_DEBUG, "CTrack: Set position.");
        if (m_isRecording)
            ClearUndoUnsafe();
        m_shouldUpdatePosition = true;
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
        m_shouldUpdatePosition = true;

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
    m_quedTrackManipulations.Push([&]() {
        // todo do we want to undo while recording?
        if (m_isRecording)
            return;


        int undoLen = m_undoRecordingEndSamplePosition - m_undoRecordingStartSamplePosition;

        int tmp1 = m_undoRecordingStartSamplePosition;
        int tmp2 = m_undoRecordingEndSamplePosition;

//        NMsc::CLogger::Log(NMsc::ELogType::TMP_DEBUG, "CTrack: Undoing recording from % to % of length %.", tmp1, tmp2, undoLen);
        for (int i = 0; i < undoLen; ++i) {
            // Delete recorded slice.
            if (m_trackData[m_undoRecordingStartSamplePosition])
                m_trackData[m_undoRecordingStartSamplePosition]->Delete();
            // Undo recording.
            m_trackData[m_undoRecordingStartSamplePosition] = m_undoRecording[i];
            m_undoRecording[i] = nullptr; // Might be a bit paranoid.
            ++m_undoRecordingStartSamplePosition;
        }

        m_undoRecordingStartSamplePosition = m_undoRecordingEndSamplePosition = 0;
    });
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
    int undoStart = m_undoRecordingStartSamplePosition;
    int undoEnd = m_undoRecordingEndSamplePosition;
    NMsc::CLogger::Log(NMsc::ELogType::TMP_DEBUG, "CTrack: Clearing undo from positions %-%.", undoStart, undoEnd);

    // Delete all the slices.
    int sliceCnt = m_undoRecordingEndSamplePosition - m_undoRecordingStartSamplePosition;
    for (int i = 0; i < sliceCnt; ++i) {
        if (m_undoRecording[i]) {
            m_undoRecording[i]->Delete();
            m_undoRecording[i] = nullptr; // Might be a bit paranoid.
        }
    }
    m_undoRecordingStartSamplePosition = m_undoRecordingEndSamplePosition = 0;

}

/*----------------------------------------------------------------------*/
bool CTrack::CanUndo() {
    return m_undoRecordingStartSamplePosition != m_undoRecordingEndSamplePosition;
}

/*----------------------------------------------------------------------*/
void CTrack::Serialize(NMsc::ASerializationNode &node) const {
    std::vector<int64_t> track;
    track.reserve(m_trackData.size());

    for (const auto &slice : m_trackData) {
        if (slice)
            track.push_back(slice->GetId());
        else
            track.push_back(0);
    }

    node->SerializeIntArray("slices", std::move(track));
}

/*----------------------------------------------------------------------*/
void CTrack::RegisterWorkerTask() {
    m_workerId = NMsc::CMaintainer::GetInstance().RegisterTask([&]() {
        // nothing to do, no need to lock anything
        if (m_quedTrackManipulations.Empty() && !m_recordingEmergencyLen &&
            ((m_trackData.size() - m_farthestPlaybackSample) > TRACK_REALLOCATION_THRESHOLD) &&
            (m_undoRecording.size() - (m_undoRecordingEndSamplePosition - m_undoRecordingStartSamplePosition) >
             TRACK_REALLOCATION_THRESHOLD))
            return;

        // RT thread holds the lock, try it next time
        if (!m_recordingManipulationLock.TryLockBlue())
            return;

        // Alloc more space
        // todo change this while to if - calculate the final multiplication instead of iterating
        // todo this might also create an endless loop when RPI runs out of RAM, right?
        while ((m_trackData.size() - m_farthestPlaybackSample) <= TRACK_REALLOCATION_THRESHOLD)
            m_trackData.resize(m_trackData.size() * TRACK_REALLOCATION_RATIO, nullptr);

        while (m_undoRecording.size() - (m_undoRecordingEndSamplePosition - m_undoRecordingStartSamplePosition) <=
               TRACK_REALLOCATION_THRESHOLD)
            m_undoRecording.resize(m_undoRecording.size() * TRACK_REALLOCATION_RATIO, nullptr);

#ifdef DEBUG
        NMsc::CDebugInfo::m_sndTrackSize = m_trackData.size();
#endif


        if (m_recordingEmergencyLen) {
            // todo emergency recording
        }

        // Serve deferred calls.
        while (!m_quedTrackManipulations.Empty()) {
            // Pop function and CALL it.
            m_quedTrackManipulations.Pop()();
        }

        m_recordingManipulationLock.Unlock();

    });
}







