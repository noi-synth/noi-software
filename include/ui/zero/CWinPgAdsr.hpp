//
// Created by ddl_blue on 28.6.19.
//

#ifndef NOI_SOFTWARE_CWINPGADSR_HPP
#define NOI_SOFTWARE_CWINPGADSR_HPP

#include "../CWindow.hpp"

namespace NUi::NZero {
    class CWinPgAdsr : public CWindow {
    public:
        explicit CWinPgAdsr(WWindowManager windowManager);

        virtual CInptutEventInfo ProcessInput(CInptutEventInfo input) override;

        virtual void Draw() override;

    private:
        /// Reference to main App class
        NLgc::ANoiApp m_app;

    };
}

#endif //NOI_SOFTWARE_CWINPGADSR_HPP
