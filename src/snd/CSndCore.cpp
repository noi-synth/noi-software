//
// Created by ddl_blue on 7.4.18.
//

#include <cstring>
#include "../../include/snd/CSndCore.hpp"
#include "../../include/msc/CLogger.hpp"

using namespace NSnd;

/*----------------------------------------------------------------------*/
CSndCore::CSndCore() : m_bpm(0), m_metronomeEnabled(true) {
    NMsc::CLogger::Log(NMsc::ELogType::TMP_DEBUG, "CSndCore constructor");

    m_trackManager = std::make_shared<CTrackManager>();
    ATrack firstTrack;

    // todo this probably is not the best idea...
    while (!firstTrack)
        firstTrack = m_trackManager->CreateTrack();




}

/*----------------------------------------------------------------------*/
bool CSndCore::AudioDeviceSet(NSnd::AAudioDevice &device) {
    AudioDeviceStop();
    m_audioDevice = device;
    if (!m_audioDevice->BindCallback(
            [=]
                    (const SND_DATA_TYPE *inputBuffer, SND_DATA_TYPE *outputBuffer,
                     unsigned long sampleCnt) {
                //NMsc::CLogger::Log("Lambda happenned");
                return AudioDevCallback(inputBuffer, outputBuffer, sampleCnt);

            }
    ) || !m_audioDevice->Open()) {
        m_audioDevice.reset();
        return false;
    }

}

/*----------------------------------------------------------------------*/
void CSndCore::AudioDeviceUnset() {
    AudioDeviceStop();
    m_audioDevice.reset();
}

/*----------------------------------------------------------------------*/
bool CSndCore::AudioDeviceStart() {
    if (m_audioDevice)
        return m_audioDevice->StartStream();

    NMsc::CLogger::Log(NMsc::ELogType::ERROR, "NsdCore: AudioDeviceStart: no device selected.");
    return false;
}

/*----------------------------------------------------------------------*/
bool CSndCore::AudioDeviceStop() {
    if (m_audioDevice)
        return m_audioDevice->StopStream();
    return false;
}

/*----------------------------------------------------------------------*/
bool CSndCore::ReciveMidiMsg(const NSnd::CMidiMsg &message) {
    return m_midiMsgQue.Push(message);
}

/*----------------------------------------------------------------------*/
bool CSndCore::ChainSelect(NSnd::AChain &chain) {
    return m_newSelectedChain.Push(chain);
}

/*----------------------------------------------------------------------*/
void CSndCore::ChainClearAllActive() {
    m_shouldClearActiveCHains = true;
}

/*----------------------------------------------------------------------*/
void CSndCore::Panic() {
    AudioDeviceStop();
    m_newSelectedChain.Clear();
    m_activeChains.clear();
    m_selectedChain.reset();
}

/*----------------------------------------------------------------------*/
//int CSndCore::AudioDevCallbackFunction(const SND_DATA_TYPE *inputBuffer, SND_DATA_TYPE *outputBuffer,
//                                       unsigned long sampleCnt, void *caller) {
//    if (!caller)
//        return -1; // TODO does this return val make sense?
//    return ((CSndCore *) caller)->AudioDevCallback(inputBuffer, outputBuffer, sampleCnt);
//}

/*----------------------------------------------------------------------*/
int CSndCore::AudioDevCallback(const SND_DATA_TYPE *inputBuffer, SND_DATA_TYPE *outputBuffer,
                               unsigned long sampleCnt) {
    //todo mixer
    //todo active chains , proper deletion

    while (!m_newSelectedChain.Empty())
        m_selectedChain = m_newSelectedChain.Pop();

    if (m_selectedChain) {
        while (!m_midiMsgQue.Empty())
            m_selectedChain->ReciveMidiMsg(m_midiMsgQue.Pop());

        // todo process all the active chains
        int ret = m_selectedChain->ProcessBuffer(inputBuffer, outputBuffer, sampleCnt);
    }

    // Metronome
    if (m_metronomeEnabled && m_trackManager->IsPlaying()) {
        const int bpmModulo = SAMPLE_RATE * 60 / m_bpm;
        int positionInBeat = m_trackManager->GetPlaybackPosition() % bpmModulo;

        // todo do this properly
        if (positionInBeat < 1000) {
            for (unsigned int i = 0; i < sampleCnt * 2 && i < 10000; ++i) {
                outputBuffer[i] += (i & 512) - 0.5;
            }
        }
    }

    //memset(outputBuffer, 0, sampleCnt<<1);
    memcpy(m_buffer, outputBuffer, sampleCnt << 1);
    m_trackManager->ProcessBuffer(m_buffer, outputBuffer, sampleCnt);


    return 0; //todo what should I return here?
}

/*----------------------------------------------------------------------*/
bool CSndCore::TrackRecordingStart() {
    return m_trackManager->StartRecording();
}

/*----------------------------------------------------------------------*/
bool CSndCore::TrackRecordingStop() {
    return m_trackManager->StopRecording();
}

/*----------------------------------------------------------------------*/
bool CSndCore::IsRecording() {
    return m_trackManager->IsRecording();
}

/*----------------------------------------------------------------------*/
bool CSndCore::TrackPlaybackStart() {
    return m_trackManager->StartPlayback();
}

/*----------------------------------------------------------------------*/
bool CSndCore::TrackPlaybackStop() {
    return m_trackManager->StopPlayback();
}

/*----------------------------------------------------------------------*/
bool CSndCore::IsPlaying() {
    return m_trackManager->IsPlaying();
}

/*----------------------------------------------------------------------*/
uint32_t CSndCore::TrackGetPosition() {
    return m_trackManager->GetPlaybackPosition();
}

/*----------------------------------------------------------------------*/
bool CSndCore::TrackSetPosition(uint32_t position) {
    return m_trackManager->SetPlaybackPosition(position);
}

//TODO


