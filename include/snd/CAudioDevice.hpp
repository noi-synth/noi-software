//
// Created by ddl_blue on 1.4.18.
//

#ifndef NOI_SOFTWARE_CAUDIODEVICE_HPP
#define NOI_SOFTWARE_CAUDIODEVICE_HPP

#include "CAudioDeviceConfig.hpp"
#include "NSndConfig.hpp"
#include "../../include/lib/portaudio.h"

namespace NSnd {

    // TEMPORARY SOLUTION !!!!!!
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
        CAudioDevice(int(*callback)(const SND_DATA_TYPE *, SND_DATA_TYPE *,
                                    unsigned long),
                     CAudioDeviceConfig config
        );

        bool StartStream();

        bool StopStream();

        static void StreamFinishedCallback(void *userData);

        bool Open();

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


        int (*m_callback)(const SND_DATA_TYPE *, SND_DATA_TYPE *, unsigned long);

        PaStream *m_stream;
        ScopedPaHandler m_paInit;
    };
}


#endif //NOI_SOFTWARE_CAUDIODEVICE_HPP
