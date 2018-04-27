//
// Created by ddl_blue on 15.11.17.
//

#include "../../include/snd/CInstrument.hpp"


using namespace NSnd;

/*----------------------------------------------------------------------*/
CInstrument::CInstrument() : m_sambleFromTickCounter(0) {

}

/*----------------------------------------------------------------------*/
void CInstrument::ReciveMidiMsg(const NSnd::CMidiMsg &msg) {
    m_newMidiMsg.push_back(msg);
}