//
// Created by ddl_blue on 7.7.18.
//

#ifndef NOI_SOFTWARE_UIENUMS_HPP
#define NOI_SOFTWARE_UIENUMS_HPP

namespace NUi {

    /*----------------------------------------------------------------------*/
    enum EDrawingPolicy {
        DRAW_ALL,
        DRAW_TOP,
        DRAW_NONE
    };

    /*----------------------------------------------------------------------*/
    enum EControlInput {
        _CONTROL_FIRST,

        // Note keys
        _NOTE_FIRST,
        NOTE_0, NOTE_1, NOTE_2, NOTE_3, NOTE_4, NOTE_5, NOTE_6, NOTE_7, NOTE_8, NOTE_9,
        NOTE_10, NOTE_11, NOTE_12, NOTE_13, NOTE_14, NOTE_15, NOTE_16, NOTE_17, NOTE_18, NOTE_19,
        NOTE_20, NOTE_21, NOTE_22, NOTE_23, NOTE_24, NOTE_25, NOTE_26, NOTE_27, NOTE_28, NOTE_29,
        _NOTE_LAST, // For implementation purposes

        // Rotation counters
                _ROT_FIRST,
        ROT_0, ROT_0_PRESS,
        ROT_1, ROT_1_PRESS,
        ROT_2, ROT_2_PRESS,
        ROT_3, ROT_3_PRESS,
        _ROT_LAST,

        _BTN_FIRST,

        /* NOI ZERO */

                BTN_PAGE, BTN_METRONOME,
        BTN_PLAY, BTN_REC,
        BTN_UNDO, BTN_SHIFT,

        BTN_LEFT, BTN_DOWN, BTN_RIGHT, BTN_UP,



        // Special / nurses debug only.
                BTN_NC_MENU,

        BTN_BACK, BTN_SHUTDOWN, BTN_OK,


        _BTN_LAST,

        _CONTROL_LAST,
        NONE
    };

    /*----------------------------------------------------------------------*/
    enum EControlInputType {
        PRESS,
        RELEASE,
        SCROLL_UP,
        SCROLL_DOWN
    };

}

#endif //NOI_SOFTWARE_UIENUMS_HPP
