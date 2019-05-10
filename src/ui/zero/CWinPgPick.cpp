//
// Created by ddl_blue on 9.5.19.
//

#include "../../../include/ui/zero/CWinPgPick.hpp"
#include "../../../include/ui/zero/CNoiZeroCommunicator.hpp"
#include "../../../include/ui/zero/CWinPgPlayback.hpp"

using namespace NUi::NZero;

/*----------------------------------------------------------------------*/
CWinPgPick::CWinPgPick(NUi::WWindowManager windowManager) : CWindow(windowManager) {
    AWindowManager manager = windowManager.lock();
    // m_app = manager->GetApp();
}

/*----------------------------------------------------------------------*/
NUi::CInptutEventInfo CWinPgPick::ProcessInput(NUi::CInptutEventInfo input) {


    AWindowManager manager = m_manager.lock();
    if (!manager) {
        NMsc::CLogger::Log(NMsc::ELogType::ERROR, "CWinPgPick: Manager not found.");
        return input;
    }


    // Close this window
    if (input.m_input == EControlInput::BTN_PAGE && input.m_type == EControlInputType::RELEASE) {
        manager->CloseTopWindowCallback();
        return CInptutEventInfo();
    }

    if (!input.IsFnKey())
        return input;

    if (input.m_type != EControlInputType::PRESS)
        return CInptutEventInfo();

    uint32_t pgNumber = NMsc::Functions::EnumSub(input.m_input, EControlInput::BTN_FN_0);

    switch (pgNumber) {
        case 0:
            manager->OpenSingleWindowCallback(std::make_shared<CWinPgPlayback>(m_manager));
            break;

        default:
            NMsc::CLogger::Log(NMsc::ELogType::WARNING, "CWinPgPick: Page number % not implemented", pgNumber);
            manager->CloseTopWindowCallback();
    }

    // Open picker on top
    manager->OpenWindowCallback(std::make_shared<CWinPgPick>(m_manager));

}

/*----------------------------------------------------------------------*/
void CWinPgPick::Draw() {


    CNoiZeroCommunicator *g = CNoiZeroCommunicator::GetInstance();
    if (!g) {
        NMsc::CLogger::Log(NMsc::ELogType::ERROR, "CWinPgPick: No CNoiZeroCommunicator found.");
        return;
    }

    g->SetFnLed(0, ELedState::ON, NHw::ELedColor::WHITE);
    g->SetFnLed(1, ELedState::ON, NHw::ELedColor::BLUE);
    g->SetFnLed(2, ELedState::ON, NHw::ELedColor::CYAN);
    g->SetFnLed(3, ELedState::ON, NHw::ELedColor::GREEN);
    g->SetFnLed(4, ELedState::ON, NHw::ELedColor::MAGENTA);
    g->SetFnLed(5, ELedState::ON, NHw::ELedColor::YELLOW);
    g->SetFnLed(6, ELedState::ON, NHw::ELedColor::RED);
    g->SetFnLed(7, ELedState::ON, NHw::ELedColor::BLACK);
}

/*----------------------------------------------------------------------*/
