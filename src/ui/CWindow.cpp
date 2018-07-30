//
// Created by ddl_blue on 7.7.18.
//

#include "../../include/ui/CWindow.hpp"

using namespace NUi;

/*----------------------------------------------------------------------*/
CWindow::CWindow(WWindowManager windowManager) : m_manager(windowManager), m_isActive(false) {
}

/*----------------------------------------------------------------------*/
void CWindow::Activate() {
    m_isActive = true;
}

/*----------------------------------------------------------------------*/
void CWindow::Deactivate() {
    m_isActive = false;
}

/*----------------------------------------------------------------------*/
void CWindow::Draw() {}

/*----------------------------------------------------------------------*/
ControlInput CWindow::ProcessInput(NUi::ControlInput control, NUi::ControlInputType type) {
    return control;
}