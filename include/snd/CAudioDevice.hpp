//
// Created by ddl_blue on 1.4.18.
//

#ifndef NOI_SOFTWARE_CAUDIODEVICE_HPP
#define NOI_SOFTWARE_CAUDIODEVICE_HPP

#include <memory>
#include <functional>
#include "CAudioDeviceConfig.hpp"
#include "NSndConfig.hpp"
#include "../../include/lib/portaudio.h"

namespace NSnd {

    // TEMPORARY SOLUTION !!!!!! TODO write this in a proper way
    //TODO research priorities of audio
    class ScopedPaHandler {
    public:
        ScopedPaHandler();

        ~ScopedPaHandler();


        PaError result() const;

    private:
        PaError _result;
    };

    /*############################################################################################################################*/


    class CAudioDevice {
    public:
        CAudioDevice(CAudioDeviceConfig config);

        bool StartStream();

        bool StopStream();

        static void StreamFinishedCallback(void *userData);

        bool Open();

        bool BindCallback(const std::function<int(const SND_DATA_TYPE *, SND_DATA_TYPE *, unsigned long)> &callback);

    private:
        static int InnerCallback(const void *inputBuffer, void *outputBuffer,
                                 unsigned long framesPerBuffer,
                                 const PaStreamCallbackTimeInfo *timeInfo,
                                 PaStreamCallbackFlags statusFlags,
                                 void *userData);


        int InnerCallbackMethod(const void *inputBuffer, void *outputBuffer,
                                unsigned long framesPerBuffer,
                                const PaStreamCallbackTimeInfo *timeInfo,
                                PaStreamCallbackFlags statusFlags);


        std::function<int(const SND_DATA_TYPE *, SND_DATA_TYPE *, unsigned long)> m_callback;

        PaStream *m_stream;
        ScopedPaHandler m_paInit;
        bool m_running;
    };

    typedef std::shared_ptr<CAudioDevice> AAudioDevice;
}


#endif //NOI_SOFTWARE_CAUDIODEVICE_HPP
