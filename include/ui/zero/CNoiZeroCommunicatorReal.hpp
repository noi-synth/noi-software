//
// Created by ddl_blue on 18.5.19.
//

#ifndef NOI_SOFTWARE_CNOIZEROCOMMUNICATORREAL_HPP
#define NOI_SOFTWARE_CNOIZEROCOMMUNICATORREAL_HPP

#include "CNoiZeroCommunicator.hpp"

#include "../../config.hpp"

#ifndef NO_RPI_HW

#include "../../hw/CNoiZeroHw.hpp"

namespace NUi::NZero {
    class CNoiZeroCommunicatorReal : public CNoiZeroCommunicator {
    public:
    public:
        /**
         * Constructor
         * @param hw Reference to CNoiZeroHw
         * @param ui Reference to UI connected to this class
         */
        CNoiZeroCommunicatorReal(NHw::ANoiZeroHw hw, AZeroUi ui);

        virtual void SetFnLed(uint32_t ledId, ELedState state, NHw::ELedColor color) override;


        virtual void SetStatusLed(EStatusLed ledId, ELedState state, NHw::ELedColor color) override;


        virtual void ClearFnLeds() override;


        virtual void ClearStatusLeds() override;


        virtual void Update() override;


    protected:
        /// Reference to HW controller
        NHw::ANoiZeroHw m_hw;

        /// Reference to user interface. Used for user input sending
        AZeroUi m_ui;

        /// Used for LED blinking
        uint32_t m_blinkCounter;

        /// Global instance of this calss
        static CNoiZeroCommunicator *m_instance;

        /**
         * Represents LED status
         */
        struct CLedStatus {
            CLedStatus() : m_state(ELedState::OFF), m_color(NHw::ELedColor::BLACK) {};

            /// State of the LED
            ELedState m_state;

            /// Color of the LED
            NHw::ELedColor m_color;
        };

        /// State of Fn LEDs
        CLedStatus m_fnLeds[8];

        /// State of status LEDs
        CLedStatus m_statusLeds[4];

        /// Que of user inputs
        NMsc::ALocklessQue<CInptutEventInfo> m_inputQue;

        /// Affects how fast is LED blinking
        const uint32_t BLINK_MASK = 1;

        /// True if shift key is being pressed
        bool m_shift;
    };
}

#endif /* NO_RPI_HW */

#endif //NOI_SOFTWARE_CNOIZEROCOMMUNICATORREAL_HPP
