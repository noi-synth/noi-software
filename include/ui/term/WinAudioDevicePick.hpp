//
// Created by ddl_blue on 2.8.18.
//

#ifndef NOI_SOFTWARE_WINAUDIODEVICEPICK_HPP
#define NOI_SOFTWARE_WINAUDIODEVICEPICK_HPP

#include "../CWindow.hpp"

namespace NUi {
    namespace NTerm {
        class WinAudioDevicePick : public CWindow {

        public:

            WinAudioDevicePick(WWindowManager manager);

            //void Update() override;

            ControlInput ProcessInput(ControlInput control, ControlInputType type) override;

            void Draw() override;
        };
    }
}

#endif //NOI_SOFTWARE_WINAUDIODEVICEPICK_HPP
