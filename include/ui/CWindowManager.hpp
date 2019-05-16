//
// Created by ddl_blue on 7.7.18.
//

#ifndef NOI_SOFTWARE_CWINDOWMANAGER_HPP
#define NOI_SOFTWARE_CWINDOWMANAGER_HPP

#include <list>
#include <memory>

#include "CInputEventInfo.hpp"

namespace NUi {
    class CWindowManager;

    typedef std::shared_ptr<CWindowManager> AWindowManager;
    typedef std::weak_ptr<CWindowManager> WWindowManager;
}

#include "CWindow.hpp"
#include "../lgc/CNoiApp.hpp"

namespace NUi {
    /**
     * Window manager. Manages windows.
     */
    class CWindowManager : public std::enable_shared_from_this<CWindowManager> {
    public:
        /**
         * Constructor
         * @param drawingPolicy Defines what windows will recive Draw method calls
         * @param app Reference to the main App class
         */
        CWindowManager(EDrawingPolicy drawingPolicy, NLgc::ANoiApp app);

        // static AWindowManager CreateSharedInstance(EDrawingPolicy drawingPolicy, AWindow mainWindow);

        // todo these "Callback" names are a bit unlucky
        /**
         * Replaces main window with other window
         * @param window New main window
         */
        void SetMainWindowCallback(const AWindow &window);

        /**
         * Replace top window with other window
         * @param window New top window
         */
        void ReplaceTopWindowCallback(AWindow window);

        /**
         * Open window on top of all windows
         * @param window Window to be openned
         */
        void OpenWindowCallback(AWindow window);

        /**
         * Close all windows but main window and open this window on top
         * @param window New top window
         */
        void OpenSingleWindowCallback(AWindow window);

        /**
         * Close the window on top
         */
        void CloseTopWindowCallback();

        /**
         * Sends user input to windows.
         * @param input Input to be processed
         */
        void ProcessControlInput(CInptutEventInfo input);

        /**
         * Will do redraw on next update.
         */
        void RequestRedraw();

        /**
         * Update windows. Should be called periodically from UI thread
         */
        void Update();

        /// Information for UI to exit
        bool m_exiting;

        /**
         * Get reference to main App class
         * @return reference to App class
         */
        NLgc::ANoiApp GetApp();

        /// Affects how windows are being drawn
        EDrawingPolicy m_drawingPolicy;
    private:

        /// Redraws all windows
        void Redraw();

        /// True if redraw was requested
        bool m_redrawRequested;

        /// True if window iteration should not continue due to window manipulation
        bool m_breakWindowIteration;

        /// Stack of windows
        std::list<AWindow> m_windows;

        /// Windows that were deleted from window stack. They are kept there until next update, so they are not deleted too soon.
        std::list<AWindow> m_deletedWindows;

        /// Main window of the UI
        AWindow m_mainWindow;

        /// Reference to main App class
        NLgc::ANoiApp m_app;
    };

}

#endif //NOI_SOFTWARE_CWINDOWMANAGER_HPP
