//
// Created by ddl_blue on 14.4.19.
//

#ifndef NOI_SOFTWARE_HWENUMS_HPP
#define NOI_SOFTWARE_HWENUMS_HPP
namespace NHw {

    /**
     * All possible colors of RGB LED
     */
    enum ELedColor {
        BLACK = 0b000,
        RED = 0b001,
        GREEN = 0b010,
        BLUE = 0b100,
        YELLOW = RED | GREEN,
        CYAN = BLUE | GREEN,
        MAGENTA = BLUE | RED,
        WHITE = RED | GREEN | BLUE
    };

    /*----------------------------------------------------------------------*/
    /**
     * All LEDs connected to Noi Zero
     */
    enum ELedId {
        S0, S1, S2, S3,
        F0, F1, F2, F3, F4, F5, F6, F7,
        _LED_NUMBER
    };

    /*----------------------------------------------------------------------*/
    /**
     * All GPIO extenders connected to the device
     */
    enum EExtenderId {
        A, B, C, D, E
    };
}
#endif //NOI_SOFTWARE_HWENUMS_HPP
