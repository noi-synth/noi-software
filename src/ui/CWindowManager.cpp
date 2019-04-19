//
// Created by ddl_blue on 7.7.18.
//

#include "../../include/ui/CWindowManager.hpp"
#include "../../include/msc/CLogger.hpp"

using namespace NUi;

/*----------------------------------------------------------------------*/
CWindowManager::CWindowManager(NUi::EDrawingPolicy drawingPolicy, NLgc::ANoiApp app) : m_drawingPolicy(drawingPolicy),
                                                                                       m_exiting(false),
                                                                                       m_app(app),
                                                                                       m_redrawRequested(true),
                                                                                       m_breakWindowIteration(false) {
    NMsc::CLogger::Log(NMsc::ELogType::TMP_DEBUG, "CWindowManager constructor, app  = %, m_app = %", app.get(),
                       m_app.get());
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
        case EDrawingPolicy::DRAW_ALL:
            if (m_mainWindow)
                m_mainWindow->Draw();
            for (auto it = m_windows.rbegin(); it != m_windows.rend(); --it)
                (*it)->Draw();
            break;

        case EDrawingPolicy::DRAW_TOP:
            if (!m_windows.empty())
                m_windows.front()->Draw();
            else if (m_mainWindow)
                m_mainWindow->Draw();
            break;

        case EDrawingPolicy::DRAW_NONE:
            break;
    }
}

/*----------------------------------------------------------------------*/
void CWindowManager::ProcessControlInput(NUi::EControlInput control, NUi::EControlInputType type) {
    auto it = m_windows.begin();
    m_breakWindowIteration = false;

    while (control != EControlInput::NONE && it != m_windows.end() && !m_breakWindowIteration)
        control = (*it)->ProcessInput(control, type);

    if (control != EControlInput::NONE && m_mainWindow && !m_breakWindowIteration)
        m_mainWindow->ProcessInput(control, type);
}

/*----------------------------------------------------------------------*/
NLgc::ANoiApp CWindowManager::GetApp() {
    //NMsc::CLogger::Log(NMsc::ELogType::TMP_DEBUG, "CWindowManager::GetApp = %, this=%", m_app.get(), this);
    return m_app;
}

/*----------------------------------------------------------------------*/
void CWindowManager::RequestRedraw() {
    m_redrawRequested = true;
}

/*----------------------------------------------------------------------*/
void CWindowManager::Update() {
    if (m_mainWindow)
        m_mainWindow->Update();
    for (auto &&window : m_windows) {
        window->Update();
    }
    if (m_redrawRequested) {
        Redraw();
        m_redrawRequested = false;
    }
}