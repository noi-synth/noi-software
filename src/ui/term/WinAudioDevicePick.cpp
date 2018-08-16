//
// Created by ddl_blue on 2.8.18.
//

#include "../../../include/ui/term/WinAudioDevicePick.hpp"
#include "../../../include/gfx/CNcurses.hpp"
#include "../../../include/snd/CAudioDevicePA.hpp"
#include "../../../include/msc/CLogger.hpp"

using namespace NUi;
using namespace NUi::NTerm;
using Color = NGfx::CNcurses::ColorPair;

/*----------------------------------------------------------------------*/
WinAudioDevicePick::WinAudioDevicePick(NUi::WWindowManager manager) : CWindow(manager), m_selectedDevice(0) {}

/*----------------------------------------------------------------------*/
ControlInput WinAudioDevicePick::ProcessInput(ControlInput control, ControlInputType type) {

    AWindowManager wm = m_manager.lock();

    if (m_isActive)
        switch (control) {
            case ControlInput::BTN_SHUTDOWN:
                wm->CloseTopWindowCallback();
                return ControlInput::NONE;

            case ControlInput::BTN_UP:
                if (m_selectedDevice == 0)
                    m_selectedDevice = m_devCnt - 1;
                else
                    --m_selectedDevice;
                RequestRedraw();
                return ControlInput::NONE;

            case ControlInput::BTN_DOWN:
                m_selectedDevice = (m_selectedDevice + 1) % m_devCnt;
                RequestRedraw();
                return ControlInput::NONE;

            case BTN_OK:
                SetPaDevice();
                wm->CloseTopWindowCallback();
                return ControlInput::NONE;

            default:
                return ControlInput::NONE;
        }
}

/*----------------------------------------------------------------------*/
void WinAudioDevicePick::Draw() {
    NGfx::CNcurses *gfx = NGfx::CNcurses::GetInstance();

    gfx->SetFrame(80, 20);

    gfx->DrawEmptyWindow(Color::WHITE_BLUE);
    gfx->DrawTextCentered(2, "Pick sound device:", Color::WHITE_BLACK);

    unsigned int devDispY = 4;
    unsigned int i = 0;

    if (m_devices.empty())
        gfx->DrawText(2, devDispY, "No devices found.", Color::WHITE_BLUE);

    for (const auto &dev : m_devices) {
        const int BUFF_LEN = 75;
        char bff[BUFF_LEN];
        std::snprintf(bff, BUFF_LEN, "%s [%lf]", dev.m_name.c_str(), dev.m_latency);
        gfx->DrawText(2, devDispY + i++, bff, i == m_selectedDevice ? Color::WHITE_BLUE : Color::BLACK_BLUE);
    }

    gfx->Update();
}

void WinAudioDevicePick::Init() {
    m_devices = NSnd::CAudioDevicePA::GetAvailableDevices();
    m_devCnt = (int) m_devices.size();
    m_selectedDevice = NSnd::CAudioDevicePA::GetDefaultDeviceId();
}

/*----------------------------------------------------------------------*/

void WinAudioDevicePick::SetPaDevice() {
    DoWithManager([=](AWindowManager manager) {

        NSnd::AAudioDevice device = std::make_shared<NSnd::CAudioDevicePA>(m_devices[m_selectedDevice]);
        manager->GetApp()->AudioDeviceSet(device);
        manager->GetApp()->AudioStart();
    });
}