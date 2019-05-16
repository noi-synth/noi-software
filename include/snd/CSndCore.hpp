//
// Created by ddl_blue on 7.4.18.
//

#ifndef NOI_SOFTWARE_CSNDCORE_HPP
#define NOI_SOFTWARE_CSNDCORE_HPP

#include <set>
#include<thread>
#include "../msc/CLocklessQue.hpp"
#include "CMidiMsg.hpp"
#include "CChain.hpp"
#include "CAudioDevice.hpp"
#include "CTrackManager.hpp"

using NMsc::CLocklessQue;


namespace NSnd {
    /**
     * Main gate into the sound core.
     */
    class CSndCore {
    public:

        CSndCore();

        ~CSndCore();

        //TODO working thread
        //TODO Mixing
        //TODO tracks
        //TODO more todos

        /**
         * Send midi message into active chain
         * @param message Midi message
         * @return True on success
         */
        bool ReciveMidiMsg(const CMidiMsg &message);

        /**
         * Select new chain as active
         * @param chain New active chain
         * @return True on success
         */
        bool ChainSelect(AChain &chain);

        //void ChainClearAllActive();

        // AChain ChainGetSelected() = delete; // This is more tricky than it seems to be. Also, should not be needed.

        /**
         * Stop all audio playback, clear audio buffers, try to clean all the mess.
         */
        void Panic();

        /**
         * Stops current audio device and replace it wint new one.
         * @param device New audio device
         * @return True on success
         */
        bool AudioDeviceSet(AAudioDevice &device);

        /**
         * Stops audio device and removes it from the sound core.
         */
        void AudioDeviceUnset();

        /**
         * Start audio stream
         * @return True on success
         */
        bool AudioDeviceStart();

        /**
         * Stop audio stream
         * @return True on success
         */
        bool AudioDeviceStop();

        /**
         * Start recording of current channel into currently selected track
         * @return True on success
         */
        bool TrackRecordingStart();

        /**
         * Stop audio recording
         * @return True on success
         */
        bool TrackRecordingStop();

        /**
         * Is track recording?
         * @return True if recording
         */
        bool IsRecording();

        /**
         * Start playback of tracks
         * @return True on success
         */
        bool TrackPlaybackStart();

        /**
         * Stop playback of tracks
         * @return True on success
         */
        bool TrackPlaybackStop();

        /**
         * Is track playback on?
         * @return True if playing
         */
        bool IsPlaying();


        //bool TrackRecordingUndo();

        /**
         * Get current track playback position
         * @return Playback position in samples
         */
        uint32_t TrackGetPosition();

        /**
         * Create new track
         * @return New track. Null if not successful
         */
        ATrack TrackCreate();

        /**
         * Set different track as active for recording.
         * @param track New active track
         * @return True on success
         */
        bool TrackSetActive(ATrack track);

        /**
         * Set position of playback. Stops recording.
         * @param position New playback position
         * @return True on success
         */
        bool TrackSetPosition(uint32_t position);

        /**
         * Turns metronome on and off
         * @param enabled True to enable, false to disable
         * @return True on success
         */
        bool SetMetronome(bool enabled);

        /**
         * Set new tempo
         * @param bpm Tempo in beats per minute
         */
        void BpmSet(uint32_t bpm);


    private:
        /**
         * Audio generating callback called by audio device
         * @param inputBuffer Buffer with input data
         * @param outputBuffer Buffer for generated audio
         * @param sampleCnt Buffer lengths in frames
         * @return 0 on success
         */
        int AudioDevCallback(const SND_DATA_TYPE *inputBuffer, SND_DATA_TYPE *outputBuffer,
                             unsigned long sampleCnt);

        /*static int AudioDevCallbackFunction(const SND_DATA_TYPE * inputBuffer, SND_DATA_TYPE * outputBuffer,
                                     unsigned long sampleCnt, void * caller);*/


        // std::thread m_thread;
        /// que of MIDI msgs to be sent to selected instrument
        CLocklessQue<CMidiMsg> m_midiMsgQue;

        /// que of instruments, that should replace the selected one. Happens on callback for race condition reasons.
        CLocklessQue<AChain> m_newSelectedChain;

        //std::set<AChain> m_activeChains;

        // bool m_shouldClearActiveCHains; //todo

        /// Selected chain
        AChain m_selectedChain;

        /// Currently selected audio device
        AAudioDevice m_audioDevice;

        /// Song tempo
        int m_bpm;

        /// True if metronome enabled
        bool m_metronomeEnabled;

        /// Temporary buffer for audio callback
        SND_DATA_TYPE m_buffer[INTERNAL_BUFFERS_LEN];

        /// Track manager, that manages all tracks
        ATrackManager m_trackManager;
    };

    typedef std::shared_ptr<CSndCore> ASndCore;
}
#endif //NOI_SOFTWARE_CSNDCORE_HPP
