//
// Created by ddl_blue on 13.5.19.
//

#ifndef NOI_SOFTWARE_CWINPGEFFECTS_HPP
#define NOI_SOFTWARE_CWINPGEFFECTS_HPP

#include "../CWindow.hpp"

namespace NUi::NZero {
    class CWinPgEffects : public CWindow {
    public:
        explicit CWinPgEffects(WWindowManager windowManager);

        virtual CInptutEventInfo ProcessInput(CInptutEventInfo input) override;

        virtual void Draw() override;

    private:
        NLgc::ANoiApp m_app;
        NSnd::AEffect m_editingEffect;
    };
}


#endif //NOI_SOFTWARE_CWINPGEFFECTS_HPP
