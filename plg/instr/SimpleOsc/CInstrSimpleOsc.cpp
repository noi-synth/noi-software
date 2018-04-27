//
// Created by ddl_blue on 8.4.18.
//

#include <cstring>
#include "CInstrSimpleOsc.hpp"
#include "../../../include/msc/CLogger.hpp"

using namespace NPlg::NInstr;
using namespace NSnd;

/*----------------------------------------------------------------------*/
CInstrSimpleOsc::CInstrSimpleOsc() : m_phase(0), m_on(false) {

}

/*----------------------------------------------------------------------*/
int CInstrSimpleOsc::GenerateBuffer(const SND_DATA_TYPE *inputBuff, SND_DATA_TYPE *outputBuff, unsigned long buffLen) {

    unsigned int cur = 0;

    if (m_on)
        for (int i = 0; i < buffLen; ++i) {
            outputBuff[cur++] = ((m_phase) & 512) * .5 + ((m_phase) & 128) * .5;
            outputBuff[cur++] = ((m_phase) & 512) * .5 + ((m_phase) & 128) * .5;
            ++m_phase;
        }
    else
        memset(outputBuff, 0, sizeof(SND_DATA_TYPE) * buffLen * 2);

    // Should be done in Tick()
    while (!m_newMidiMsg.empty()) {
        NSnd::CMidiMsg msg = m_newMidiMsg.front();
        m_newMidiMsg.pop_front();

        if (msg.m_type == EMidiMsgType::NOTE_ON)
            m_on = true;
        else if (msg.m_type == EMidiMsgType::NOTE_OFF || msg.m_type == EMidiMsgType::ALL_OFF)
            m_on = false;
    }

    return 0; //todo what should I return here?
}

/*----------------------------------------------------------------------*/
void CInstrSimpleOsc::Tick() {
    //todo
}