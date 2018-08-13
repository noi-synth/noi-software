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
    class CUi {
    public:
        CUi();

        void Run();

        void Stop();

        void WaitForStop();

        bool IsRunning();

    protected:
        NUi::AWindowManager m_windowManager;
        std::atomic_bool m_shouldStop;
    private:


        virtual void UiThreadWorker() = 0;

        std::mutex m_runningSignalizer;
        std::thread m_uiThread;
    };
}


#endif //NOI_SOFTWARE_CUI_HPP
