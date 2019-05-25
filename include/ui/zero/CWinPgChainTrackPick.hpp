//
// Created by ddl_blue on 25.5.19.
//

#ifndef NOI_SOFTWARE_CWINPGCHAINTRACKPICK_HPP
#define NOI_SOFTWARE_CWINPGCHAINTRACKPICK_HPP

#include "../CWindow.hpp"

namespace NUi::NZero {
    /**
     * Window for page for
     */
    class CWinPgChainTrackPick : public CWindow {
    public:
        /**
         * Constructor
         * @param windowManager Parent window manager
         */
        explicit CWinPgChainTrackPick(WWindowManager windowManager);

        virtual CInptutEventInfo ProcessInput(CInptutEventInfo input) override;

        virtual void Draw() override;

    private:
        /// Reference to main App class
        NLgc::ANoiApp m_app;
    };
}

#endif //NOI_SOFTWARE_CWINPGCHAINTRACKPICK_HPP
