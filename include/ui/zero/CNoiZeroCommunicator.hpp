//
// Created by ddl_blue on 9.5.19.
//

#ifndef NOI_SOFTWARE_CNOIZEROCOMMUNICATOR_HPP
#define NOI_SOFTWARE_CNOIZEROCOMMUNICATOR_HPP


#include <cstdint>
#include "../../hw/CNoiZeroHw.hpp"
#include "CZeroUi.hpp"

namespace NUi::NZero {

    enum ELedState {
        OFF, ON, BLINKING
    };

    enum EStatusLed {
        PAGE, PLAY, UNDO, OCTAVE
    };

    class CNoiZeroCommunicator {
    public:
        CNoiZeroCommunicator(NHw::ANoiZeroHw hw, AZeroUi ui);

        static CNoiZeroCommunicator *GetInstance();

        void SetFnLed(uint32_t ledId, ELedState state, NHw::ELedColor color);

        void SetStatusLed(EStatusLed ledId, ELedState state, NHw::ELedColor color);

        void ClearFnLeds();

        void ClearStatusLeds();

        void Update();

        const NHw::ELedColor COL_GRADIENT_LONG[7] = {NHw::ELedColor::RED, NHw::ELedColor::YELLOW,
                                                     NHw::ELedColor::GREEN, NHw::ELedColor::CYAN, NHw::ELedColor::BLUE,
                                                     NHw::ELedColor::MAGENTA, NHw::ELedColor::WHITE};

        const NHw::ELedColor COL_GRADIENT_SHORT[3] = {NHw::ELedColor::RED, NHw::ELedColor::GREEN, NHw::ELedColor::BLUE};

    private:
        NHw::ANoiZeroHw m_hw;
        AZeroUi m_ui;

        uint32_t m_blinkCounter;

        static CNoiZeroCommunicator *m_instance;

        struct CLedStatus {
            CLedStatus() : m_state(ELedState::OFF), m_color(NHw::ELedColor::BLACK) {};
            ELedState m_state;
            NHw::ELedColor m_color;
        };

        CLedStatus m_fnLeds[8];
        CLedStatus m_statusLeds[4];

        NMsc::ALocklessQue<CInptutEventInfo> m_inputQue;

        const uint32_t BLINK_MASK = 1;

        bool m_shift;

    };


}


#endif //NOI_SOFTWARE_CNOIZEROCOMMUNICATOR_HPP
