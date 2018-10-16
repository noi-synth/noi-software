//
// Created by ddl_blue on 26.8.18.
//

#ifndef NOI_SOFTWARE_CTRACK_HPP
#define NOI_SOFTWARE_CTRACK_HPP

#include <functional>
#include <memory>
#include <vector>
#include "../msc/CTeamLock.hpp"
#include "../msc/CLocklessQue.hpp"
#include "CTrackSlice.hpp"

namespace NSnd {
    class CTrack {
    public:
        CTrack();

        ~CTrack();

        void ProcessBuffer(SND_DATA_TYPE *input, SND_DATA_TYPE *buffer, unsigned long int len);

        void SetPosition(uint32_t position); // clears undo
        void ClearUndo(); // clears undo :)
        void StartRecording(); // clears undo
        void StopRecording();

        void UndoRecording(); // clears undo

        float m_volume;

        static inline uint32_t PositionToSampleNumber(uint32_t position);

        static inline uint32_t PositionToSampleOffset(uint32_t position);

        static inline uint32_t SampleNumberOffsetToPosition(uint32_t sampleNumber, uint32_t sampleOffset);

    private:

        void ClearUndoUnsafe();

        std::vector<CTrackSlice *> m_trackData;
        std::atomic_int m_playbackPosition;
        std::atomic_int m_farestPlaybackSample;

        bool m_isRecording;

        NMsc::CLocklessQue<std::function<void()>> m_quedTrackManipulations;
        // Blue team = Maintainer thread, Red team = RT thread
        NMsc::CTeamLock m_recordingManipulationLock;

        std::vector<CTrackSlice *> m_undoRecording;
        uint32_t m_undoRecordingStartSamplePosition;

        SND_DATA_TYPE m_recordingEmergencyBuffer[4096];
        uint32_t m_recordingEmergencyLen;

        int m_workerId;

    };

    typedef std::shared_ptr<CTrack> ATrack;
}

#endif //NOI_SOFTWARE_CTRACK_HPP
