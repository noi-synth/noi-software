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
    /**
     * Handler to PortAudio library
     */
    class ScopedPaHandler {
    public:
        ScopedPaHandler();

        ~ScopedPaHandler();


        PaError result() const;

    private:
        PaError _result;
    };

    /*############################################################################################################################*/

    /**
     * Wrapper around PortAudio library. Represents PortAudio audio I/O device.
     */
    class CAudioDevicePA : public CAudioDevice {
    public:

        /**
         * Constructor
         * @param config Settings of the device
         */
        CAudioDevicePA(CAudioDeviceInfo config);

        virtual bool StartStream() override;

        virtual bool StopStream() override;

        /**
         * Method that is called by PortAudio when stream is finished.
         * @param userData User data. Not used.
         */
        static void StreamFinishedCallback(void *userData);

        virtual bool Open() override;

        virtual bool
        BindCallback(
                const std::function<int(const SND_DATA_TYPE *, SND_DATA_TYPE *, unsigned long)> &callback) override;

        /**
         * Get list of all available audio devices.
         * @return Array of device configurations. Used for constructor of this class.
         */
        static std::vector<CAudioDeviceInfo> GetAvailableDevices();

        /**
         * Get ID of default audio device.
         * @return Default device's ID
         */
        static int GetDefaultDeviceId();

    private:
        /**
         * Function that will be called by the PortAudio library when requesting audio data. Calls InnerCallbackMethod.
         * @param inputBuffer pointer to input audio data buffer
         * @param outputBuffer pointer to output audio data buffer
         * @param framesPerBuffer Frames in the buffer. Tells length of buffers
         * @param timeInfo Info about audio timig. Not used.
         * @param statusFlags Status flags, not used.
         * @param userData User data. Used for pointer to the CAudioDevicePA instance.
         * @return Result. Should return 0 for stream to continue.
         */
        static int InnerCallback(const void *inputBuffer, void *outputBuffer,
                                 unsigned long framesPerBuffer,
                                 const PaStreamCallbackTimeInfo *timeInfo,
                                 PaStreamCallbackFlags statusFlags,
                                 void *userData);


        /**
         *
         * @param inputBuffer pointer to input audio data buffer
         * @param outputBuffer pointer to output audio data buffer
         * @param framesPerBuffer Frames in the buffer. Tells length of buffers
         * @param timeInfo Info about audio timig. Not used.
         * @param statusFlags Status flags, not used.
         * @return Result. Should return 0 for stream to continue.
         */
        int InnerCallbackMethod(const void *inputBuffer, void *outputBuffer,
                                unsigned long framesPerBuffer,
                                const PaStreamCallbackTimeInfo *timeInfo,
                                PaStreamCallbackFlags statusFlags);


        /**
         * Function that will be called by PortAudio callback.
         */
        std::function<int(const SND_DATA_TYPE *, SND_DATA_TYPE *, unsigned long)> m_callback;

        /**
         * PortAudio handler to audio stream
         */
        PaStream *m_stream;

        /**
         * Handler to PortAudio. Makes sure it was initialized.
         */
        static ScopedPaHandler m_paInit;

        /**
         * True if stream is running
         */
        bool m_running;

        /**
         * Basic info about the device
         */
        CAudioDeviceInfo m_deviceInfo;
    };


}


#endif //NOI_SOFTWARE_CAUDIODEVICEPA_HPP
