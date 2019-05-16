//
// Created by ddl_blue on 13.5.19.
//

#ifndef NOI_SOFTWARE_CWINPGEFFECTS_HPP
#define NOI_SOFTWARE_CWINPGEFFECTS_HPP

#include "../CWindow.hpp"

namespace NUi::NZero {
    /**
     * Page for effect manipulation
     */
    class CWinPgEffects : public CWindow {
    public:
        /**
         * Constructor
         * @param windowManager Parent window manager
         */
        explicit CWinPgEffects(WWindowManager windowManager);

        virtual CInptutEventInfo ProcessInput(CInptutEventInfo input) override;

        virtual void Draw() override;

    private:
        /// Reference to main App class
        NLgc::ANoiApp m_app;

        /// Effect that is being being currently edited.
        NSnd::AEffect m_editingEffect;
    };
}


#endif //NOI_SOFTWARE_CWINPGEFFECTS_HPP
