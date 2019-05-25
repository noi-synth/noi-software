//
// Created by ddl_blue on 23.5.19.
//

#ifndef NOI_SOFTWARE_CWINSYSTEMMENU_HPP
#define NOI_SOFTWARE_CWINSYSTEMMENU_HPP

#include "../CWindow.hpp"

namespace NUi::NZero {
    /**
     * Page for system menu. Allows user to clear/save/load, shutdown the device etc.
     */
    class CWinSystemMenu : public CWindow {
    public:
        /**
         * Constructor
         * @param windowManager Parent window manager
         */
        explicit CWinSystemMenu(WWindowManager windowManager);

        virtual CInptutEventInfo ProcessInput(CInptutEventInfo input) override;

        virtual void Draw() override;

        void Update() override;

    private:
        /// Reference to main App class
        NLgc::ANoiApp m_app;

        /// ID of "clear project" confirmation dialog's return value
        uint64_t m_clearProjectDialog;

        /// ID of "shutdown" confirmation dialog's return value
        uint64_t m_shutdownDialog;
    };
}

#endif //NOI_SOFTWARE_CWINSYSTEMMENU_HPP
