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
#include <atomic>

namespace NMsc {
    /**
     * This is a singleton class that allows you to periodically call some code from another thread
     * so you don't need to maintain your extra thread.
     * Used mostly for classes with RT calls from audio devices that can't eg. manage memory etc.
     * Please do not abuse this class for time-consuming tasks. Playback and recording heavily relies on this class.
     */
    class CMaintainer {
    public:
        ~CMaintainer();

        /**
         * Get the singleton instance of the class.
         * @return CMaintainer instance
         */
        static CMaintainer &GetInstance();

        /**
         * Registers new task and runs worker thread, if not running.
         * @param task Task to be executed periodically.
         * @return Task id. Used for unregistering a task.
         */
        int RegisterTask(const std::function<void()> &task);

        /**
         * Unregisters the task with specified ID. You probably want to do this in destructor of an object that registered the task.
         * @param taskId ID of the task.
         */
        void UnregisterTask(int taskId);

        /**
         * Stop the worker thread if running. Blocking call.
         */
        void Stop();

        /***
         * Run the worker thread, if not running.
         */
        void Run();

        void ClearTaskList();

    private:
        /**
         * Private constructor for a singleton pattern.
         */
        CMaintainer();

        /**
         * Method that is being run in a worker thread.
         */
        void ThreadWorker();

        /**
         * Guards an access to the task list and ID counter.
         */
        std::mutex m_lock;

        /**
         * If set to true, worker thread will exit as soon as possible.
         */
        std::atomic_bool m_shouldStop;

        /**
         * Counts unique IDs for new registered tasks.
         */
        uint32_t m_uniqueIdCounter;

        /**
         * Contains all the tasks and their IDs.
         */
        std::map<int, std::function<void()>> m_tasks;

        /**
         * The worker thread that periodically calls all the tasks.
         */
        std::thread m_workerThread;

        /**
         * Instance of the maintainer for singleton pattern.
         */
        static CMaintainer *m_instance;

        /**
         * Number of milliseconds that worker thread waits between iterations.
         */
        static constexpr int WAIT_WOR_MILLISECONDS = 1000 / 10;
    };
}

#endif //NOI_SOFTWARE_CMAINTAINER_HPP
