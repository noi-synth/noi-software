//
// Created by ddl_blue on 9.5.19.
//

#ifndef NOI_SOFTWARE_CNOIZEROCOMMUNICATOR_HPP
#define NOI_SOFTWARE_CNOIZEROCOMMUNICATOR_HPP


#include <cstdint>
#include "../../hw/CNoiZeroHw.hpp"
#include "CZeroUi.hpp"

namespace NUi::NZero {

    /**
     * Possible states of LED
     */
    enum ELedState {
        OFF, ON, BLINKING
    };

    /**
     * Meanings of status LEDs
     */
    enum EStatusLed {
        PAGE, PLAY, UNDO, OCTAVE
    };

    /**
     * Abstracts the access to NOI Zero HW.
     */
    class CNoiZeroCommunicator {
    public:
        /**
         * Constructor
         * @param hw Reference to CNoiZeroHw
         * @param ui Reference to UI connected to this class
         */
        CNoiZeroCommunicator(NHw::ANoiZeroHw hw, AZeroUi ui);

        /**
         * Get global instance of this class
         * @return Global instance of this class. Nullptr if none exists.
         */
        static CNoiZeroCommunicator *GetInstance();

        /**
         * Set state of Fn LED
         * @param ledId ID of Fn LED
         * @param state State of this LED
         * @param color Color of the LED
         */
        void SetFnLed(uint32_t ledId, ELedState state, NHw::ELedColor color);

        /**
         * Set state of status LED
         * @param ledId Id of the LED
         * @param state state of the LED
         * @param color Color of the LED
         */
        void SetStatusLed(EStatusLed ledId, ELedState state, NHw::ELedColor color);

        /**
         * Turn all Fn LEDs black
         */
        void ClearFnLeds();

        /**
         * Turn all status LEDs black
         */
        void ClearStatusLeds();

        /**
         * Submit changes to the HW. Should be called at the end of redrawing the whole UI.
         */
        void Update();

        /// Long color gradient
        const NHw::ELedColor COL_GRADIENT_LONG[7] = {NHw::ELedColor::RED, NHw::ELedColor::YELLOW,
                                                     NHw::ELedColor::GREEN, NHw::ELedColor::CYAN, NHw::ELedColor::BLUE,
                                                     NHw::ELedColor::MAGENTA, NHw::ELedColor::WHITE};
        /// Short color gradient
        const NHw::ELedColor COL_GRADIENT_SHORT[3] = {NHw::ELedColor::RED, NHw::ELedColor::GREEN, NHw::ELedColor::BLUE};

    private:
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


#endif //NOI_SOFTWARE_CNOIZEROCOMMUNICATOR_HPP
