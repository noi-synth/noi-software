//
// Created by ddl_blue on 15.11.17.
//

#include <cstring>
#include "CInstrument.hpp"
#include "../../include/msc/CLogger.hpp"


using namespace NSnd;

/*----------------------------------------------------------------------*/
CInstrument::CInstrument() : m_sambleFromTickCounter(0), m_mode(EInstrumentMode::POLYPHONE) {

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

    int a = 0;

    for (auto &&voice : m_voices) {
        if (voice.second->IsActive()) {
            voice.second->GenerateBuffer(inputBuff, /*outputBuff*/ m_tmpBuffer, buffLen);
            //++a;
            /*SND_DATA_TYPE *end = outputBuff + 2 * buffLen;
            SND_DATA_TYPE *cursorSrc = m_tmpBuffer;
            SND_DATA_TYPE *cursorDst = outputBuff;

            while (cursorDst != end)
                *cursorDst = *(cursorDst++) + *(cursorSrc++);
            */

            for (int i = 0; i < buffLen * 2;) {
                outputBuff[i] += m_tmpBuffer[i];
                ++i;
                outputBuff[i] += m_tmpBuffer[i];
                ++i;
            }

        }
    }

//    char bff[16];
//    snprintf(bff, 16, "Playing: %d", a);
//
//
//    NMsc::CLogger::Log(bff);
    return 0; //todo
}

/*----------------------------------------------------------------------*/
void CInstrument::AsyncTick() {

    bool debugPrint = false;

    while (!m_newMidiMsg.empty()) {
        debugPrint = true;
        CMidiMsg msg = m_newMidiMsg.front();
        m_newMidiMsg.pop_front();

        switch (msg.m_type) {
            case EMidiMsgType::ALL_OFF:
                for (auto &&s : m_voices) {
                    s.second->Deactivate();
                }
                break;

            case EMidiMsgType::NOTE_ON:
                switch (m_mode) {
                    case EInstrumentMode::MONOPHONE:
                        // Deactivate
                        for (auto &&s : m_voices)
                            if (s.first == msg.m_tone)
                                s.second->Deactivate();
                    default:
                        for (auto &&s: m_voices)
                            if (!s.second->IsActive()) {
                                s.first = msg.m_tone;
                                s.second->Activate(msg);
                                break;
                            }
                }

                break;

            case EMidiMsgType::NOTE_OFF:
                switch (m_mode) {
                    case EInstrumentMode::ONE_SHOT:
                        break;
                    default:
                        for (auto &&s : m_voices) {
                            if (s.second->IsActive() && s.first == msg.m_tone)
                                s.second->Deactivate();
                        }
                }


                break;
            default: //todo

                break;
        }
    }


    if (debugPrint) {
        int debugVoiceCnt = 0;
        for (auto &&s : m_voices)
            if (s.second->IsActive())
                ++debugVoiceCnt;

        char bff[64];
        snprintf(bff, 64, "Active voice cnt: %d", debugVoiceCnt);
        NMsc::CLogger::Log(bff);
    }

}


/*##############################################################################*/
CInstrument::CInstrumentVoice::CInstrumentVoice(CInstrument &instrument, NSnd::ETones tone)
        : m_active(false), m_instrument(instrument), m_tone(tone) {

}

/*----------------------------------------------------------------------*/
bool CInstrument::CInstrumentVoice::IsActive() {
    return m_active;
}

/*----------------------------------------------------------------------*/
void CInstrument::CInstrumentVoice::Activate(NSnd::CMidiMsg midiMessage) {
    m_tone = midiMessage.m_tone;
    m_active = true;
    NMsc::CLogger::Log("Activated");
}

/*----------------------------------------------------------------------*/
void CInstrument::CInstrumentVoice::Deactivate() {
    m_active = false;
    NMsc::CLogger::Log("DeActivated");
}
