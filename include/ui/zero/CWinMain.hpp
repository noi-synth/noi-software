//
// Created by ddl_blue on 9.5.19.
//

#ifndef NOI_SOFTWARE_CWINMAIN_HPP
#define NOI_SOFTWARE_CWINMAIN_HPP

#include "../CWindow.hpp"

namespace NUi {
    namespace NZero {
        class CWinMain : public CWindow {
        public:
            explicit CWinMain(WWindowManager windowManager);

            virtual CInptutEventInfo ProcessInput(CInptutEventInfo input) override;

            virtual void Draw() override;

            void Update() override;

            void Init() override;

        private:
            NLgc::ANoiApp m_app;

            bool m_metronom;
            uint32_t m_octave;

            uint32_t m_undoBlinkCountdown;

            uint32_t m_lastBeat;
            uint32_t m_beatBlinkCountdown;
        };
    }
}


#endif //NOI_SOFTWARE_CWINMAIN_HPP
