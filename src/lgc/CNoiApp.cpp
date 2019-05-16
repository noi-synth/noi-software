//
// Created by ddl_blue on 2.8.18.
//

#include <fstream>
#include "../../include/lgc/CNoiApp.hpp"
#include "../../plg/instr/SimpleOsc/CInstrSimpleOsc.hpp"
#include "../../include/msc/CLogger.hpp"
#include "../../include/msc/CSerializationNode.hpp"

using namespace NLgc;

/*----------------------------------------------------------------------*/
CNoiApp::CNoiApp() : m_octave(3) {
    // Create SndCore
    m_state.m_soundCore = std::make_shared<NSnd::CSndCore>();

    m_state.m_bpm = 120;
    m_state.m_soundCore->BpmSet(m_state.m_bpm);

    /*for (int i = 0; i < 4; ++i) {
        TrackCreate();
    }*/

    // Create instrument and chain
    NSnd::AInstrument instrument = std::make_shared<NPlg::NInstr::CInstrSimpleOsc>();
    m_state.m_chains.push_back(std::make_shared<NSnd::CChain>(instrument));
    //m_state.m_soundCore.get();
    ChainActiveSet(m_state.m_chains[0]);
    //m_state.m_soundCore->ChainSelect(m_state.m_chains[0]);
    NMsc::CLogger::Log(NMsc::ELogType::TMP_DEBUG, "App constructor. m_sndCore = %", m_state.m_soundCore.get());


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
        NMsc::CLogger::Log(NMsc::ELogType::ERROR, "SndCore not set");
        return false;
    }

    message.m_tone = (NSnd::ETones) (((int) message.m_tone) + m_octave * 12);

    return m_state.m_soundCore->ReciveMidiMsg(message);
}

/*----------------------------------------------------------------------*/
bool CNoiApp::IsRecording() {
    return m_state.m_soundCore->IsRecording();
}

/*----------------------------------------------------------------------*/
bool CNoiApp::RecordingStart() {
    return m_state.m_soundCore->TrackRecordingStart();
}

/*----------------------------------------------------------------------*/
bool CNoiApp::RecordingStop() {
    return m_state.m_soundCore->TrackRecordingStop();
}

/*----------------------------------------------------------------------*/
bool CNoiApp::PlaybackStart() {
    return m_state.m_soundCore->TrackPlaybackStart();
}

/*----------------------------------------------------------------------*/
bool CNoiApp::PlaybackStop() {
    return m_state.m_soundCore->TrackPlaybackStop();
}

/*----------------------------------------------------------------------*/
bool CNoiApp::PlaybackSetPosition(uint32_t position) {
    if (IsRecording())
        RecordingStop();

    m_state.m_soundCore->TrackSetPosition(position);
}

/*----------------------------------------------------------------------*/
bool CNoiApp::PlaybaclSetPositionBeats(uint32_t beat) {
    PlaybackSetPosition(NSnd::SAMPLE_RATE * 60 / m_state.m_bpm * beat);
}

/*----------------------------------------------------------------------*/
uint32_t CNoiApp::PlaybackGetPosition() {
    return m_state.m_soundCore->TrackGetPosition();
}

/*----------------------------------------------------------------------*/
uint32_t CNoiApp::PlaybackGetPositionBeats() {
    return m_state.m_soundCore->TrackGetPosition() / 60 * m_state.m_bpm / NSnd::SAMPLE_RATE;
}

/*----------------------------------------------------------------------*/
bool CNoiApp::IsPlaying() {
    return m_state.m_soundCore->IsPlaying();
}

/*----------------------------------------------------------------------*/
bool CNoiApp::BpmSet(uint32_t bpm) {
    m_state.m_bpm = bpm;
    m_state.m_soundCore->BpmSet(bpm);
    return true;
}

/*----------------------------------------------------------------------*/
uint32_t CNoiApp::BpmGet() {
    return m_state.m_bpm;
}

/*----------------------------------------------------------------------*/
bool CNoiApp::RecordingUndo() {
    if (!m_state.m_activeTrack || !RecordingGetCanUndo())
        return false;

    m_state.m_activeTrack->UndoRecording();

    return true;
    //return m_state.m_soundCore->TrackRecordingUndo();
}

/*----------------------------------------------------------------------*/
bool CNoiApp::RecordingGetCanUndo() {
    if (!m_state.m_activeTrack)
        return false;

    return m_state.m_activeTrack->CanUndo();
}

/*----------------------------------------------------------------------*/
bool CNoiApp::SetOctave(uint32_t octave) {
    m_octave = octave;
    return true;
}

/*----------------------------------------------------------------------*/
bool CNoiApp::MetronomeSet(bool enable) {
    m_state.m_soundCore->SetMetronome(enable);
}

/*----------------------------------------------------------------------*/
NSnd::ATrack CNoiApp::TrackCreate() {

    NSnd::ATrack track = m_state.m_soundCore->TrackCreate();
    if (track)
        m_state.m_tracks.push_back(track);

    return track;
}

/*----------------------------------------------------------------------*/
NSnd::ATrack CNoiApp::TrackActiveGet() {
    return m_state.m_activeTrack;
}

/*----------------------------------------------------------------------*/
bool CNoiApp::TrackActiveSet(NSnd::ATrack track) {
    if (m_state.m_soundCore->TrackSetActive(track)) {
        m_state.m_activeTrack = track;
        return true;
    }
    return false;
}

/*----------------------------------------------------------------------*/
bool CNoiApp::ChainActiveSet(NSnd::AChain chain) {
    if (m_state.m_soundCore->ChainSelect(chain)) {
        m_state.m_activeChain = chain;
        return true;
    }

    return false;
}


/*----------------------------------------------------------------------*/
std::vector<NSnd::ATrack> CNoiApp::TracksGet() {
    return m_state.m_tracks;
}

/*----------------------------------------------------------------------*/
NSnd::AChain CNoiApp::ChainActiveGet() {
    return m_state.m_activeChain;
}

/*----------------------------------------------------------------------*/
std::vector<NSnd::AChain> CNoiApp::ChainsGet() {
    return m_state.m_chains;
}

/*----------------------------------------------------------------------*/
bool CNoiApp::SaveProject(std::string name) {
    std::ofstream project(name + ".noiProj");
    std::ofstream data(name + ".noiDat");

    // serialize track data
    NSnd::CTrackSlice::SerializeAllUsedSlices(data);

    NMsc::ASerializationNode rootNode = NMsc::CSerializationNode::GetNewTopNode();

    m_state.Serialize(rootNode);

    project << rootNode->Dump();

    project.close();
    data.close();

}

/*----------------------------------------------------------------------*/
bool CNoiApp::LoadProject(std::string name) {
    std::ifstream project(name + ".noiProj");
    std::ifstream data(name + ".noiDat");

    // Deserialize track data
    NSnd::CTrackSlice::DeserializeSlices(data);

    // Load project

    NMsc::ASerializationNode projNode = NMsc::CSerializationNode::Deserialize(project);
    m_state = CAppState(projNode);

    project.close();
    data.close();
}



