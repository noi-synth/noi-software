//
// Created by ddl_blue on 2.8.18.
//

#include "../../../include/ui/term/WinAudioDevicePick.hpp"
#include "../../../include/gfx/CNcurses.hpp"

using namespace NUi;
using namespace NUi::NTerm;
using Color = NGfx::CNcurses::ColorPair;

/*----------------------------------------------------------------------*/
WinAudioDevicePick::WinAudioDevicePick(NUi::WWindowManager manager) : CWindow(manager) {}

/*----------------------------------------------------------------------*/
ControlInput WinAudioDevicePick::ProcessInput(ControlInput control, ControlInputType type) {

    AWindowManager wm = m_manager.lock();

    if (m_isActive)
        switch (control) {
            case ControlInput::BTN_SHUTDOWN:
                wm->CloseTopWindowCallback();
                return ControlInput::NONE;

            default:
                return ControlInput::NONE;
        }

}

/*----------------------------------------------------------------------*/
void WinAudioDevicePick::Draw() {
    NGfx::CNcurses *gfx = NGfx::CNcurses::GetInstance();

    gfx->SetFrame(30, 20);
    gfx->DrawEmptyWindow(Color::WHITE_BLUE);
    gfx->DrawTextCentered(2, "Pick sound device:", Color::WHITE_BLACK);
    gfx->Update();
}

/*----------------------------------------------------------------------*/

