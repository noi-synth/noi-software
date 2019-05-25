//
// Created by ddl_blue on 24.5.19.
//

#include "../../../include/ui/zero/CWinFileDialog.hpp"
#include "../../../include/ui/zero/CWinOkDialog.hpp"
#include "../../../include/hw/HwEnums.hpp"
#include "../../../include/ui/zero/CNoiZeroCommunicator.hpp"

using namespace NUi::NZero;

/*----------------------------------------------------------------------*/
CWinFileDialog::CWinFileDialog(NUi::WWindowManager windowManager, bool saveDialog) : CWindow(windowManager),
                                                                                     m_saveDialog(saveDialog),
                                                                                     m_confirmationDialogID(0),
                                                                                     m_fileId(0) {
    AWindowManager manager = windowManager.lock();
    m_app = manager->GetApp();
}

/*----------------------------------------------------------------------*/
NUi::CInptutEventInfo CWinFileDialog::ProcessInput(NUi::CInptutEventInfo input) {
    if (input.IsKey()) {

        if (!input.JustPressed())
            return CInptutEventInfo();

        if (input.IsFnKey()) {
            m_fileId = NMsc::Functions::EnumSub(input.m_input, EControlInput::BTN_FN_0);
            DoWithManager([&](AWindowManager manager) {

                // Open dialog window
                if (m_saveDialog || NMsc::Functions::FileExtists(FILE_FOLDER + std::to_string(m_fileId)))
                    manager->OpenWindowCallback(std::make_shared<CWinOkDialog>(m_manager,
                                                                               m_confirmationDialogID = manager->RequestReturnValue()));
            });

        } else {
            DoWithManager([&](AWindowManager manager) {
                manager->CloseTopWindowCallback();
            });
            return CInptutEventInfo();
        }
    }

    return CInptutEventInfo();
}

/*----------------------------------------------------------------------*/
void CWinFileDialog::Draw() {

    CNoiZeroCommunicator *g = CNoiZeroCommunicator::GetInstance();
    if (!g) {
        NMsc::CLogger::Log(NMsc::ELogType::ERROR, "CWinPgEffects: No CNoiZeroCommunicator found.");
        return;
    }

    // Save and load dialogs has different colors
    NHw::ELedColor emptyCol = m_saveDialog ? NHw::ELedColor::RED : NHw::ELedColor::BLUE;
    NHw::ELedColor fullCol = m_saveDialog ? NHw::ELedColor::BLUE : NHw::ELedColor::CYAN;

    for (uint32_t i = 0; i < 8; ++i) {
        // Existing and nonexisting file slots has different colors
        NHw::ELedColor col = NMsc::Functions::FileExtists(FILE_FOLDER + std::to_string(i)) ? fullCol : emptyCol;
        g->SetFnLed(i, ELedState::ON, col);
    }
}

/*----------------------------------------------------------------------*/
void CWinFileDialog::Update() {
    if (m_confirmationDialogID) {
        NMsc::ASerializationNode ret;
        DoWithManager([&](AWindowManager manager) {
            ret = manager->GetReturnValue(m_confirmationDialogID);
        });

        // Save or load the file
        if (ret) {
            m_confirmationDialogID = 0;
            if (ret->GetBool("ok")) {
                if (m_saveDialog)
                    m_app->SaveProject(FILE_FOLDER + std::to_string(m_fileId));
                else if (NMsc::Functions::FileExtists(FILE_FOLDER + std::to_string(m_fileId)))
                    m_app->LoadProject(FILE_FOLDER + std::to_string(m_fileId));

                DoWithManager([&](AWindowManager manager) {
                    manager->CloseTopWindowCallback();
                });
            }
        }

    }
}