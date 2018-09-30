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

        bool PlaybackStart();

        bool PlaybackStop();

        bool PlaybackSetPosition(uint32_t position);

        uint32_t PlaybackGetPosition();

        bool IsPlaying();

        bool BpmSet(int bpm);

        bool MetronomeSet(bool enable);

        bool SendMidiMessage(NSnd::CMidiMsg message);

    private:
        CAppState m_state;
    };

    typedef std::shared_ptr<CNoiApp> ANoiApp;
}


#endif //NOI_SOFTWARE_CNOIAPP_HPP
