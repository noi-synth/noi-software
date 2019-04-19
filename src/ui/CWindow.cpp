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
EControlInput CWindow::ProcessInput(NUi::EControlInput control, NUi::EControlInputType type) {
    return control;
}

/*----------------------------------------------------------------------*/
void CWindow::Draw() {}

/*----------------------------------------------------------------------*/
void CWindow::Update() {}

/*----------------------------------------------------------------------*/
void CWindow::Init() {}

/*----------------------------------------------------------------------*/
void CWindow::RequestRedraw() {
    AWindowManager manager = m_manager.lock();
    if (manager)
        manager->RequestRedraw();
}

/*----------------------------------------------------------------------*/
bool CWindow::DoWithManager(std::function<void(AWindowManager)> funct) {
    AWindowManager manager = m_manager.lock();

    if (manager) {
        funct(manager);
        return true;
    }
    return false;
}