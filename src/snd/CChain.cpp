//
// Created by ddl_blue on 27.12.17.
//

#include "../../include/snd/CChain.hpp"
#include "../../include/msc/CLogger.hpp"

using namespace NSnd;

/*----------------------------------------------------------------------*/
CChain::CChain(const AInstrument &instr) : m_instrument(instr), m_active(false), m_uiInstrument(instr) {
    m_uiEffectChain = m_effectChain = std::make_shared<std::vector<AEffect>>();
    NMsc::CLogger::Log(NMsc::ELogType::TMP_DEBUG, "CChain constructor");

}

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
    m_uiInstrument = instrument;
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
        m_instrument->GenerateBuffer(inputBuff, outputBuff, buffLen);

    // Change the eff chain
    while (!m_newEffectChains.Empty()) {

        if (m_effectChain)
            m_toDeleteEffectChains.Push(m_effectChain);
        m_effectChain = m_newEffectChains.Pop();
    }

    // Process the eff chain
    if (m_effectChain)
        for (const auto &eff : *m_effectChain)
            if (eff)
                eff->ProcessBuffer(outputBuff, buffLen);


    return 0; // todo what should I return?
}

/*----------------------------------------------------------------------*/
bool CChain::EffectChainChange(const std::vector<NSnd::AEffect> &newEffChain) {

    // Clear chains to be deleted
    m_toDeleteInstruments.Clear();

    AEffectChain effectChain = std::make_shared<std::vector<AEffect>>(newEffChain);

    m_uiEffectChain = effectChain;

    // Push new eff chain
    m_newEffectChains.Push(effectChain);
}

/*----------------------------------------------------------------------*/
std::vector<AEffect> CChain::EffectChainGet() {
    // Return a clone of "UI" active effect chain

    if (!m_uiEffectChain) {
        NMsc::CLogger::Log(NMsc::ELogType::WARNING, "CChain: There is no effect chain!");
        return std::vector<AEffect>();
    }

    return std::vector<AEffect>(*m_uiEffectChain);
}

/*----------------------------------------------------------------------*/
AInstrument CChain::InstrumentGet() {
    return m_uiInstrument;
}

