//
// Created by ddl_blue on 7.7.18.
//

#ifndef NOI_SOFTWARE_CWINDOW_HPP
#define NOI_SOFTWARE_CWINDOW_HPP

#include <memory>
#include <functional>

namespace NUi {
    class CWindow;

    typedef std::shared_ptr<CWindow> AWindow;
    typedef std::weak_ptr<CWindow> WWindow;
}

#include "UiEnums.hpp"
#include "CWindowManager.hpp"

namespace NUi {

    /**
     * Represents a single window in UI system
     */
    class CWindow {
    public:
        /**
         * Constructor
         * @param windowManager Window manager of UI
         */
        CWindow(WWindowManager windowManager);

        /**
         * Called when window becomes a top window
         */
        virtual void Activate();

        /**
         * Called when window is no more a top window
         */
        virtual void Deactivate();

        /**
         * Process an user input. It can either pass the input to next window or catch it.
         * @param input User input
         * @return input if passing the input to next window or empty input
         */
        virtual CInptutEventInfo ProcessInput(CInptutEventInfo input);

        /**
         * Redraws the window.
         */
        virtual void Draw();

        /**
         * Updates the window
         */
        virtual void Update();

        /**
         * Called when opened by the manager
         */
        virtual void Init();


    protected:
        /**
         * Request call of the Draw method
         */
        void RequestRedraw();

        /**
         * Do an action with window manager.
         * @param funct Action to do.
         * @return True on success
         */
        bool DoWithManager(std::function<void(AWindowManager)> funct);

        /// Window manager this window belongs to
        WWindowManager m_manager;

        /// True if window is active (top window)
        bool m_isActive;
    };

}

#endif //NOI_SOFTWARE_CWINDOW_HPP
