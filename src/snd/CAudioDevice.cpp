//
// Created by ddl_blue on 1.4.18.
//


#include <cstdio>

#include "../../include/snd/CAudioDevice.hpp"
#include "../../include/msc/CLogger.hpp"

// Just fast implementation of some basic functionality to start with. Mostly just a copy from "paex_sine_c++.cpp" example from PortAudio library.
// TODO implement it like a reasonable human being would do that.

using namespace NSnd;

/*############################################################################################################################*/

ScopedPaHandler::ScopedPaHandler()
        : _result(Pa_Initialize()) {
}

ScopedPaHandler::~ScopedPaHandler() {
    if (_result == paNoError) {
        Pa_Terminate();
    }
}

PaError ScopedPaHandler::result() const { return _result; }

/*############################################################################################################################*/

/*----------------------------------------------------------------------*/
CAudioDevice::CAudioDevice(NSnd::CAudioDeviceConfig config) : m_callback(nullptr), m_running(false) {
    if (m_paInit.result() != paNoError) {
        NMsc::CLogger::Log("paInit failed");
    }
}

/*----------------------------------------------------------------------*/
bool CAudioDevice::StartStream() {
    if (!m_stream)
        return false;

    PaError err = Pa_StartStream(m_stream);

    m_running = err == paNoError;

    return (err == paNoError);
}

/*----------------------------------------------------------------------*/
bool CAudioDevice::StopStream() {
    if (!m_stream)
        return false;

    PaError err = Pa_CloseStream(m_stream);
    m_stream = nullptr;

    m_running = false; //todo make sure it really is off.

    return (err == paNoError);
}

/*----------------------------------------------------------------------*/
int CAudioDevice::InnerCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
                                const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags,
                                void *userData) {
    return ((CAudioDevice *) userData)->InnerCallbackMethod(inputBuffer, outputBuffer, framesPerBuffer, timeInfo,
                                                            statusFlags);
}

/*----------------------------------------------------------------------*/
int CAudioDevice::InnerCallbackMethod(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
                                      const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags) {
    if (m_callback)

        return m_callback((SND_DATA_TYPE *) inputBuffer, (SND_DATA_TYPE *) outputBuffer, framesPerBuffer);
    return -1; // todo does this return value make sense?
}

/*----------------------------------------------------------------------*/
void CAudioDevice::StreamFinishedCallback(void *userData) {

}

/*----------------------------------------------------------------------*/
bool CAudioDevice::Open() {

    PaDeviceIndex index = Pa_GetDefaultOutputDevice();

    char msg[256];

    if (!m_callback) {
        NMsc::CLogger::Log("Callback not set!");

        return false;
    }



    sprintf(msg, "Trying device %d \n", index);
    NMsc::CLogger::Log(msg);


    PaStreamParameters outputParameters;

    outputParameters.device = index;
    if (outputParameters.device == paNoDevice) {
        NMsc::CLogger::Log("PaNoDevice");
        return false;
    }

    const PaDeviceInfo *pInfo = Pa_GetDeviceInfo(index);
    if (pInfo != 0) {
        sprintf(msg, "Output device name: '%s'\r", pInfo->name);
        NMsc::CLogger::Log(msg);
    }

    outputParameters.channelCount = 2;       /* stereo output */
    outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    PaError err = Pa_OpenStream(
            &m_stream,
            NULL, /* no input */
            &outputParameters,
            SAMPLE_RATE,
            paFramesPerBufferUnspecified,
            paClipOff,      /* we won't output out of range samples so don't bother clipping them */
            &CAudioDevice::InnerCallback,
            this            /* Using 'this' for userData so we can cast to Sine* in paCallback method */
    );

    if (err != paNoError) {
        NMsc::CLogger::Log("Failed to open stream to device!");
        /* Failed to open stream to device !!! */
        return false;
    }

    err = Pa_SetStreamFinishedCallback(m_stream, &CAudioDevice::StreamFinishedCallback);

    if (err != paNoError) {
        Pa_CloseStream(m_stream);
        m_stream = 0;
        NMsc::CLogger::Log("Error in opening stream. Finish could not be assigned.");
        return false;
    }

    return true;
}

/*----------------------------------------------------------------------*/
bool
CAudioDevice::BindCallback(const std::function<int(const SND_DATA_TYPE *, SND_DATA_TYPE *, unsigned long)> &callback) {

    if (m_running) {
        NMsc::CLogger::Log("Stream is active, can not bind the callback.");
        return false;
    }
    m_callback = callback;
    return true;
}
