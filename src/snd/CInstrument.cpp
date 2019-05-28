//
// Created by ddl_blue on 15.11.17.
//

#include <cstring>
#include "../../include/snd/CInstrument.hpp"
#include "../../include/msc/CLogger.hpp"
#include "../../include/snd/CTimeInfo.hpp"


using namespace NSnd;

/*----------------------------------------------------------------------*/
CInstrument::CInstrument() : m_sambleFromTickCounter(0), m_mode(EInstrumentMode::POLYPHONE) {

}

/*----------------------------------------------------------------------*/
void CInstrument::ReceiveMidiMsg(const CMidiMsg &msg, bool midiProcessed) {

    AMidiProcessor processor = m_midiProcessor;

    if (m_midiProcessor && !midiProcessed)
        processor->RecieveMidiMessage(msg, *this);
    else
        m_newMidiMsg.Push(msg);
}

/*----------------------------------------------------------------------*/
void CInstrument::Tick(const CTimeInfo &timeInfo) {
    this->AsyncTick();

    // Tick of midi processor
    if (m_midiProcessor)
        m_midiProcessor->Tick(*this, timeInfo);
}

/*----------------------------------------------------------------------*/
int CInstrument::GenerateBuffer(const SND_DATA_TYPE *inputBuff, SND_DATA_TYPE *outputBuff, unsigned long buffLen,
                                const CTimeInfo &timeInfo) {

    this->AsyncTick();

    {
        // Synchronous ticks
        CTimeInfo now = timeInfo;
        CTimeInfo afterThisCallback = timeInfo;
        afterThisCallback.AddTime(buffLen);
        uint64_t tickCnt = afterThisCallback.GetTickNumber() - now.GetTickNumber();
        for (uint64_t i = 0; i < tickCnt; ++i) {
            Tick(timeInfo);
        }
    }


    memset(outputBuff, 0, buffLen * 2 * sizeof(SND_DATA_TYPE));

    int a = 0;

    for (auto &&voice : m_voices) {
        if (voice.second->IsActive()) {
            voice.second->GenerateBuffer(inputBuff, /*outputBuff*/ m_tmpBuffer, buffLen, timeInfo);
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

    // New midi processor
    while (!m_newMidiProcessor.Empty())
        m_midiProcessor = m_newMidiProcessor.Pop();

    ProcessInputChanges();

    bool debugPrint = false;

    while (!m_newMidiMsg.Empty()) {
        debugPrint = true;
        CMidiMsg msg = m_newMidiMsg.Pop();


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

        /*char bff[64];
        snprintf(bff, 64, "Active voice cnt: %d", debugVoiceCnt);
        NMsc::CLogger::Log(bff);*/
    }

}

/*----------------------------------------------------------------------*/
void CInstrument::ApplyMidiProcessor(NSnd::AMidiProcessor &processor) {
    m_newMidiProcessor.Push(processor);
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
    //NMsc::CLogger::Log("Activated");
}

/*----------------------------------------------------------------------*/
void CInstrument::CInstrumentVoice::Deactivate() {
    m_active = false;
    //NMsc::CLogger::Log("DeActivated");
}

/*----------------------------------------------------------------------*/
void CInstrument::ProcessInputChanges() {
    if (!m_paramChanges.Empty())
        m_paramChanges.Clear();
}