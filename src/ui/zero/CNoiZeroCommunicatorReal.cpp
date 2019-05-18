//
// Created by ddl_blue on 18.5.19.
//

#include "../../../include/ui/zero/CNoiZeroCommunicatorReal.hpp"

#include "../../../include/config.hpp"

#ifndef NO_RPI_HW

#include "../../../include/hw/CNoiZeroHw.hpp"

using namespace NUi::NZero;

/*----------------------------------------------------------------------*/
CNoiZeroCommunicatorReal::CNoiZeroCommunicatorReal(NHw::ANoiZeroHw hw, NUi::NZero::AZeroUi ui) : m_hw(hw), m_ui(ui),
                                                                                         m_blinkCounter(0),
                                                                                         m_shift(false) {
    m_inputQue = std::make_shared<NMsc::CLocklessQue<CInptutEventInfo>>();
    m_hw->AttachControlOutput(m_inputQue);
}

/*----------------------------------------------------------------------*/
void CNoiZeroCommunicatorReal::SetFnLed(uint32_t ledId, NUi::NZero::ELedState state, NHw::ELedColor color) {
    if (ledId > 7 || ledId < 0) {
        NMsc::CLogger::Log(NMsc::ELogType::ERROR, "CNoiZeroCommunicator: Fn LED ID % is out of bounds", ledId);
        return;
    }

    m_fnLeds[ledId].m_state = state;
    m_fnLeds[ledId].m_color = color;
}

/*----------------------------------------------------------------------*/
void CNoiZeroCommunicatorReal::SetStatusLed(NUi::NZero::EStatusLed ledId, NUi::NZero::ELedState state,
                                        NHw::ELedColor color) {

    m_statusLeds[(int) ledId].m_state = state;
    m_statusLeds[(int) ledId].m_color = color;
}

/*----------------------------------------------------------------------*/
void CNoiZeroCommunicatorReal::ClearFnLeds() {
    for (uint32_t i = 0; i < 8; ++i) {
        m_fnLeds[i].m_state = ELedState::OFF;
        m_fnLeds[i].m_color = NHw::ELedColor::BLACK;
    }
}

/*----------------------------------------------------------------------*/
void CNoiZeroCommunicatorReal::ClearStatusLeds() {
    for (uint32_t i = 0; i < 4; ++i) {
        m_statusLeds[i].m_state = ELedState::OFF;
        m_statusLeds[i].m_color = NHw::ELedColor::BLACK;
    }
}

/*----------------------------------------------------------------------*/
void CNoiZeroCommunicatorReal::Update() {
    ++m_blinkCounter;

    // Set Fn LEDs
    for (uint32_t i = 0; i < 8; ++i) {

        NHw::ELedColor col = m_fnLeds[i].m_color;
        if (m_fnLeds[i].m_state == ELedState::OFF ||
            (m_fnLeds[i].m_state == ELedState::BLINKING && (m_blinkCounter & BLINK_MASK))) {
            col = NHw::ELedColor::BLACK;
        }
        m_hw->SetLedOutput(NMsc::Functions::EnumAdd(NHw::ELedId::F0, i), col);
    }


    // Set Status LEDs
    for (uint32_t i = 0; i < 4; ++i) {

        NHw::ELedColor col = m_statusLeds[i].m_color;
        if (m_statusLeds[i].m_state == ELedState::OFF ||
            (m_statusLeds[i].m_state == ELedState::BLINKING && (m_blinkCounter & BLINK_MASK))) {
            col = NHw::ELedColor::BLACK;
        }
        m_hw->SetLedOutput(NMsc::Functions::EnumAdd(NHw::ELedId::S0, i), col);
    }


    while (!m_inputQue->Empty()) {
        CInptutEventInfo input = m_inputQue->Pop();

        // Shift
        if (input.m_input == EControlInput::BTN_SHIFT) {
            m_shift = input.m_type == EControlInputType::PRESS;
        }
        input.m_shift = m_shift;

        m_ui->ProcessInput(input);
    }
}

#endif /* NO_RPI_HW */