//
// Created by ddl_blue on 27.5.19.
//

#include "CSequencer.hpp"

using namespace NPlg::NSequencer;
using namespace NSnd;

/*----------------------------------------------------------------------*/
CSequencer::CSequencer(AToneSequence &sequence) : m_sequence(sequence), m_sequencePosition(0), m_time(0),
                                                  m_sequenceStartTime(0), m_speed(4), m_playing(
                false), m_baseTone(ETones::NO_TONE) {

}

/*----------------------------------------------------------------------*/
void CSequencer::Tick(NSnd::CInstrument &instrument, const NSnd::CTimeInfo &timeInfo) {
    m_time = timeInfo.m_trackTime;

    while (!m_newSequence.Empty())
        m_sequence = m_newSequence.Pop(); //todo trash bin

    if (!m_sequence || !m_playing || m_sequence->empty())
        return;

    // in case play button was just pressed
    if (timeInfo.m_trackTime < m_sequenceStartTime) {
        m_sequenceStartTime = timeInfo.m_trackTime;
        m_sequencePosition = 0;
    }

    uint32_t timeInSequence = timeInfo.m_trackTime - m_sequenceStartTime;
    uint64_t desiredSequencePosition = ((double) timeInSequence) / NSnd::SAMPLE_RATE * timeInfo.m_bpm / 60 * m_speed;
    desiredSequencePosition = desiredSequencePosition % m_sequence->size();

    while (m_sequencePosition < desiredSequencePosition || m_sequencePosition > desiredSequencePosition) {
        ++m_sequencePosition;

        if (m_sequencePosition >= m_sequence->size()) {
            m_sequencePosition = 0;

        }

//        NMsc::CLogger::Log(NMsc::ELogType::TMP_DEBUG, "sequence position = %", m_sequencePosition);

        // Stop previous tones
        instrument.ReceiveMidiMsg(CMidiMsg(EMidiMsgType::ALL_OFF, ETones::NO_TONE, 255), true);

        // Start new tones
        for (const auto &tone : (*m_sequence)[m_sequencePosition])
            instrument.ReceiveMidiMsg(CMidiMsg(EMidiMsgType::NOTE_ON, NMsc::Functions::EnumAdd(m_baseTone, tone), 255),
                                      true);

    }
}

/*----------------------------------------------------------------------*/
void CSequencer::RecieveMidiMessage(const NSnd::CMidiMsg &midiMsg, NSnd::CInstrument &instrument) {
    if (!m_sequence)
        return;

    if (midiMsg.m_type == EMidiMsgType::NOTE_ON) {
        if (m_baseTone == ETones::NO_TONE) {
            m_sequencePosition = 0;
            m_sequenceStartTime = m_time;
        }

        // Stop all previous tones
        instrument.ReceiveMidiMsg(CMidiMsg(EMidiMsgType::ALL_OFF, ETones::NO_TONE, 255), true);

        m_playing = true;
        m_baseTone = midiMsg.m_tone;

        // Start new tones
        if (m_sequence)
            for (const auto &tone : (*m_sequence)[m_sequencePosition])
                instrument.ReceiveMidiMsg(
                        CMidiMsg(EMidiMsgType::NOTE_ON, NMsc::Functions::EnumAdd(m_baseTone, tone), 255), true);

    } else if ((midiMsg.m_type == EMidiMsgType::NOTE_OFF && m_baseTone == midiMsg.m_tone) ||
               midiMsg.m_type == EMidiMsgType::ALL_OFF) {
        m_playing = false;

        // Stop all tones
        instrument.ReceiveMidiMsg(CMidiMsg(EMidiMsgType::ALL_OFF, ETones::NO_TONE, 255), true);
    }
}


