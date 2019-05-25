//
// Created by ddl_blue on 23.5.19.
//

#include "../../../include/ui/zero/CWinSystemMenu.hpp"
#include "../../../include/ui/zero/CNoiZeroCommunicator.hpp"
#include "../../../include/ui/zero/CWinOkDialog.hpp"
#include "../../../include/ui/zero/CWinFileDialog.hpp"

using namespace NUi::NZero;

/*----------------------------------------------------------------------*/
CWinSystemMenu::CWinSystemMenu(NUi::WWindowManager windowManager) : CWindow(windowManager), m_clearProjectDialog(0),
                                                                    m_shutdownDialog(0) {
    AWindowManager manager = windowManager.lock();
    m_app = manager->GetApp();
}

/*----------------------------------------------------------------------*/
NUi::CInptutEventInfo CWinSystemMenu::ProcessInput(NUi::CInptutEventInfo input) {
    if (!input.IsFnKey())
        return input;

    if (input.m_type != EControlInputType::PRESS)
        return NUi::CInptutEventInfo();

    switch (input.m_input) {
        case EControlInput::BTN_FN_0:
            DoWithManager([&](AWindowManager manager) {
                m_clearProjectDialog = manager->RequestReturnValue();
                manager->OpenWindowCallback(std::make_shared<CWinOkDialog>(m_manager, m_clearProjectDialog));
            });
            break;

        case EControlInput::BTN_FN_2:
            DoWithManager([&](AWindowManager manager) {
                manager->OpenWindowCallback(std::make_shared<CWinFileDialog>(m_manager, true));
            });
            break;

        case EControlInput::BTN_FN_3:
            DoWithManager([&](AWindowManager manager) {
                manager->OpenWindowCallback(std::make_shared<CWinFileDialog>(m_manager, false));
            });
            break;

        case EControlInput::BTN_FN_7:
            DoWithManager([&](AWindowManager manager) {
                m_shutdownDialog = manager->RequestReturnValue();
                manager->OpenWindowCallback(std::make_shared<CWinOkDialog>(m_manager, m_shutdownDialog));
            });
            break;


        default:
            break;
    }

    return CInptutEventInfo();


}

/*----------------------------------------------------------------------*/
void CWinSystemMenu::Update() {
    CNoiZeroCommunicator *g = CNoiZeroCommunicator::GetInstance();
    if (!g) {
        NMsc::CLogger::Log(NMsc::ELogType::ERROR, "CWinPgEffects: No CNoiZeroCommunicator found.");
        return;
    }

    // Page LED
    g->SetStatusLed(EStatusLed::PAGE, ELedState::OFF, NHw::ELedColor::BLACK);

    if (m_clearProjectDialog) {
        DoWithManager([&](AWindowManager manager) {
            NMsc::ASerializationNode ret = manager->GetReturnValue(m_clearProjectDialog);
            if (ret) {
                if (ret->GetBool("ok"))
                    m_app->ClearProject();
            }
        });
    }

    if (m_shutdownDialog) {
        DoWithManager([&](AWindowManager manager) {
            NMsc::ASerializationNode ret = manager->GetReturnValue(m_shutdownDialog);
            if (ret) {
                if (ret->GetBool("ok")) {
                    system("./poweroff.sh");
                    manager->m_exiting = true; //todo real poweroff
                }
            }
        });
    }

}

/*----------------------------------------------------------------------*/
void CWinSystemMenu::Draw() {
    CNoiZeroCommunicator *g = CNoiZeroCommunicator::GetInstance();
    if (!g) {
        NMsc::CLogger::Log(NMsc::ELogType::ERROR, "CWinSystemMenu: No CNoiZeroCommunicator found.");
        return;
    }

    g->SetStatusLed(EStatusLed::PAGE, ELedState::OFF, NHw::ELedColor::BLUE);

    g->ClearFnLeds();

    g->SetFnLed(0, ELedState::ON, NHw::ELedColor::GREEN);

    g->SetFnLed(2, ELedState::ON, NHw::ELedColor::RED);
    g->SetFnLed(3, ELedState::ON, NHw::ELedColor::BLUE);

    g->SetFnLed(7, ELedState::ON, NHw::ELedColor::WHITE);
}