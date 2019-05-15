//
// Created by ddl_blue on 2.8.18.
//

#ifndef NOI_SOFTWARE_CNOIAPP_HPP
#define NOI_SOFTWARE_CNOIAPP_HPP

#include "CAppState.hpp"

namespace NLgc {
    class CNoiApp {
    public:
        CNoiApp();

        bool AudioDeviceSet(NSnd::AAudioDevice device);

        void AudioDeviceUnset();

        bool AudioStart();

        bool AudioStop();

        void AudioPanic();

        bool IsRecording();

        bool RecordingStart();

        bool RecordingStop();

        bool RecordingUndo();

        bool RecordingGetCanUndo();

        bool PlaybackStart();

        bool PlaybackStop();

        bool PlaybackSetPosition(uint32_t position);

        bool PlaybaclSetPositionBeats(uint32_t beat);

        uint32_t PlaybackGetPosition();

        uint32_t PlaybackGetPositionBeats();

        bool IsPlaying();

        bool BpmSet(uint32_t bpm);

        uint32_t BpmGet();

        bool MetronomeSet(bool enable);

        bool SetOctave(uint32_t octave);

        bool SendMidiMessage(NSnd::CMidiMsg message);

        NSnd::ATrack TrackCreate();

        bool TrackActiveSet(NSnd::ATrack track);

        NSnd::ATrack TrackActiveGet();

        std::vector<NSnd::ATrack> TracksGet();

        bool ChainActiveSet(NSnd::AChain chain);

        NSnd::AChain ChainActiveGet();

        std::vector<NSnd::AChain> ChainsGet();


    private:
        CAppState m_state;
        uint32_t m_octave;
    };

    typedef std::shared_ptr<CNoiApp> ANoiApp;
}


#endif //NOI_SOFTWARE_CNOIAPP_HPP
