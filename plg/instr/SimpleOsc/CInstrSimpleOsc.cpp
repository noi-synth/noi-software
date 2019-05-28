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
CInstrSimpleOsc::CInstrSimpleOsc() : m_on(false), m_oscillatorRatio(0.5), m_phaseShift(0), m_firstOscIndex(0),
                                     m_secondOscIndex(1), m_paramChangeDelay(0) {
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

/*----------------------------------------------------------------------*/
CInstrSimpleOsc::CSimpleOscVoice::CSimpleOscVoice(CInstrSimpleOsc &instrument, NSnd::ETones tone)
        : CInstrument::CInstrumentVoice(instrument, tone), m_phase(0), m_owner(instrument) {
    if (!waveTablesInitialized) {
        for (int i = 0; i < WAVETABLE_LEN; ++i) {
            SIN_WAVE[i] = sin(i / (double) WAVETABLE_LEN * M_PI);
            SAW_WAVE[i] = -1 + 2 * (i / (double) WAVETABLE_LEN);
            /*TRIANGLE_WAVE[i] = (i < WAVETABLE_LEN / 4) ? (-1 + 1 * ((double) i / (WAVETABLE_LEN / 4))) :
                               (i < WAVETABLE_LEN / 4 * 3) ? (1 - (double) 2 * (i - WAVETABLE_LEN / 4) /
                                                                  (WAVETABLE_LEN / 2)) :
                               -1 + 1 * ((i - ((double) WAVETABLE_LEN / 4 * 3)) / ((double) WAVETABLE_LEN / 4));*/

            TRIANGLE_WAVE[i] = -1 + std::fabs((i - (double) WAVETABLE_LEN / 2)) / (double) WAVETABLE_LEN * 2;

            SQUARE50_WAVE[i] = (i < WAVETABLE_LEN / 2) ? 1 : -1;
            SQUARE25_WAVE[i] = (i < WAVETABLE_LEN / 4) ? 1 : -1;
            SQUARE10_WAVE[i] = (i < WAVETABLE_LEN / 10) ? 1 : -1;
        }

        waveTablesInitialized = true;
        NMsc::CLogger::Log(NMsc::ELogType::TMP_DEBUG, "CSimpleOscVoice tables initialized.");
    }
}

float CInstrSimpleOsc::CSimpleOscVoice::SIN_WAVE[CInstrSimpleOsc::CSimpleOscVoice::WAVETABLE_LEN];
float CInstrSimpleOsc::CSimpleOscVoice::TRIANGLE_WAVE[CInstrSimpleOsc::CSimpleOscVoice::WAVETABLE_LEN];
float CInstrSimpleOsc::CSimpleOscVoice::SAW_WAVE[CInstrSimpleOsc::CSimpleOscVoice::WAVETABLE_LEN];
float CInstrSimpleOsc::CSimpleOscVoice::SQUARE50_WAVE[CInstrSimpleOsc::CSimpleOscVoice::WAVETABLE_LEN];
float CInstrSimpleOsc::CSimpleOscVoice::SQUARE25_WAVE[CInstrSimpleOsc::CSimpleOscVoice::WAVETABLE_LEN];
float CInstrSimpleOsc::CSimpleOscVoice::SQUARE10_WAVE[CInstrSimpleOsc::CSimpleOscVoice::WAVETABLE_LEN];

/*----------------------------------------------------------------------*/
void CInstrSimpleOsc::CSimpleOscVoice::Activate(NSnd::CMidiMsg midiMessage) {
    CInstrument::CInstrumentVoice::Activate(midiMessage);
    m_sampleStep = CSndHelpers::GetSampleStep(m_tone, WAVETABLE_LEN);
}

/*----------------------------------------------------------------------*/
int CInstrSimpleOsc::CSimpleOscVoice::GenerateBuffer(const SND_DATA_TYPE *inputBuff, SND_DATA_TYPE *outputBuff,
                                                     unsigned long buffLen,
                                                     const CTimeInfo &timeInfo) {

    unsigned int cur = 0;

    SND_DATA_TYPE *tables[WAVE_CNT] = {SIN_WAVE, TRIANGLE_WAVE, SAW_WAVE, SQUARE50_WAVE, SQUARE25_WAVE, SQUARE10_WAVE};

    SND_DATA_TYPE *wave1 = tables[m_owner.m_firstOscIndex];
    SND_DATA_TYPE *wave2 = tables[m_owner.m_secondOscIndex];

    if (m_active)
        for (int i = 0; i < buffLen; ++i) {
            outputBuff[cur++] = wave1[(int) m_phase] * m_owner.m_oscillatorRatio
                                + wave2[(int) fmod(m_phase + m_owner.m_phaseShift * WAVETABLE_LEN, WAVETABLE_LEN)] *
                                  (1 - m_owner.m_oscillatorRatio);

            outputBuff[cur++] = wave1[(int) m_phase] * m_owner.m_oscillatorRatio
                                + wave2[(int) fmod(m_phase + m_owner.m_phaseShift * WAVETABLE_LEN, WAVETABLE_LEN)] *
                                  (1 - m_owner.m_oscillatorRatio);
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

/*----------------------------------------------------------------------*/
void CInstrSimpleOsc::ProcessInputChanges() {
    // Parameter change
    while (!m_paramChanges.Empty()) {
        CParamScroll change = m_paramChanges.Pop();

        NMsc::CLogger::Log(NMsc::ELogType::TMP_DEBUG, "CSimpleOscVoice: got % from %", change.m_directionUp,
                           change.m_paramId);

        switch (change.m_paramId) {
            case 0:
                if (change.m_directionUp)
                    m_phaseShift = fmod(m_phaseShift + 0.05, 1);
                else
                    m_phaseShift = fmod(m_phaseShift + 0.95, 1);

                NMsc::CLogger::Log(NMsc::ELogType::TMP_DEBUG, "CSimpleOscVoice: phase shift = %", m_phaseShift);
                break;


            case 1:
                if (change.m_directionUp && m_oscillatorRatio < 1)
                    m_oscillatorRatio += 0.05;

                if (!change.m_directionUp && m_oscillatorRatio > 0)
                    m_oscillatorRatio -= 0.05;

                NMsc::CLogger::Log(NMsc::ELogType::TMP_DEBUG, "CSimpleOscVoice: oscillator ratio = %",
                                   m_oscillatorRatio);
                break;

            case 2:
                if (m_paramChangeDelay) {
                    --m_paramChangeDelay;
                } else {
                    m_paramChangeDelay = 2;
                    if (change.m_directionUp)
                        m_firstOscIndex =
                                m_firstOscIndex >= CInstrSimpleOsc::CSimpleOscVoice::WAVE_CNT - 1 ? 0 :
                                m_firstOscIndex + 1;
                    else
                        m_firstOscIndex = m_firstOscIndex ? m_firstOscIndex - 1 :
                                          CInstrSimpleOsc::CSimpleOscVoice::WAVE_CNT - 1;
                }

                NMsc::CLogger::Log(NMsc::ELogType::TMP_DEBUG, "CSimpleOscVoice: first oscillator = %", m_firstOscIndex);
                break;

            case 3:
                if (m_paramChangeDelay) {
                    --m_paramChangeDelay;
                } else {
                    m_paramChangeDelay = 2;
                    if (change.m_directionUp)
                        m_secondOscIndex =
                                m_secondOscIndex >= CInstrSimpleOsc::CSimpleOscVoice::WAVE_CNT - 1 ? 0 :
                                m_secondOscIndex + 1;
                    else
                        m_secondOscIndex = m_secondOscIndex ? m_secondOscIndex - 1 :
                                           CInstrSimpleOsc::CSimpleOscVoice::WAVE_CNT -
                                           1;
                }

                NMsc::CLogger::Log(NMsc::ELogType::TMP_DEBUG, "CSimpleOscVoice: second oscillator = %",
                                   m_secondOscIndex);
                break;

        }

    }
}
