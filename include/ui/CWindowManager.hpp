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

namespace NUi {
    class CWindowManager : public std::enable_shared_from_this<CWindowManager> {
    public:
        CWindowManager(DrawingPolicy drawingPolicy, AWindow mainWindow);

        // static AWindowManager CreateSharedInstance(DrawingPolicy drawingPolicy, AWindow mainWindow);

        // todo these "Callback" names are a bit unlucky
        void SetMainWindowCallback(AWindow &window);

        void ReplaceTopWindowCallback(AWindow window);

        void OpenWindowCallback(AWindow window);

        void OpenSingleWindowCallback(AWindow window);

        void CloseTopWindowCallback();

        void Redraw();

        void ProcessControlInput(ControlInput control, ControlInputType type);

        DrawingPolicy m_drawingPolicy;
    private:
        std::list<AWindow> m_windows;
        AWindow m_mainWindow;
    };

}

#endif //NOI_SOFTWARE_CWINDOWMANAGER_HPP
