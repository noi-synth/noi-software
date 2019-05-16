//
// Created by ddl_blue on 2.8.18.
//

#ifndef NOI_SOFTWARE_WNDMAIN_HPP
#define NOI_SOFTWARE_WNDMAIN_HPP

#include "../CWindow.hpp"

namespace NUi {
    namespace NTerm {
        /**
         * Main window of terminal UI
         */
        class WinMain : public CWindow {
        public:
            explicit WinMain(WWindowManager windowManager);

            virtual CInptutEventInfo ProcessInput(CInptutEventInfo input) override;

            virtual void Draw() override;

            void Update() override;

            void Init() override;

        private:
            /// Reference to main app
            NLgc::ANoiApp m_app;

            /// number of frames requested from latest RT thread call
            int m_frameBufferLength;
            // int m_position;
        };
    }

}

#endif //NOI_SOFTWARE_WNDMAIN_HPP
