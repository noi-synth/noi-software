//
// Created by ddl_blue on 23.5.19.
//

#ifndef NOI_SOFTWARE_CWININSTRUMENT_HPP
#define NOI_SOFTWARE_CWININSTRUMENT_HPP

#include "../CWindow.hpp"

namespace NUi::NZero {
    class CWinInstrument : public CWindow {
    public:
        /**
         * Constructor
         * @param windowManager Parent window manager
         */
        explicit CWinInstrument(WWindowManager windowManager);

        virtual CInptutEventInfo ProcessInput(CInptutEventInfo input) override;

        virtual void Draw() override;

    private:
        /// Reference to main App class
        NLgc::ANoiApp m_app;
    };
}

#endif //NOI_SOFTWARE_CWININSTRUMENT_HPP
