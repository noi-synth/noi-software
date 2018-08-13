//
// Created by ddl_blue on 31.7.18.
//

#include "../../include/ui/CUi.hpp"

using namespace NUi;

/*----------------------------------------------------------------------*/
CUi::CUi() : m_shouldStop(false) {}

/*----------------------------------------------------------------------*/
void CUi::Run() {
    if (!m_runningSignalizer.try_lock())
        return;


    if (m_uiThread.joinable()) {
        m_shouldStop = true;
        m_uiThread.join();
    }

    m_shouldStop = false;
    m_uiThread = std::thread([this]() {
        this->UiThreadWorker();
        m_runningSignalizer.unlock();
    });
}

/*----------------------------------------------------------------------*/
void CUi::Stop() {
    if (m_runningSignalizer.try_lock()) {
        m_runningSignalizer.unlock();
        return;
    }

    m_shouldStop = true;
    if (m_uiThread.joinable())
        m_uiThread.join();

    //m_runningSignalizer.unlock();
}

/*----------------------------------------------------------------------*/
void CUi::WaitForStop() {
    m_runningSignalizer.lock();
    if (m_uiThread.joinable())
        m_uiThread.join();
    m_runningSignalizer.unlock();
}

/*----------------------------------------------------------------------*/
bool CUi::IsRunning() {
    bool ret = !m_runningSignalizer.try_lock();

    if (!ret)
        m_runningSignalizer.unlock();
    return ret;
}
