//
// Created by ddl_blue on 2.8.18.
//

#ifndef NOI_SOFTWARE_CNOIAPP_HPP
#define NOI_SOFTWARE_CNOIAPP_HPP

#include "CAppState.hpp"

namespace NLgc {
    /**
     * Main gate into the backend. This is the class that user interfaces interact with.
     */
    class CNoiApp {
    public:
        CNoiApp();

        /**
         * Set new audio device for audio I/O
         * @param device New device
         * @return True on success
         */
        bool AudioDeviceSet(NSnd::AAudioDevice device);

        /**
         * Clears the audio device used
         */
        void AudioDeviceUnset();

        /**
         * Start the audio stream
         * @return True on success
         */
        bool AudioStart();

        /**
         * Stops the audio stream
         * @return True on success
         */
        bool AudioStop();

        /**
         * Stops all playback audio production
         */
        void AudioPanic();

        /**
         * Is recording on?
         * @return True if recording
         */
        bool IsRecording();

        /**
         * Starts recording
         * @return True on success
         */
        bool RecordingStart();

        /**
         * Stops recording
         * @return True on success
         */
        bool RecordingStop();

        /**
         * Undo last recording. Revert a track to state before last recording
         * @return True on success
         */
        bool RecordingUndo();

        /**
         * Is there a change on active track that can be undone?
         * @return True if undo available
         */
        bool RecordingGetCanUndo();

        /**
         * Starts track playback
         * @return True on success
         */
        bool PlaybackStart();

        /**
         * Stops track playback
         * @return True on success
         */
        bool PlaybackStop();

        /**
         * Changes a position on tracks
         * @param position New position in samples
         * @return True on success
         */
        bool PlaybackSetPosition(uint32_t position);

        /**
         * Changes a position on tracks
         * @param beat New position in beats
         * @return True on success
         */
        bool PlaybaclSetPositionBeats(uint32_t beat);

        /**
         * Get current position on tracks
         * @return Position in samples
         */
        uint32_t PlaybackGetPosition();

        /**
         * Get current position on tracks
         * @return Positions in beats
         */
        uint32_t PlaybackGetPositionBeats();

        /**
         * Are tracks being played?
         * @return True if playback is on
         */
        bool IsPlaying();

        /**
         * Sets new Beats Per Minute value
         * @param bpm New BPM
         * @return True on success
         */
        bool BpmSet(uint32_t bpm);

        /**
         * Gets currently set Beats Per Minute value
         * @return Current BPM
         */
        uint32_t BpmGet();

        /**
         * Turns metronome on and off
         * @param enable True to turn it on, false to turn it off
         * @return True on success
         */
        bool MetronomeSet(bool enable);

        /**
         * Set current octave shift of all midi messages
         * @param octave Octave
         * @return True on success
         */
        bool SetOctave(uint32_t octave);

        /**
         * Send midi message into the Sound Core
         * @param message Midi message
         * @return True on success
         */
        bool SendMidiMessage(NSnd::CMidiMsg message);

        /**
         * Creates new track
         * @return New track. Nullptr if not successful
         */
        NSnd::ATrack TrackCreate();

        /**
         * Set a track as a track active for recording
         * @param track New active track
         * @return True on success
         */
        bool TrackActiveSet(NSnd::ATrack track);

        /**
         * Get currently active track
         * @return Currently active track
         */
        NSnd::ATrack TrackActiveGet();

        /**
         * Get all tracks in a project
         * @return Collection of all tracks
         */
        std::vector<NSnd::ATrack> TracksGet();

        /**
         * Set a chain as active
         * @param chain New active chain
         * @return True on success
         */
        bool ChainActiveSet(NSnd::AChain chain);

        /**
         * Get an active chain
         * @return Active chain
         */
        NSnd::AChain ChainActiveGet();

        /**
         * Get all chains in the project
         * @return All chains in the project
         */
        std::vector<NSnd::AChain> ChainsGet();


    private:
        /**
         * Holds current state of the project
         */
        CAppState m_state;
        /**
         * Current octave
         */
        uint32_t m_octave;
    };

    typedef std::shared_ptr<CNoiApp> ANoiApp;
}


#endif //NOI_SOFTWARE_CNOIAPP_HPP
