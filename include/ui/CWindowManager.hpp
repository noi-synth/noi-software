//
// Created by ddl_blue on 7.7.18.
//

#ifndef NOI_SOFTWARE_CWINDOWMANAGER_HPP
#define NOI_SOFTWARE_CWINDOWMANAGER_HPP

#include <list>
#include <memory>

namespace NUi {
    class CWindowManager;

    typedef std::shared_ptr<CWindowManager> AWindowManager;
    typedef std::weak_ptr<CWindowManager> WWindowManager;
}

#include "CWindow.hpp"
#include "../lgc/CNoiApp.hpp"

namespace NUi {
    class CWindowManager : public std::enable_shared_from_this<CWindowManager> {
    public:
        CWindowManager(EDrawingPolicy drawingPolicy, NLgc::ANoiApp app);

        // static AWindowManager CreateSharedInstance(EDrawingPolicy drawingPolicy, AWindow mainWindow);

        // todo these "Callback" names are a bit unlucky
        void SetMainWindowCallback(const AWindow &window);

        void ReplaceTopWindowCallback(AWindow window);

        void OpenWindowCallback(AWindow window);

        void OpenSingleWindowCallback(AWindow window);

        void CloseTopWindowCallback();

        void ProcessControlInput(EControlInput control, EControlInputType type);

        void RequestRedraw();

        void Update();

        bool m_exiting;

        NLgc::ANoiApp GetApp();

        EDrawingPolicy m_drawingPolicy;
    private:

        void Redraw();

        bool m_redrawRequested;
        bool m_breakWindowIteration;
        std::list<AWindow> m_windows;
        std::list<AWindow> m_deletedWindows;
        AWindow m_mainWindow;
        NLgc::ANoiApp m_app;
    };

}

#endif //NOI_SOFTWARE_CWINDOWMANAGER_HPP
