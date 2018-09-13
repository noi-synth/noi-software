//
// Created by ddl_blue on 27.12.17.
//

#include "../../include/snd/CChain.hpp"
#include "../../include/msc/CLogger.hpp"

using namespace NSnd;

/*----------------------------------------------------------------------*/
CChain::CChain(const AInstrument &instr) : m_instrument(instr), m_active(false) {}

/*----------------------------------------------------------------------*/
void CChain::MakrActive() {
    //TODO make sure this mechanic is used properly
    m_active = true;
}

/*----------------------------------------------------------------------*/
void CChain::MarkInactive() {
    m_active = false;
}

/*----------------------------------------------------------------------*/
bool CChain::InstrumentChange(const NSnd::AInstrument &instrument) {
    //TODO make sure all these return bools are checked
    return m_newInstrument.Push(instrument);
}

/*----------------------------------------------------------------------*/
bool CChain::ReciveMidiMsg(const NSnd::CMidiMsg &message) {
    if (!m_instrument)
        return false;
    m_instrument->ReciveMidiMsg(message);
    return false;
}

/*----------------------------------------------------------------------*/
int CChain::ProcessBuffer(const SND_DATA_TYPE *inputBuff, SND_DATA_TYPE *outputBuff, unsigned long buffLen) {
    m_active = true;

    //todo do this at tick
    // Replace instrument with ne one
    while (!m_newInstrument.Empty()) {
        if (!m_toDeleteInstruments.Push(m_instrument)) {
            NMsc::CLogger::Log(NMsc::ELogType::RT_ERROR, "ERROR: Chain: instrument deletion que is full.");
        }
        m_instrument = m_newInstrument.Pop();
        //TODO wake up thread (and implement it) (Make ThrashBinCLass)

    }

    if (m_instrument)
        return m_instrument->GenerateBuffer(inputBuff, outputBuff, buffLen);

    return 0; // todo what should I return?

}