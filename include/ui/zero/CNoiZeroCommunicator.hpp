//
// Created by ddl_blue on 9.5.19.
//

#ifndef NOI_SOFTWARE_CNOIZEROCOMMUNICATOR_HPP
#define NOI_SOFTWARE_CNOIZEROCOMMUNICATOR_HPP

namespace NUi {}
namespace NUi::NZero {
    class CNoiZeroCommunicator;
}

#include <cstdint>
#include "CZeroUi.hpp"
#include "../../hw/HwEnums.hpp"

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
        CNoiZeroCommunicator();

        virtual ~CNoiZeroCommunicator();

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
        virtual void SetFnLed(uint32_t ledId, ELedState state, NHw::ELedColor color) = 0;

        /**
         * Set state of status LED
         * @param ledId Id of the LED
         * @param state state of the LED
         * @param color Color of the LED
         */
        virtual void SetStatusLed(EStatusLed ledId, ELedState state, NHw::ELedColor color) = 0;

        /**
         * Turn all Fn LEDs black
         */
        virtual void ClearFnLeds() = 0;

        /**
         * Turn all status LEDs black
         */
        virtual void ClearStatusLeds() = 0;

        /**
         * Submit changes to the HW. Should be called at the end of redrawing the whole UI.
         */
        virtual void Update() = 0;

        /// Long color gradient
        const NHw::ELedColor COL_GRADIENT_LONG[7] = {NHw::ELedColor::RED, NHw::ELedColor::YELLOW,
                                                     NHw::ELedColor::GREEN, NHw::ELedColor::CYAN, NHw::ELedColor::BLUE,
                                                     NHw::ELedColor::MAGENTA, NHw::ELedColor::WHITE};
        /// Short color gradient
        const NHw::ELedColor COL_GRADIENT_SHORT[3] = {NHw::ELedColor::RED, NHw::ELedColor::GREEN, NHw::ELedColor::BLUE};

    protected:

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

    };


}


#endif //NOI_SOFTWARE_CNOIZEROCOMMUNICATOR_HPP
