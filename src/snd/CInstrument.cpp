//
// Created by ddl_blue on 15.11.17.
//

#include <cstring>
#include "CInstrument.hpp"


using namespace NSnd;

/*----------------------------------------------------------------------*/
CInstrument::CInstrument() : m_sambleFromTickCounter(0) {

}

/*----------------------------------------------------------------------*/
void CInstrument::ReciveMidiMsg(const NSnd::CMidiMsg &msg) {
    m_newMidiMsg.push_back(msg);
}

/*----------------------------------------------------------------------*/
void CInstrument::Tick() {
    this->AsyncTick();
}

/*----------------------------------------------------------------------*/
int CInstrument::GenerateBuffer(const SND_DATA_TYPE *inputBuff, SND_DATA_TYPE *outputBuff, unsigned long buffLen) {
    this->AsyncTick();
    memset(outputBuff, 0, buffLen * 2 * sizeof(SND_DATA_TYPE));

    for (auto &&voice : m_voices) {
        voice.second->GenerateBuffer(inputBuff, m_tmpBuffer, buffLen);

        SND_DATA_TYPE *end = outputBuff + 2 * buffLen;
        SND_DATA_TYPE *cursorSrc = m_tmpBuffer;
        SND_DATA_TYPE *cursorDst = outputBuff;

        while (cursorDst != end)
            *cursorDst = *(cursorDst++) + *(cursorSrc++);


    }

    return 0; //todo
}

/*----------------------------------------------------------------------*/
void CInstrument::AsyncTick() {
    while (!m_newMidiMsg.empty()) {
        CMidiMsg msg = m_newMidiMsg.front();
        m_newMidiMsg.pop_front();

        switch (msg.m_type) {
            case EMidiMsgType::ALL_OFF:
                for (auto &&s : m_voices) {
                    s.second->Dectivate();
                }
                break;

            case EMidiMsgType::NOTE_ON:
                switch (m_mode) {
                    case EInstrumentMode::MONOPHONE:
                        // Deactivate
                        for (auto &&s : m_voices)
                            if (s.first == msg.m_tone)
                                s.second->Dectivate();
                    default:
                        for (auto &&s: m_voices)
                            if (!s.second->IsActive()) {
                                s.first = msg.m_tone;
                                s.second->Activate(msg);
                                break;
                            }
                }

            case EMidiMsgType::NOTE_OFF:
                switch (m_mode) {
                    case EInstrumentMode::ONE_SHOT:
                        break;
                    default:
                        for (auto &&s : m_voices) {
                            if (s.second->IsActive())
                                s.second->Dectivate();
                        }
                }


                break;
            default: //todo

                break;
        }


    }
}


/*##############################################################################*/
CInstrument::CInstrumentVoice::CInstrumentVoice(std::weak_ptr<NSnd::CInstrument> instrument, NSnd::ETones tone)
        : m_active(false), m_instrument(instrument), m_tone(tone) {

}

/*----------------------------------------------------------------------*/
bool CInstrument::CInstrumentVoice::IsActive() {
    return m_active;
}