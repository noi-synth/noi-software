//
// Created by ddl_blue on 24.8.18.
//

#include "../../include/msc/CMaintainer.hpp"
#include "../../include/msc/CLogger.hpp"
#include <chrono>
#include <thread>

using namespace NMsc;

CMaintainer *CMaintainer::m_instance;

/*----------------------------------------------------------------------*/
CMaintainer::CMaintainer() : m_shouldStop(false), m_uniqueIdCounter(1) {
    if (m_instance)
        delete m_instance;
    m_instance = this;
}

/*----------------------------------------------------------------------*/
CMaintainer::~CMaintainer() {
    Stop();
    m_instance = nullptr;
}

/*----------------------------------------------------------------------*/
CMaintainer &CMaintainer::GetInstance() {
    if (m_instance)
        return *m_instance;

    return *(m_instance = new CMaintainer());
}

/*----------------------------------------------------------------------*/
void CMaintainer::Stop() {
    if (m_workerThread.joinable()) {
        m_shouldStop = true;
        m_workerThread.join();
    }
}

/*----------------------------------------------------------------------*/
void CMaintainer::Run() {
    if (!m_workerThread.joinable()) {
        m_shouldStop = false;
        m_workerThread = std::thread([&]() { ThreadWorker(); });
    }
}

/*----------------------------------------------------------------------*/
int CMaintainer::RegisterTask(const std::function<void()> &task) {
    std::lock_guard<std::mutex> guard(m_lock);

    m_tasks[++m_uniqueIdCounter] = task;
    Run();
    return m_uniqueIdCounter;
}

/*----------------------------------------------------------------------*/
void CMaintainer::ClearTaskList() {
    std::lock_guard<std::mutex> guard(m_lock);
    m_tasks.clear();
}

/*----------------------------------------------------------------------*/
void CMaintainer::UnregisterTask(int taskId) {
    std::lock_guard<std::mutex> guard(m_lock);
    if (m_tasks.find(taskId) != m_tasks.end())
        m_tasks.erase(taskId);
}

/*----------------------------------------------------------------------*/
void CMaintainer::ThreadWorker() {
    while (!m_shouldStop) {

        // Locked part
        {
            std::lock_guard<std::mutex> guard(m_lock);
            for (const auto &task : m_tasks) {
                task.second();
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}