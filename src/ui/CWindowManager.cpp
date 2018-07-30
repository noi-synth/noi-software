//
// Created by ddl_blue on 7.7.18.
//

#include "../../include/ui/CWindowManager.hpp"

using namespace NUi;

/*----------------------------------------------------------------------*/
CWindowManager::CWindowManager(NUi::DrawingPolicy drawingPolicy, AWindow mainWindow) : m_drawingPolicy(drawingPolicy),
                                                                                       m_mainWindow(mainWindow) {
}

/*----------------------------------------------------------------------*/
void CWindowManager::SetMainWindowCallback(NUi::AWindow &window) {
    m_mainWindow = window;
    if (m_windows.empty())
        m_mainWindow->Activate();
}

/*----------------------------------------------------------------------*/
void CWindowManager::ReplaceTopWindowCallback(NUi::AWindow window) {
    if (m_windows.empty()) {
        m_windows.push_front(window);
        window->Activate();
        return;
    }
    m_windows.front()->Deactivate();
    m_windows.front() = window;
    m_windows.front()->Activate();
}

/*----------------------------------------------------------------------*/
void CWindowManager::OpenWindowCallback(NUi::AWindow window) {
    if (!m_windows.empty())
        m_windows.front()->Deactivate();

    m_windows.push_front(window);
    window->Activate();
}

/*----------------------------------------------------------------------*/
void CWindowManager::OpenSingleWindowCallback(NUi::AWindow window) {
    if (!m_windows.empty())
        m_windows.front()->Deactivate();

    m_windows.clear();
    m_windows.push_front(window);
    window->Activate();
}

/*----------------------------------------------------------------------*/
void CWindowManager::CloseTopWindowCallback() {
    if (!m_windows.empty()) {
        m_windows.front()->Deactivate();
        m_windows.pop_front();
        if (!m_windows.empty())
            m_windows.front()->Activate();
    }
}

/*----------------------------------------------------------------------*/
void CWindowManager::Redraw() {
    switch (m_drawingPolicy) {
        case DrawingPolicy::DRAW_ALL:
            if (m_mainWindow)
                m_mainWindow->Draw();
            for (auto it = m_windows.rbegin(); it != m_windows.rend(); --it) {
                (*it)->Draw();
            }
            break;

        case DrawingPolicy::DRAW_TOP:
            if (!m_windows.empty())
                m_windows.front()->Draw();
            else if (m_mainWindow)
                m_mainWindow->Draw();
            break;

        case DrawingPolicy::DRAW_NONE:
            break;
    }
}

/*----------------------------------------------------------------------*/
void CWindowManager::ProcessControlInput(NUi::ControlInput control, NUi::ControlInputType type) {
    auto it = m_windows.begin();

    while (control != ControlInput::NONE && it != m_windows.end())
        control = (*it)->ProcessInput(control, type);

    if (control != ControlInput::NONE && m_mainWindow)
        m_mainWindow->ProcessInput(control, type);
}






