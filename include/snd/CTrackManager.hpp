//
// Created by ddl_blue on 26.8.18.
//

#ifndef NOI_SOFTWARE_CTRACKMANAGER_HPP
#define NOI_SOFTWARE_CTRACKMANAGER_HPP

#include <functional>
#include <memory>
#include <set>
#include "CTrackSlice.hpp"
#include "CTrack.hpp"
#include "../msc/CTeamLock.hpp"

namespace NSnd {
    class CTrackManager {
    public:
        CTrackManager();

        ~CTrackManager();

        void ProcessBuffer(SND_DATA_TYPE *input, SND_DATA_TYPE *buffer, int len);

        bool SelectTrack(const ATrack &track); // (Also adds the track to manager) no, it does not

        ATrack CreateTrack(); // null if failed (locked)
        bool RemoveTrack(const ATrack &track);

        bool RemoveAllTracks();

        bool UndoRecording();

        bool StartRecording();

        bool StopRecording();

        bool StartPlayback();

        bool StopPlayback();

        bool SetPlaybackPosition();

        bool GetPlaybackPosition();

// todo mixer, chain volume vs active track volume
// todo claiming lock just for iteration m_tracks may cause glitches, when changing position
    private:
        uint32_t m_playbackPosition;
        ATrack m_selectedTrack;
        // Red team = RW access (UI thread), Blue team = RO access (UI and RT thread)
        NMsc::CTeamLock m_trackLock;
        std::set<ATrack> m_tracks;
        std::atomic_bool m_isRecording; // todo is this needed?
        std::atomic_bool m_isPlaying;

        int m_workerId;
    };

    typedef std::shared_ptr<CTrackManager> ATrackManager;
}

#endif //NOI_SOFTWARE_CTRACKMANAGER_HPP
