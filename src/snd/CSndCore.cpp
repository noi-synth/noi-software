//
// Created by ddl_blue on 7.4.18.
//

#include "../../include/snd/CSndCore.hpp"
#include "../../include/msc/CLogger.hpp"

using namespace NSnd;

/*----------------------------------------------------------------------*/
CSndCore::CSndCore() {
    NMsc::CLogger::Log(NMsc::ELogType::TMP_DEBUG, "CSndCore constructor");

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
    m_selectedCHain.reset();
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
        m_selectedCHain = m_newSelectedChain.Pop();

    if (m_selectedCHain) {
        while (!m_midiMsgQue.Empty())
            m_selectedCHain->ReciveMidiMsg(m_midiMsgQue.Pop());

        return m_selectedCHain->ProcessBuffer(inputBuffer, outputBuffer, sampleCnt);
    }

    return 0; //todo what should I return here?
}

//TODO




