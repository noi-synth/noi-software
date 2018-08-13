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

        bool SendMidiMessage(NSnd::CMidiMsg message);

    private:
        CAppState m_state;
    };

    typedef std::shared_ptr<CNoiApp> ANoiApp;
}


#endif //NOI_SOFTWARE_CNOIAPP_HPP
