//
// Created by ddl_blue on 31.7.18.
//

#ifndef NOI_SOFTWARE_CUI_HPP
#define NOI_SOFTWARE_CUI_HPP

#include <thread>
#include <atomic>
#include <mutex>
#include "CWindowManager.hpp"

namespace NUi {
    /**
     * Base class for user interfaces.
     */
    class CUi {
    public:
        CUi();

        /**
         * Start the UI thread
         */
        void Run();

        /**
         * Stop the UI thread.
         */
        void Stop();

        /**
         * Blocks until UI thread is stopped.
         */
        void WaitForStop();

        /**
         * Is UI thread running?
         * @return True if UI thread is running.
         */
        bool IsRunning();

    protected:
        /// Window namager of this UI
        NUi::AWindowManager m_windowManager;
        /// If true. UI thread will stop
        std::atomic_bool m_shouldStop;
    private:

        /// UI thread worker method
        virtual void UiThreadWorker() = 0;

        /// Tells if UI thread is running
        std::mutex m_runningSignalizer;

        /// UI thread
        std::thread m_uiThread;
    };
}


#endif //NOI_SOFTWARE_CUI_HPP
