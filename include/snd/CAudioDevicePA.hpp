//
// Created by ddl_blue on 1.4.18.
//

#ifndef NOI_SOFTWARE_CAUDIODEVICEPA_HPP
#define NOI_SOFTWARE_CAUDIODEVICEPA_HPP

#include <memory>
#include <functional>
#include <vector>
#include "CAudioDeviceInfo.hpp"
#include "NSndConfig.hpp"
#include "../../include/lib/portaudio.h"
#include "CAudioDevice.hpp"

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


    class CAudioDevicePA : public CAudioDevice {
    public:

        CAudioDevicePA(CAudioDeviceInfo config);

        virtual bool StartStream();

        virtual bool StopStream();

        static void StreamFinishedCallback(void *userData);

        virtual bool Open();

        virtual bool
        BindCallback(const std::function<int(const SND_DATA_TYPE *, SND_DATA_TYPE *, unsigned long)> &callback);

        static std::vector<CAudioDeviceInfo> GetAvailableDevices();

        static int GetDefaultDeviceId();

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
        static ScopedPaHandler m_paInit;
        bool m_running;
        CAudioDeviceInfo m_deviceInfo;
    };


}


#endif //NOI_SOFTWARE_CAUDIODEVICEPA_HPP
