//
// Created by ddl_blue on 2.8.18.
//

#include "../../../include/ui/term/WinMain.hpp"
#include "../../../include/gfx/CNcurses.hpp"
#include "../../../include/ui/term/WinAudioDevicePick.hpp"
#include "../../../include/msc/CLogger.hpp"

using namespace NUi;
using namespace NUi::NTerm;
using Color = NGfx::CNcurses::ColorPair;

/*----------------------------------------------------------------------*/
WinMain::WinMain(NUi::WWindowManager windowManager) : CWindow(windowManager), m_lastFrameBufferLength(-1) {
    AWindowManager manager = windowManager.lock();
    m_app = manager->GetApp();
}

/*----------------------------------------------------------------------*/
void WinMain::Draw() {
    NGfx::CNcurses *gfx = NGfx::CNcurses::GetInstance();

    char buff[64];

    gfx->SetFrame(gfx->GetScreenWidth(), gfx->GetScreenHeight());

    gfx->ClearScreen();

    gfx->DrawTextCentered(1, "NOI synth", Color::WHITE_BLACK);

    snprintf(buff, 64, "Last audio buff len: %d", m_lastFrameBufferLength);
    gfx->DrawText(1, 5, buff, Color::WHITE_BLACK);

    gfx->Update();
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
    int audioBufferLen = m_app->GetAudioDeviceBufferLen();
    if (audioBufferLen != m_lastFrameBufferLength) {
        m_lastFrameBufferLength = audioBufferLen;
        RequestRedraw();
    }
}
