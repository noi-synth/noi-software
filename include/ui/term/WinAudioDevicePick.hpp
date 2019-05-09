//
// Created by ddl_blue on 2.8.18.
//

#ifndef NOI_SOFTWARE_WINAUDIODEVICEPICK_HPP
#define NOI_SOFTWARE_WINAUDIODEVICEPICK_HPP

#include "../CWindow.hpp"
#include "../../snd/CAudioDeviceInfo.hpp"

namespace NUi {
    namespace NTerm {
        class WinAudioDevicePick : public CWindow {

        public:

            WinAudioDevicePick(WWindowManager manager);

            //void Update() override;

            virtual CInptutEventInfo ProcessInput(CInptutEventInfo input) override;

            void Init() override;

            void Draw() override;

            void SetPaDevice();

        private:
            std::vector<NSnd::CAudioDeviceInfo> m_devices;
            int m_selectedDevice;
            int m_devCnt;
        };
    }
}

#endif //NOI_SOFTWARE_WINAUDIODEVICEPICK_HPP
