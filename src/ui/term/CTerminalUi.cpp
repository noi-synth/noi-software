//
// Created by ddl_blue on 31.7.18.
//

#include <time.h>

#include "../../../include/ui/term/CTerminalUi.hpp"
#include "../../../include/ui/term/WinMain.hpp"
#include "../../../include/gfx/CNcurses.hpp"
#include "../../../include/ui/term/CInputResolver.hpp"
#include "../../../include/msc/CLogger.hpp"

using namespace NUi::NTerm;

/*----------------------------------------------------------------------*/
CTerminalUi::CTerminalUi(NLgc::ANoiApp app) {
    NMsc::CLogger::Log(NMsc::ELogType::TMP_DEBUG, "CTerminalUI constructor. app=%", app.get());
    m_windowManager = std::make_shared<CWindowManager>(NUi::EDrawingPolicy::DRAW_ALL, app);
    m_toneState.insert(m_toneState.begin(), EControlInput::_NOTE_LAST - EControlInput::_NOTE_FIRST, true);
}

/*----------------------------------------------------------------------*/
void CTerminalUi::UiThreadWorker() {

    m_windowManager->SetMainWindowCallback(std::make_shared<WinMain>(m_windowManager->shared_from_this()));

    NGfx::CNcurses *ncurses = NGfx::CNcurses::GetInstance();

    m_windowManager->RequestRedraw();

    while (!m_shouldStop && !m_windowManager->m_exiting) {

        // Get and process input
        int rawInput = ncurses->GetInput();
        EControlInput input = CInputResolver::ResolveInput(rawInput);

        // Notes are in toggle mode
        EControlInputType inputType = EControlInputType::PRESS;
        if (input > EControlInput::_NOTE_FIRST && input < EControlInput::_NOTE_LAST)
            if (m_toneState[input - EControlInput::_NOTE_FIRST] = !m_toneState[input - EControlInput::_NOTE_FIRST])
                inputType = EControlInputType::RELEASE;

        if (input != EControlInput::NONE)
            m_windowManager->ProcessControlInput(input, inputType);

        m_windowManager->Update();

        NGfx::CNcurses *gfx = NGfx::CNcurses::GetInstance();
        gfx->Update();

        // Wait...
        timespec waitingTime;
        waitingTime.tv_sec = 0;
        waitingTime.tv_nsec = 50000000;
        nanosleep(&waitingTime, nullptr);
    }

    NGfx::CNcurses::GetInstance()->FreeInstance();

}