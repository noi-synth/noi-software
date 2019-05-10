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
CNoiZeroHw::CNoiZeroHw() : m_stopWorker(false), m_msWaiting(1),
                           m_extenders(std::vector<CI2cGpioExtender>(
                                   {
                                           CI2cGpioExtender(0x20, 0xFFFF, 0x0000), // A 1111 1111 1111 1111
                                           CI2cGpioExtender(0x21, 0x01FF, 0x7E00), // B x000 0001 1111 1111
                                           CI2cGpioExtender(0x22, 0xFFFF, 0x0000,
                                                            0b0000000010101010), // C 1111 1111 1111 1111
                                           CI2cGpioExtender(0x23, 0x0000, 0x7FFF), // D x000 0000 0000 0000
                                           CI2cGpioExtender(0x24, 0x0000, 0x7FFF)  // E x000 0000 0000 0000

                                   })) {
    // Initialize wiringPi
    NHw::CWiringPiHandler::Init();

    // LED positions ----
    // Status LEDs
    LED_POSITIONS[ELedId::S0] = CPinPosition(EExtenderId::E, 9);
    LED_POSITIONS[ELedId::S1] = CPinPosition(EExtenderId::E, 12);
    LED_POSITIONS[ELedId::S2] = CPinPosition(EExtenderId::B, 9);
    LED_POSITIONS[ELedId::S3] = CPinPosition(EExtenderId::B, 12);

    // Function LEDs
    LED_POSITIONS[ELedId::F0] = CPinPosition(EExtenderId::E, 6);
    LED_POSITIONS[ELedId::F1] = CPinPosition(EExtenderId::E, 3);
    LED_POSITIONS[ELedId::F2] = CPinPosition(EExtenderId::E, 0);
    LED_POSITIONS[ELedId::F3] = CPinPosition(EExtenderId::D, 12);
    LED_POSITIONS[ELedId::F4] = CPinPosition(EExtenderId::D, 9);
    LED_POSITIONS[ELedId::F5] = CPinPosition(EExtenderId::D, 6);
    LED_POSITIONS[ELedId::F6] = CPinPosition(EExtenderId::D, 3);
    LED_POSITIONS[ELedId::F7] = CPinPosition(EExtenderId::D, 0);

    // CONTROL positions ----
    //note keys
    CONTROL_POSITIONS[NUi::EControlInput::NOTE_0] = CPinPosition(EExtenderId::B, 0);
    CONTROL_POSITIONS[NUi::EControlInput::NOTE_1] = CPinPosition(EExtenderId::A, 0);
    CONTROL_POSITIONS[NUi::EControlInput::NOTE_2] = CPinPosition(EExtenderId::A, 1);
    CONTROL_POSITIONS[NUi::EControlInput::NOTE_3] = CPinPosition(EExtenderId::A, 2);
    CONTROL_POSITIONS[NUi::EControlInput::NOTE_4] = CPinPosition(EExtenderId::A, 3);
    CONTROL_POSITIONS[NUi::EControlInput::NOTE_5] = CPinPosition(EExtenderId::A, 4);
    CONTROL_POSITIONS[NUi::EControlInput::NOTE_6] = CPinPosition(EExtenderId::A, 5);
    CONTROL_POSITIONS[NUi::EControlInput::NOTE_7] = CPinPosition(EExtenderId::A, 6);
    CONTROL_POSITIONS[NUi::EControlInput::NOTE_8] = CPinPosition(EExtenderId::A, 7);
    CONTROL_POSITIONS[NUi::EControlInput::NOTE_9] = CPinPosition(EExtenderId::A, 8);
    CONTROL_POSITIONS[NUi::EControlInput::NOTE_10] = CPinPosition(EExtenderId::A, 9);
    CONTROL_POSITIONS[NUi::EControlInput::NOTE_11] = CPinPosition(EExtenderId::A, 10);
    CONTROL_POSITIONS[NUi::EControlInput::NOTE_12] = CPinPosition(EExtenderId::A, 11);
    CONTROL_POSITIONS[NUi::EControlInput::NOTE_13] = CPinPosition(EExtenderId::A, 12);
    CONTROL_POSITIONS[NUi::EControlInput::NOTE_14] = CPinPosition(EExtenderId::A, 13);
    CONTROL_POSITIONS[NUi::EControlInput::NOTE_15] = CPinPosition(EExtenderId::A, 14);
    CONTROL_POSITIONS[NUi::EControlInput::NOTE_16] = CPinPosition(EExtenderId::A, 15);

    // Function keys
    CONTROL_POSITIONS[NUi::EControlInput::BTN_FN_0] = CPinPosition(EExtenderId::C, 8);
    CONTROL_POSITIONS[NUi::EControlInput::BTN_FN_1] = CPinPosition(EExtenderId::C, 9);
    CONTROL_POSITIONS[NUi::EControlInput::BTN_FN_2] = CPinPosition(EExtenderId::C, 10);
    CONTROL_POSITIONS[NUi::EControlInput::BTN_FN_3] = CPinPosition(EExtenderId::C, 11);
    CONTROL_POSITIONS[NUi::EControlInput::BTN_FN_4] = CPinPosition(EExtenderId::C, 12);
    CONTROL_POSITIONS[NUi::EControlInput::BTN_FN_5] = CPinPosition(EExtenderId::C, 13);
    CONTROL_POSITIONS[NUi::EControlInput::BTN_FN_6] = CPinPosition(EExtenderId::C, 14);
    CONTROL_POSITIONS[NUi::EControlInput::BTN_FN_7] = CPinPosition(EExtenderId::C, 15);

    // Control keys
    CONTROL_POSITIONS[NUi::EControlInput::BTN_PAGE] = CPinPosition(EExtenderId::B, 8);
    CONTROL_POSITIONS[NUi::EControlInput::BTN_METRONOME] = CPinPosition(EExtenderId::B, 7);
    CONTROL_POSITIONS[NUi::EControlInput::BTN_PLAY] = CPinPosition(EExtenderId::B, 6);
    CONTROL_POSITIONS[NUi::EControlInput::BTN_REC] = CPinPosition(EExtenderId::B, 5);
    CONTROL_POSITIONS[NUi::EControlInput::BTN_UNDO] = CPinPosition(EExtenderId::B, 4);
    CONTROL_POSITIONS[NUi::EControlInput::BTN_SHIFT] = CPinPosition(EExtenderId::B, 3);
    CONTROL_POSITIONS[NUi::EControlInput::BTN_LEFT] = CPinPosition(EExtenderId::B, 2);
    CONTROL_POSITIONS[NUi::EControlInput::BTN_RIGHT] = CPinPosition(EExtenderId::B, 1);

    // Knobs
    CONTROL_POSITIONS[NUi::EControlInput::ROT_0] = CPinPosition(EExtenderId::C, 0);
    CONTROL_POSITIONS[NUi::EControlInput::ROT_1] = CPinPosition(EExtenderId::C, 2);
    CONTROL_POSITIONS[NUi::EControlInput::ROT_2] = CPinPosition(EExtenderId::C, 4);
    CONTROL_POSITIONS[NUi::EControlInput::ROT_3] = CPinPosition(EExtenderId::C, 6);



    // Inverted table
    for (uint32_t i = 0; i < EXTENDER_CNT * 16; ++i) {
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

        usleep(300);
        int res = digitalRead(7);

        /*if (!res) {
            std::cout << "INT" << std::endl;
        }*/

        uint16_t value, diff, preValue, preDiff;
        EExtenderId extenderId = EExtenderId::A;
        // Did interrupt come?
        if (!res) {
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

    for (uint32_t i = 0; i < 16 && diff; ++i) {

        // Next pin
        if (i) {
            value >>= 1;
            diff >>= 1;
        }

        if (diff & 1) {

            CPinPosition position(extenderId, i);
            NUi::EControlInput input = CONTROLS[position.GetGlobalPortNumber()];

            if (input == NUi::EControlInput::NONE)
                continue;

            // Knobs
            if (input > NUi::EControlInput::_ROT_FIRST && input < NUi::EControlInput::_ROT_LAST) {

                // Only evaluate knobs from interrupt-time values.
                if (interruptTimeValues && (value & 1)) {
                    SendControl(NUi::CInptutEventInfo(input, (value & 2) ? NUi::EControlInputType::SCROLL_DOWN
                                                                         : NUi::EControlInputType::SCROLL_UP, false));
                }

                // Don't send knob updates either as note keys or button presses.
                continue;
            }

            // Note keys
            if (input > NUi::EControlInput::_NOTE_FIRST && input < NUi::EControlInput::_NOTE_LAST) {
                NSnd::ETones tone = static_cast<NSnd::ETones>(input - NUi::EControlInput::_NOTE_FIRST);
                SendMidiMsg(NSnd::CMidiMsg((value & 1) ? NSnd::EMidiMsgType::NOTE_OFF : NSnd::EMidiMsgType::NOTE_ON,
                                           tone, 255));
            }

            // Send control message
            SendControl(NUi::CInptutEventInfo(input,
                                              (value & 1) ? NUi::EControlInputType::RELEASE
                                                          : NUi::EControlInputType::PRESS, false)
            );
        }
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