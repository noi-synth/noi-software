//
// Created by ddl_blue on 8.4.18.
//

#include <cstring>
#include <cmath>
#include "CInstrSimpleOsc.hpp"
#include "../../../include/msc/CLogger.hpp"
#include "../../../include/snd/CSndHelpers.hpp"

using namespace NPlg::NInstr;
using namespace NSnd;

/*----------------------------------------------------------------------*/
CInstrSimpleOsc::CInstrSimpleOsc() : m_on(false) {
    for (int i = 0; i < NSnd::INSTRUMENT_VOICE_NUMBER; ++i)
        m_voices.push_back(std::make_pair(ETones::NO_TONE,
                                          std::make_shared<CSimpleOscVoice>(CSimpleOscVoice(*this, ETones::NO_TONE))));
}

/*----------------------------------------------------------------------*/
//int CInstrSimpleOsc::GenerateBuffer(const SND_DATA_TYPE *inputBuff, SND_DATA_TYPE *outputBuff, unsigned long buffLen) {
//
//    unsigned int cur = 0;
//
//    if (m_on)
//        for (int i = 0; i < buffLen; ++i) {
//            outputBuff[cur++] = SIN_WAVE[m_phase & (WAVETABLE_LEN - 1)];
//            outputBuff[cur++] = SIN_WAVE[m_phase & (WAVETABLE_LEN - 1)];
//            ++m_phase;
//        }
//    else
//        memset(outputBuff, 0, sizeof(SND_DATA_TYPE) * buffLen * 2);
//
//    // Should be done in Tick()
//    while (!m_newMidiMsg.empty()) {
//        NSnd::CMidiMsg msg = m_newMidiMsg.front();
//        m_newMidiMsg.pop_front();
//
//        if (msg.m_type == EMidiMsgType::NOTE_ON)
//            m_on = true;
//        else if (msg.m_type == EMidiMsgType::NOTE_OFF || msg.m_type == EMidiMsgType::ALL_OFF)
//            m_on = false;
//    }
//
//    return 0; //todo what should I return here?
//}

/*----------------------------------------------------------------------*/

/*##############################################################################*/

bool CInstrSimpleOsc::CSimpleOscVoice::waveTablesInitialized = false;
const int CInstrSimpleOsc::CSimpleOscVoice::WAVETABLE_LEN;
float CInstrSimpleOsc::CSimpleOscVoice::SIN_WAVE[CInstrSimpleOsc::CSimpleOscVoice::WAVETABLE_LEN];

/*----------------------------------------------------------------------*/
CInstrSimpleOsc::CSimpleOscVoice::CSimpleOscVoice(CInstrument &instrument, NSnd::ETones tone)
        : CInstrument::CInstrumentVoice(instrument, tone), m_phase(0) {
    if (!waveTablesInitialized) {
        for (int i = 0; i < 512; ++i) {
            //SIN_WAVE[i] = sin(i / (double) WAVETABLE_LEN * M_PI);
            SIN_WAVE[i] = (i < WAVETABLE_LEN / 2) ? 1 : -1;
        }

        waveTablesInitialized = true;
        NMsc::CLogger::Log(NMsc::ELogType::TMP_DEBUG, "CSimpleOscVoice tables initialized.");
    }
}

/*----------------------------------------------------------------------*/
void CInstrSimpleOsc::CSimpleOscVoice::Activate(NSnd::CMidiMsg midiMessage) {
    CInstrument::CInstrumentVoice::Activate(midiMessage);
    m_sampleStep = CSndHelpers::GetSampleStep(m_tone, WAVETABLE_LEN);
}

/*----------------------------------------------------------------------*/
int CInstrSimpleOsc::CSimpleOscVoice::GenerateBuffer(const SND_DATA_TYPE *inputBuff, SND_DATA_TYPE *outputBuff,
                                                     unsigned long buffLen) {
    unsigned int cur = 0;

    if (m_active)
        for (int i = 0; i < buffLen; ++i) {
            outputBuff[cur++] = SIN_WAVE[(int) m_phase] * 0.2;
            outputBuff[cur++] = SIN_WAVE[(int) m_phase] * 0.2;
            m_phase = fmod(m_phase + m_sampleStep, WAVETABLE_LEN);

//
//
//      while(m_phase > WAVETABLE_LEN)
//                m_phase -= WAVETABLE_LEN;
        }
    else
        memset(outputBuff, 0, sizeof(SND_DATA_TYPE) * buffLen * 2);

//
//    char bff [64];
//    snprintf(bff, 64, "gen %lf/%lf", m_phase, m_sampleStep);
//    NMsc::CLogger::Log(bff);

    return 0; //todo what should I return here?

}