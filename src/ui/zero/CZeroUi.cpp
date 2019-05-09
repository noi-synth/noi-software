//
// Created by ddl_blue on 1.5.19.
//

#include <time.h>

#include "../../../include/ui/zero/CZeroUi.hpp"
#include "../../../include/ui/zero/CWinMain.hpp"
#include "../../../include/ui/zero/CNoiZeroCommunicator.hpp"

using namespace NUi::NZero;

/*----------------------------------------------------------------------*/
CZeroUi::CZeroUi(NLgc::ANoiApp app) {

    NMsc::CLogger::Log(NMsc::ELogType::TMP_DEBUG, "CTerminalUI constructor. app=%", app.get());
    m_windowManager = std::make_shared<CWindowManager>(NUi::EDrawingPolicy::DRAW_ALL, app);

}

/*----------------------------------------------------------------------*/
void CZeroUi::UiThreadWorker() {
    m_windowManager->SetMainWindowCallback(std::make_shared<CWinMain>(m_windowManager->shared_from_this()));
    NMsc::CLogger::Log(NMsc::ELogType::TMP_DEBUG, "CZeroUi running.");

    while (!m_shouldStop && !m_windowManager->m_exiting) {
        m_windowManager->Update();


        CNoiZeroCommunicator *g;
        g = CNoiZeroCommunicator::GetInstance();
        if (g) {
            g->Update();
        } else {
            NMsc::CLogger::Log(NMsc::ELogType::ERROR, "CZeroUi: No CNoiZeroCommunicator.");
        }

        // Wait...
        timespec waitingTime;
        waitingTime.tv_sec = 0;
        waitingTime.tv_nsec = 50000000;
        nanosleep(&waitingTime, nullptr);
    }
}

/*----------------------------------------------------------------------*/
void CZeroUi::ProcessInput(NUi::CInptutEventInfo input) {
    m_windowManager->ProcessControlInput(input.m_input, input.m_type);
}