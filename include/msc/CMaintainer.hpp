//
// Created by ddl_blue on 24.8.18.
//

#ifndef NOI_SOFTWARE_CMAINTAINER_HPP
#define NOI_SOFTWARE_CMAINTAINER_HPP

#include <functional>
#include <thread>
#include <mutex>
#include <map>
#include <list>

namespace NMsc {
    class CMaintainer {
    public:
        ~CMaintainer();

        static CMaintainer &GetInstance();

        int RegisterTask(const std::function<void()> &task);

        void UnregisterTask(int taskId);

        void Stop();

        void Run();

        void ClearTaskList();

    private:
        CMaintainer();

        void ThreadWorker();

        std::mutex m_lock;
        bool m_shouldStop;
        uint32_t m_uniqueIdCounter;
        std::map<int, std::function<void()>> m_tasks;
        std::thread m_workerThread;

        static CMaintainer *m_instance;
    };
}

#endif //NOI_SOFTWARE_CMAINTAINER_HPP
