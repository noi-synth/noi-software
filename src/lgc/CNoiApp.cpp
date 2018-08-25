//
// Created by ddl_blue on 2.8.18.
//

#include "../../include/lgc/CNoiApp.hpp"
#include "../../plg/instr/SimpleOsc/CInstrSimpleOsc.hpp"
#include "../../include/msc/CLogger.hpp"

using namespace NLgc;

/*----------------------------------------------------------------------*/
CNoiApp::CNoiApp() {
    // Create SndCore
    m_state.m_soundCore = std::make_shared<NSnd::CSndCore>();

    // Create instrument and chain
    NSnd::AInstrument instrument = std::make_shared<NPlg::NInstr::CInstrSimpleOsc>();
    m_state.m_chains.push_back(std::make_shared<NSnd::CChain>(instrument));
    //m_state.m_soundCore.get();
    m_state.m_soundCore->ChainSelect(m_state.m_chains[0]);
    NMsc::CLogger::Log("App constructor. m_sndCore = %", m_state.m_soundCore.get());

}

/*----------------------------------------------------------------------*/
bool CNoiApp::AudioDeviceSet(NSnd::AAudioDevice device) {
    m_state.m_soundCore->AudioDeviceSet(device);
}

/*----------------------------------------------------------------------*/
void CNoiApp::AudioDeviceUnset() {
    m_state.m_soundCore->AudioDeviceUnset();
}

/*----------------------------------------------------------------------*/
bool CNoiApp::AudioStart() {
    return m_state.m_soundCore->AudioDeviceStart();
}

/*----------------------------------------------------------------------*/
bool CNoiApp::AudioStop() {
    return m_state.m_soundCore->AudioDeviceStop();
}

/*----------------------------------------------------------------------*/
void CNoiApp::AudioPanic() {
    m_state.m_soundCore->Panic();
}

/*----------------------------------------------------------------------*/
bool CNoiApp::SendMidiMessage(NSnd::CMidiMsg message) {

    /*NMsc::CLogger::Log("CNoiApp: SendMidiMsg; This = %", this);
    NMsc::CLogger::Log("CNoiApp: SendMidiMsg; m_state = %", &m_state);*/

    if (!m_state.m_soundCore) {
        NMsc::CLogger::Log("SndCore not set");
        return false;
    }
    return m_state.m_soundCore->ReciveMidiMsg(message);
}