//
// Created by ddl_blue on 2.8.18.
//

#include "../../../include/ui/term/WinMain.hpp"
#include "../../../include/gfx/CNcurses.hpp"
#include "../../../include/ui/term/WinAudioDevicePick.hpp"

using namespace NUi;
using namespace NUi::NTerm;
using Color = NGfx::CNcurses::ColorPair;

/*----------------------------------------------------------------------*/
WinMain::WinMain(NUi::WWindowManager windowManager) : CWindow(windowManager) {}

/*----------------------------------------------------------------------*/
void WinMain::Draw() {
    NGfx::CNcurses *gfx = NGfx::CNcurses::GetInstance();

    //gfx->SetFrame(gfx->GetScreenWidth(), gfx->GetScreenHeight());
    gfx->SetFrame(30, 30);

    gfx->ClearScreen();

    gfx->DrawTextCentered(1, "NOI synth", Color::WHITE_BLACK);


    gfx->Update();
}

/*----------------------------------------------------------------------*/
NUi::ControlInput WinMain::ProcessInput(NUi::ControlInput control, NUi::ControlInputType type) {
    if (type != NUi::ControlInputType::PRESS)
        return control;

    AWindowManager wm;

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