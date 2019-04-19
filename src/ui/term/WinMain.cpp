//37:1
// Created by ddl_blue on 2.8.18.
//

#include "../../../include/ui/term/WinMain.hpp"
#include "../../../include/gfx/CNcurses.hpp"
#include "../../../include/ui/term/WinAudioDevicePick.hpp"
#include "../../../include/msc/CLogger.hpp"
#include "../../../include/msc/CDebugInfo.hpp"
#include "../../../include/ui/term/WinPickFromList.hpp"

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

//    gfx->DrawTextFormated(45, 5, Color::WHITE_BLACK,
//                          "DBG\n===\nLastOutput:\t%\nLastChainOutput:\t%\nLastTrackManagerOutput:\t%\nTrackSize:\t%\nPositionDisplacement:\t%",
//                          NMsc::CDebugInfo::m_sndLastOutput,
//                          NMsc::CDebugInfo::m_sndLastChainOutput,
//                          NMsc::CDebugInfo::m_sndLastTrackManagerOutput,
//                          NMsc::CDebugInfo::m_sndTrackSize,
//                          NMsc::CDebugInfo::m_sndPositionDisplacement
//    );

    //gfx->Update();
}

/*----------------------------------------------------------------------*/
NUi::EControlInput WinMain::ProcessInput(NUi::EControlInput control, NUi::EControlInputType type) {
    AWindowManager wm = m_manager.lock();
    if (!wm) {
        NMsc::CLogger::Log(NMsc::ELogType::ERROR, "WinMain: ProcessInput: Window manager could not be locked.");
        return EControlInput::NONE;
    }

    if (control > EControlInput::_NOTE_FIRST && control < EControlInput::_NOTE_LAST) {
        m_app->SendMidiMessage(NSnd::CMidiMsg(
                type == EControlInputType::PRESS ? NSnd::EMidiMsgType::NOTE_ON : NSnd::EMidiMsgType::NOTE_OFF,
                (NSnd::ETones) (NSnd::ETones::C3 + (control - EControlInput::_NOTE_FIRST)),
                255));
        return EControlInput::NONE;
    }

    if (type != NUi::EControlInputType::PRESS)
        return control;

    WinPickFromList::items ActionList;
    switch (control) {
        case EControlInput::BTN_SHUTDOWN:

            if (wm)
                wm->m_exiting = true;
            return EControlInput::NONE;

        case EControlInput::BTN_PLAY:
            if (m_app->IsPlaying()) {
                m_app->PlaybackStop();
                m_app->PlaybackSetPosition(0);
            } else
                m_app->PlaybackStart();
            return EControlInput::NONE;

        case EControlInput::BTN_REC:
            if (m_app->IsRecording())
                m_app->RecordingStop();
            else
                m_app->RecordingStart();
            return EControlInput::NONE;

        case EControlInput::BTN_NC_MENU:
            ActionList.clear();
            ActionList.push_back(std::make_pair("Undo recording", [=]() {
                m_app->RecordingUndo();
                wm->CloseTopWindowCallback();
            }));
            ActionList.push_back(
                    std::make_pair("test2", [&]() { NMsc::CLogger::Log(NMsc::ELogType::TMP_DEBUG, "test2"); }));
            ActionList.push_back(
                    std::make_pair("test3", [&]() { NMsc::CLogger::Log(NMsc::ELogType::TMP_DEBUG, "test3"); }));
            wm->OpenWindowCallback(std::make_shared<WinPickFromList>(wm, "Actions", ActionList, true));

            return EControlInput::NONE;

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
