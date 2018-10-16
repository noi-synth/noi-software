//37:1
// Created by ddl_blue on 2.8.18.
//

#include "../../../include/ui/term/WinMain.hpp"
#include "../../../include/gfx/CNcurses.hpp"
#include "../../../include/ui/term/WinAudioDevicePick.hpp"
#include "../../../include/msc/CLogger.hpp"
#include "../../../include/msc/CDebugInfo.hpp"

using namespace NUi;
using namespace NUi::NTerm;
using Color = NGfx::CNcurses::ColorPair;

/*----------------------------------------------------------------------*/
WinMain::WinMain(NUi::WWindowManager windowManager) : CWindow(windowManager), m_frameBufferLength(-1) {
    AWindowManager manager = windowManager.lock();
    m_app = manager->GetApp();
}

/*----------------------------------------------------------------------*/
void WinMain::Draw() {
    NGfx::CNcurses *gfx = NGfx::CNcurses::GetInstance();

    //char buff[64];

    gfx->SetFrame(gfx->GetScreenWidth(), gfx->GetScreenHeight());

    gfx->ClearScreen();

    gfx->DrawTextCentered(1, "NOI synth", Color::WHITE_BLACK);

    /*snprintf(buff, 64, "Last audio buff len: %d", m_frameBufferLength);
    gfx->DrawText(1, 5, buff, Color::WHITE_BLACK);*/
    int audioBufferLen = NMsc::CDebugInfo::m_lastAudioBufferLen;
    int playbackPosition = m_app->PlaybackGetPosition();

    gfx->DrawTextFormated(1, 5, Color::WHITE_BLACK,
                          "STATE\n=====\nCallback bufflen:\t%\nPlaying:\t%\nRecording:\t%\nPosition: %:%.% \t(%)",
                          audioBufferLen,
                          m_app->IsPlaying() ? "Yes" : "No",
                          m_app->IsRecording() ? "Yes" : "No",
                          (playbackPosition / NSnd::SAMPLE_RATE) / 60, // min
                          (playbackPosition / NSnd::SAMPLE_RATE) % 60,  // sec
                          (playbackPosition % NSnd::SAMPLE_RATE),
                          playbackPosition
    );

    gfx->DrawTextFormated(45, 5, Color::WHITE_BLACK,
                          "DBG\n===\nLastOutput:\t%\nLastChainOutput:\t%\nLastTrackOutput:\t%\nLastTrackManagerOutput:\t%\nTrackWorkflow:\t%\nTrackSize:\t%\nPositionDisplacement:\t%",
                          NMsc::CDebugInfo::m_sndLastOutput,
                          NMsc::CDebugInfo::m_sndLastChainOutput,
                          NMsc::CDebugInfo::m_sndLastTrackOutput,
                          NMsc::CDebugInfo::m_sndLastTrackManagerOutput,
                          NMsc::CDebugInfo::m_sndLastTrackCall,
                          NMsc::CDebugInfo::m_sndTrackSize,
                          NMsc::CDebugInfo::m_sndPositionDisplacement
    );

    //gfx->Update();
}

/*----------------------------------------------------------------------*/
NUi::ControlInput WinMain::ProcessInput(NUi::ControlInput control, NUi::ControlInputType type) {
    AWindowManager wm;

    if (control > ControlInput::_NOTE_FIRST && control < ControlInput::_NOTE_LAST) {
        m_app->SendMidiMessage(NSnd::CMidiMsg(
                type == ControlInputType::PRESS ? NSnd::EMidiMsgType::NOTE_ON : NSnd::EMidiMsgType::NOTE_OFF,
                (NSnd::ETones) (NSnd::ETones::C3 + (control - ControlInput::_NOTE_FIRST)),
                255));
        return ControlInput::NONE;
    }

    if (type != NUi::ControlInputType::PRESS)
        return control;

    switch (control) {
        case ControlInput::BTN_SHUTDOWN:
            wm = m_manager.lock();
            if (wm)
                wm->m_exiting = true;
            return ControlInput::NONE;

        case ControlInput::BTN_PLAY:
            if (m_app->IsPlaying()) {
                m_app->PlaybackStop();
                m_app->PlaybackSetPosition(0);
            } else
                m_app->PlaybackStart();
            return ControlInput::NONE;

        case ControlInput::BTN_REC:
            if (m_app->IsRecording())
                m_app->RecordingStop();
            else
                m_app->RecordingStart();
            return ControlInput::NONE;

        default:
            return control;
    }
}

/*----------------------------------------------------------------------*/
void WinMain::Init() {
    AWindowManager manager = m_manager.lock();
    if (manager) {
        manager->OpenWindowCallback(std::make_shared<WinAudioDevicePick>(manager->shared_from_this()));
    }
}

/*----------------------------------------------------------------------*/
void WinMain::Update() {
//    int audioBufferLen = NMsc::CDebugInfo::m_lastAudioBufferLen;
//    if (audioBufferLen != m_frameBufferLength) {
//        m_frameBufferLength = audioBufferLen;
//        RequestRedraw();
//    }
    RequestRedraw(); // todo do not update all the time
}
