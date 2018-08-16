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

            virtual ControlInput ProcessInput(ControlInput control, ControlInputType type) override;

            virtual void Draw() override;

            void Init() override;

        private:
            NLgc::ANoiApp m_app;
        };
    }

}

#endif //NOI_SOFTWARE_WNDMAIN_HPP
