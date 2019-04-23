//
// Created by ddl_blue on 10.9.18.
//

#include "../../include/hw/CNoiZeroHw.hpp"
#include "../../include/hw/CWiringPiHandler.hpp"

#ifndef NO_RPI_HW

#include <iostream>
#include <functional>
#include <unistd.h>

using namespace NHw;

/*----------------------------------------------------------------------*/
/*CNoiZeroHw::CNoiZeroHw()
#ifndef NO_RPI_HW
        : m_extenders(0x20, 0xf, 0x0, [this]( std::uint16_t preVal, std::uint16_t preDiff, std::uint16_t val, std::uint16_t diff) {

    for (int i = 0; i < 4; ++i) {
        if (diff & 1) {
            for (const auto &midiOutput : m_midiOutputs) {

                NSnd::CMidiMsg msg(((val & 1) ? NSnd::EMidiMsgType::NOTE_OFF : NSnd::EMidiMsgType::NOTE_ON),
                                   (NSnd::ETones) (NSnd::ETones::C4 + i), 255);
                midiOutput(msg);
            }
        }
    diff >>= 1;
    val >>= 1;
    }

})
#endif
{

}*/

/*----------------------------------------------------------------------*/
CNoiZeroHw::CNoiZeroHw() : m_stopWorker(false), m_msWaiting(1),
                           m_extenders(std::vector<CI2cGpioExtender>(
                                   {
                                           CI2cGpioExtender(0x20, 0x00ff, 0xff00), // A
                                           CI2cGpioExtender(0x21, 0xffff, 0x0000), // D
                                           CI2cGpioExtender(0x22, 0x03ff, 0xFC00), // C
                                           CI2cGpioExtender(0x23, 0x0000, 0xFFFF), // E
                                           CI2cGpioExtender(0x24, 0x0000, 0xFFFF)  // B

                                   })) {
    // Initialize wiringPi
    NHw::CWiringPiHandler::Init();

    // LED positions
    LED_POSITIONS[ELedId::S0] = CPinPosition(EExtenderId::A, 15);

    // CONTROL positions
    CONTROL_POSITIONS[NUi::EControlInput::ROT_0] = CPinPosition(EExtenderId::A, 0);
    CONTROL_POSITIONS[NUi::EControlInput::BTN_OK] = CPinPosition(EExtenderId::A, 2);

    // Inverted table
    for (uint32_t i = 0; i < EXTENDER_CNT; ++i) {
        CONTROLS[i] = NUi::EControlInput::NONE;
    }
    for (uint32_t i = 0; i < NUi::EControlInput::_CONTROL_LAST; ++i) {
        if (!CONTROL_POSITIONS[i].m_empty)
            CONTROLS[CONTROL_POSITIONS[i].GetGlobalPortNumber()] = (NUi::EControlInput) i;
    }

    // Pin 7 as input
    pinMode(7, INPUT);

    // Start worker thread
    m_worker = std::thread([&]() { WorkerMethod(); });
    // todo make thread high priority
}

/*----------------------------------------------------------------------*/
CNoiZeroHw::~CNoiZeroHw() {
    // Join worker thread.
    m_stopWorker = true;
    m_worker.join();
}

/*----------------------------------------------------------------------*/
void CNoiZeroHw::AttachControlOutput(NMsc::ALocklessQue<NUi::CInptutEventInfo> outputQue) {
    m_controlOutputs.push_back(outputQue);
}

/*----------------------------------------------------------------------*/
void CNoiZeroHw::ClearControlOutputs() {
    m_controlOutputs.clear();
}

/*----------------------------------------------------------------------*/
void CNoiZeroHw::AttachMidiOutput(const std::function<void(NSnd::CMidiMsg)> &outputFunction) {
    m_midiOutputs.push_back(outputFunction);
}

/*----------------------------------------------------------------------*/
void CNoiZeroHw::ClearMidiOuptuts() {
    m_midiOutputs.clear();
}

/*----------------------------------------------------------------------*/
void CNoiZeroHw::SetLedOutput(NHw::ELedId ledId, NHw::ELedColor color) {
    CPinPosition pin = LED_POSITIONS[(int) ledId];

    if (pin.m_empty) {
        NMsc::CLogger::Log(NMsc::ELogType::ERROR, "CNoiZeroHw: Trying to set LED number %, that has no pin.", ledId);
        return;
    }

    uint16_t val = m_extenders[pin.m_extenderId].m_output;

    // Clear bits
    val &= ~(7 << pin.m_pinId);

    // Set bits
    val |= color << pin.m_pinId;

    m_extenders[pin.m_extenderId].m_output = val;
}

/*----------------------------------------------------------------------*/
void CNoiZeroHw::WorkerMethod() {
    while (!m_stopWorker) {
        //int res = waitForInterrupt(7, m_msWaiting);

        usleep(300);
        int res = digitalRead(7);
        res = 1 - res;

        /*if (res < 0) {
            NMsc::CLogger::Log(NMsc::ELogType::ERROR, "CNoiZeroHw: GPIO interrupt error %.", res);
            continue;
        }*/

        uint16_t value, diff, preValue, preDiff;
        EExtenderId extenderId = EExtenderId::A;
        // Did interrupt come?
        if (res) {
            m_msWaiting = MIN_WAITING;
            for (auto &extender : m_extenders) {
                // Get values from extender
                extender.UpdateInputAfterInterrupt(preValue, preDiff, value, diff);

                // Process both of them
                ProcessReadTimeValues(extenderId, preValue, preDiff, true);
                ProcessReadTimeValues(extenderId, value, diff);

                // Next extender...
                extenderId = static_cast<EExtenderId >(extenderId + 1);
            }

        } else {
            if (m_msWaiting < MAX_WAITING) {
                m_msWaiting += STEP_WAITING;
            }

            for (auto &extender : m_extenders) {
                // Get values from extender
                extender.UpdateInput(value, diff);

                // Process them
                ProcessReadTimeValues(extenderId, value, diff);

                // Next extender...
                extenderId = static_cast<EExtenderId >(extenderId + 1);
            }
        }


        // Update outputs
        for (auto &extender : m_extenders) {
            extender.UpdateOutput();
        }

    }
}

/*----------------------------------------------------------------------*/
void CNoiZeroHw::ProcessReadTimeValues(NHw::EExtenderId extenderId, uint16_t value, uint16_t diff,
                                       bool interruptTimeValues) {

    for (uint32_t i = 0; i < 16; ++i) {
        if (diff & 1) {

            CPinPosition position(extenderId, i);
            NUi::EControlInput input = CONTROLS[position.GetGlobalPortNumber()];

            if (input == NUi::EControlInput::NONE)
                continue;

            // Knobs
            if (input > NUi::EControlInput::_ROT_FIRST && input < NUi::EControlInput::_BTN_LAST) {

                // Only evaluate knobs from interrupt-time values.
                if (interruptTimeValues && (value & 1)) {
                    SendControl(NUi::CInptutEventInfo(input, (value & 2) ? NUi::EControlInputType::SCROLL_UP
                                                                         : NUi::EControlInputType::SCROLL_DOWN));
                }

                // Don't send knob updates either as note keys or button presses.
                continue;
            }

            // Note keys
            if (input > NUi::EControlInput::_NOTE_FIRST && input < NUi::EControlInput::_NOTE_LAST) {
                NSnd::ETones tone = static_cast<NSnd::ETones>(input - NUi::EControlInput::_NOTE_FIRST);
                SendMidiMsg(NSnd::CMidiMsg((value & 1) ? NSnd::EMidiMsgType::NOTE_ON : NSnd::EMidiMsgType::NOTE_OFF,
                                           tone, 255));
            }

            // Send control message
            SendControl(NUi::CInptutEventInfo(input, (value & 1) ? NUi::EControlInputType::PRESS
                                                                 : NUi::EControlInputType::RELEASE));
        }

        // Next pin
        value >>= 1;
        diff >>= 1;
    }
}

/*----------------------------------------------------------------------*/
void CNoiZeroHw::SendMidiMsg(NSnd::CMidiMsg message) {
    for (auto const &output : m_midiOutputs) {
        output(message);
    }
}

/*----------------------------------------------------------------------*/
void CNoiZeroHw::SendControl(NUi::CInptutEventInfo control) {
    for (auto &output : m_controlOutputs) {
        output->Push(control);
    }
}

#endif /* NO_RPI_HW */