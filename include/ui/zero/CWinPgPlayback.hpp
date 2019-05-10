//
// Created by ddl_blue on 10.5.19.
//

#ifndef NOI_SOFTWARE_CWINPGPLAYBACK_HPP
#define NOI_SOFTWARE_CWINPGPLAYBACK_HPP

#include "../CWindow.hpp"

namespace NUi::NZero {
    class CWinPgPlayback : public CWindow {
    public:
        explicit CWinPgPlayback(WWindowManager windowManager);

        virtual CInptutEventInfo ProcessInput(CInptutEventInfo input) override;

        virtual void Draw() override;

        void Update() override;

        //void Init() override;

    private:
        NLgc::ANoiApp m_app;
        uint32_t m_currentBeat;
    };
}


#endif //NOI_SOFTWARE_CWINPGPLAYBACK_HPP
