//
// Created by ddl_blue on 7.7.18.
//

#include "../../include/ui/CWindowManager.hpp"

using namespace NUi;

/*----------------------------------------------------------------------*/
CWindowManager::CWindowManager(NUi::DrawingPolicy drawingPolicy, NLgc::ANoiApp app) : m_drawingPolicy(drawingPolicy),
                                                                                      m_exiting(false),
                                                                                      m_app(app),
                                                                                      m_redrawRequested(true),
                                                                                      m_breakWindowIteration(false) {
}

/*----------------------------------------------------------------------*/
void CWindowManager::SetMainWindowCallback(const NUi::AWindow &window) {
    if (window)
        window->Init();

    m_breakWindowIteration = true;
    m_mainWindow = window;
    if (m_windows.empty())
        m_mainWindow->Activate();
    RequestRedraw();
}

/*----------------------------------------------------------------------*/
void CWindowManager::ReplaceTopWindowCallback(NUi::AWindow window) {
    m_breakWindowIteration = true;

    window->Init();

    if (m_windows.empty()) {
        m_windows.push_front(window);
        if (m_mainWindow)
            m_mainWindow->Deactivate();
        window->Activate();
        return;
    }
    m_windows.front()->Deactivate();
    m_windows.front() = window;
    m_windows.front()->Activate();
    RequestRedraw();
}

/*----------------------------------------------------------------------*/
void CWindowManager::OpenWindowCallback(NUi::AWindow window) {

    window->Init();

    m_breakWindowIteration = true;
    if (!m_windows.empty())
        m_windows.front()->Deactivate();
    else if (m_mainWindow)
        m_mainWindow->Deactivate();

    m_windows.push_front(window);
    window->Activate();
    RequestRedraw();
}

/*----------------------------------------------------------------------*/
void CWindowManager::OpenSingleWindowCallback(NUi::AWindow window) {

    window->Init();

    m_breakWindowIteration = true;
    if (!m_windows.empty())
        m_windows.front()->Deactivate();
    else if (m_mainWindow)
        m_mainWindow->Deactivate();

    m_windows.clear();
    m_windows.push_front(window);
    window->Activate();
    RequestRedraw();
}

/*----------------------------------------------------------------------*/
void CWindowManager::CloseTopWindowCallback() {
    m_breakWindowIteration = true;
    if (!m_windows.empty()) {
        m_windows.front()->Deactivate();
        m_windows.pop_front();
        if (!m_windows.empty())
            m_windows.front()->Activate();
        else if (m_mainWindow)
            m_mainWindow->Activate();
    }
    RequestRedraw();
}

/*----------------------------------------------------------------------*/
void CWindowManager::Redraw() {
    switch (m_drawingPolicy) {
        case DrawingPolicy::DRAW_ALL:
            if (m_mainWindow)
                m_mainWindow->Draw();
            for (auto it = m_windows.rbegin(); it != m_windows.rend(); --it)
                (*it)->Draw();
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
    m_breakWindowIteration = false;

    while (control != ControlInput::NONE && it != m_windows.end() && !m_breakWindowIteration)
        control = (*it)->ProcessInput(control, type);

    if (control != ControlInput::NONE && m_mainWindow && !m_breakWindowIteration)
        m_mainWindow->ProcessInput(control, type);
}

/*----------------------------------------------------------------------*/
NLgc::ANoiApp CWindowManager::GetApp() {
    return m_app;
}

/*----------------------------------------------------------------------*/
void CWindowManager::RequestRedraw() {
    m_redrawRequested = true;
}

/*----------------------------------------------------------------------*/
void CWindowManager::Update() {
    for (auto &&window : m_windows) {
        window->Update();
    }
    if (m_redrawRequested) {
        Redraw();
        m_redrawRequested = false;
    }
}