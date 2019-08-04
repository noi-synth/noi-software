//
// Created by ddl_blue on 24.6.19.
//

#include "../../include/snd/CAdsr.hpp"
#include "../../include/msc/CLogger.hpp"
#include "../../include/snd/NSndConfig.hpp"

using namespace NSnd;

/*----------------------------------------------------------------------*/
CAdsr::CAdsr(NSnd::AAdsrSettings &settings) : m_state(ADSR_OFF), m_volume(0), m_deltaVolume(0), m_settings(settings) {}

/*----------------------------------------------------------------------*/
CAdsr::CAdsr() : m_state(ADSR_OFF), m_volume(0), m_deltaVolume(0) {
    m_settings = std::make_shared<CAdsrSettings>();
}

/*----------------------------------------------------------------------*/
void CAdsr::StartNote() {
    SetupState(ADSR_ATTACK);
}

/*----------------------------------------------------------------------*/
void CAdsr::StopNote() {
    if (m_state == ADSR_OFF || m_state == ADSR_RELEASE)
        return;

    SetupState(ADSR_RELEASE);
}

/*----------------------------------------------------------------------*/
double CAdsr::GetVolume() {
    return m_volume;
    /*double volume = m_volume;

    // more curvy shape
    if (m_state == ADSR_ATTACK){
        volume = 1-volume;
        volume = 1-(volume*volume);
    }else{
        volume *= volume;
    }

    return volume;*/
}

/*----------------------------------------------------------------------*/
void CAdsr::Tick() {
    double sustain = m_settings->GetSustain();
    switch (m_state) {
        case ADSR_OFF:
            break;

        case ADSR_ATTACK:
            m_volume += m_deltaVolume;
            if (m_volume >= 1.0) {
                m_volume = 1;
                SetupState(ADSR_DECAY);
            }
            break;

        case ADSR_DECAY:


            if (m_deltaVolume > 1) {
                NMsc::CLogger::Log(NMsc::ELogType::RT_WARNING,
                                   "CAdsr: Positive delta volume on decay. Delta volume = %", m_deltaVolume);
                break;
            }

            if (m_volume > sustain)
                m_volume += m_deltaVolume;

            if (m_volume <= sustain)
                SetupState(ADSR_SUSTAIN);

            break;

        case ADSR_SUSTAIN:
            return;

        case ADSR_RELEASE:
            if (m_deltaVolume > 1) {
                NMsc::CLogger::Log(NMsc::ELogType::RT_WARNING,
                                   "CAdsr: Positive delta volume on release. Delta volume = %", m_deltaVolume);
                return;
            }

            if (m_volume > 0)
                m_volume += m_deltaVolume;

            if (m_volume <= 0)
                SetupState(ADSR_OFF);
            break;
    }
}

/*----------------------------------------------------------------------*/
bool CAdsr::GetNoteOn() {
    return m_state != ADSR_OFF;
}

/*----------------------------------------------------------------------*/
void CAdsr::SetupState(NSnd::CAdsr::AsdrState state) {
    m_state = state;
    m_deltaVolume = 0;

    double attackLen = (double) m_settings->GetAttackSampleLen() / TICK_LENGTH;
    double decayLen = (double) m_settings->GetDecaySampleLen() / TICK_LENGTH;
    double releaseLen = (double) m_settings->GetReleaseSampleLen() / TICK_LENGTH;

    switch (state) {
        case ADSR_OFF:
            m_volume = 0;
            break;

        case ADSR_ATTACK:

            if (attackLen > 0) {
                m_deltaVolume = 1 / attackLen;
                m_volume = 0;
            } else {
                m_deltaVolume = 0;
                m_volume = 1;
            }
            break;

        case ADSR_DECAY:
            if (decayLen > 0) {
                m_volume = 1;
                m_deltaVolume = -(1 - m_settings->GetSustain()) / decayLen;
            } else {
                m_volume = m_settings->GetSustain();
                m_deltaVolume = 0;
            }
            break;

        case ADSR_SUSTAIN:
            m_volume = m_settings->GetSustain();
            break;

        case ADSR_RELEASE:
            if (releaseLen > 0) {
                m_deltaVolume = -m_volume / releaseLen;
            } else {
                m_deltaVolume = -m_volume;
            }
            return;
    }
}