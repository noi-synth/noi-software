//
// Created by ddl_blue on 31.7.18.
//

#include <time.h>

#include "../../../include/ui/term/CTerminalUi.hpp"
#include "../../../include/ui/term/WinMain.hpp"
#include "../../../include/gfx/CNcurses.hpp"
#include "../../../include/ui/term/CInputResolver.hpp"

using namespace NUi::NTerm;

/*----------------------------------------------------------------------*/
CTerminalUi::CTerminalUi(NLgc::ANoiApp app) {
    m_windowManager = std::make_shared<CWindowManager>(NUi::DrawingPolicy::DRAW_ALL, app);
}

/*----------------------------------------------------------------------*/
void CTerminalUi::UiThreadWorker() {

    m_windowManager->SetMainWindowCallback(std::make_shared<WinMain>(m_windowManager->shared_from_this()));

    NGfx::CNcurses *ncurses = NGfx::CNcurses::GetInstance();

    m_windowManager->RequestRedraw();

    while (!m_shouldStop && !m_windowManager->m_exiting) {

        // Get and process input
        int rawInput = ncurses->GetInput();
        ControlInput input = CInputResolver::ResolveInput(rawInput);
        if (input != ControlInput::NONE)
            m_windowManager->ProcessControlInput(input, ControlInputType::PRESS);

        m_windowManager->Update();

        // Wait...
        timespec waitingTime;
        waitingTime.tv_sec = 0;
        waitingTime.tv_nsec = 33333333;
        nanosleep(&waitingTime, nullptr);
    }

    NGfx::CNcurses::GetInstance()->FreeInstance();

}