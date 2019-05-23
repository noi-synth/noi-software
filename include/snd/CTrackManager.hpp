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
    /**
     * Manages tracks and their synchronous playback.
     */
    class CTrackManager {
    public:
        CTrackManager();

        ~CTrackManager();

        /**
         * Process audio buffers. Outputs track playback and records input.
         * @param input Buffer with input from chain
         * @param buffer Buffer for playback output
         * @param len Length of buffers in frames
         */
        void ProcessBuffer(SND_DATA_TYPE *input, SND_DATA_TYPE *buffer, unsigned long int len);

        /**
         * Set different track as selected. Only one track is selected at any single time.
         * This track will be target for sound recording.
         * @param track New selected track
         * @return True on success
         */
        bool SelectTrack(const ATrack &track); // (Also adds the track to manager) no, it does not

        /**
         * Create new track.
         * @return New track. Null if failed.
         */
        ATrack CreateTrack(); // null if failed (locked)

        /**
         * Remove track from TrackManager
         * @param track Track to delete
         * @return True on success
         */
        bool RemoveTrack(const ATrack &track);

        /**
         * Delete all tracks from manager.
         * @return True on success
         */
        bool RemoveAllTracks();

        // bool UndoRecording();

        /**
         * Insert new track into the manager
         * @param track Track to be inserted
         * @return True on success
         */
        bool InsertTrack(ATrack track);

        /**
         * Start recording on selected track.
         * @return True on success
         */
        bool StartRecording();

        /**
         * Stop recording.
         * @return True on success
         */
        bool StopRecording();

        /**
         * Is active track recording?
         * @return True on success
         */
        bool IsRecording();

        /**
         * Is playback on?
         * @return True if playing
         */
        bool IsPlaying();

        /**
         * Start playback
         * @return True on success
         */
        bool StartPlayback();

        /**
         * Stop playback
         * @return True on success
         */
        bool StopPlayback();

        /**
         * Set new position on all tracks. Stops recording.
         * @param position New position in samples.
         * @return True on success
         */
        bool SetPlaybackPosition(uint32_t position);

        /**
         * Get current playback position
         * @return Current playback position in samples
         */
        uint32_t GetPlaybackPosition();

// todo mixer, chain volume vs active track volume
// todo claiming lock just for iteration m_tracks may cause glitches, when changing position
    private:
        /// Current playback position
        std::atomic_int m_playbackPosition; // todo should be atomic? (SetPlaybackPosition vs ProcessBuffer)

        /// Currently selected track
        ATrack m_selectedTrack;


        /// Lock that protects access to track table. Red team = RW access (UI thread), Blue team = RO access (UI and RT thread)
        NMsc::CTeamLock m_trackLock;

        /// Table of tracks
        std::set<ATrack> m_tracks;

        /// Is selected track recording?
        std::atomic_bool m_isRecording; // todo is this needed?

        /// Is playback on?
        std::atomic_bool m_isPlaying;

        /// Handler for CMaintainer task
        int m_workerId;
    };

    typedef std::shared_ptr<CTrackManager> ATrackManager;
}

#endif //NOI_SOFTWARE_CTRACKMANAGER_HPP
