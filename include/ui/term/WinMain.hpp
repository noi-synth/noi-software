//
// Created by ddl_blue on 2.8.18.
//

#ifndef NOI_SOFTWARE_WNDMAIN_HPP
#define NOI_SOFTWARE_WNDMAIN_HPP

#include "../CWindow.hpp"

namespace NUi {
    namespace NTerm {
        class WinMain : public CWindow {
        public:
            explicit WinMain(WWindowManager windowManager);

            virtual EControlInput ProcessInput(EControlInput control, EControlInputType type) override;

            virtual void Draw() override;

            void Update() override;

            void Init() override;

        private:
            NLgc::ANoiApp m_app;
            int m_frameBufferLength;
            int m_position;
        };
    }

}

#endif //NOI_SOFTWARE_WNDMAIN_HPP
