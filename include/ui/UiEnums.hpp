//
// Created by ddl_blue on 7.7.18.
//

#ifndef NOI_SOFTWARE_UIENUMS_HPP
#define NOI_SOFTWARE_UIENUMS_HPP

namespace NUi {

    /*----------------------------------------------------------------------*/
    enum DrawingPolicy {
        DRAW_ALL,
        DRAW_TOP,
        DRAW_NONE
    };

    /*----------------------------------------------------------------------*/
    enum ControlInput {
        // Note keys
                NOTE_0, NOTE_1, NOTE_2, NOTE_3, NOTE_4, NOTE_5, NOTE_6, NOTE_7, NOTE_8, NOTE_9,
        NOTE_10, NOTE_11, NOTE_12, NOTE_13, NOTE_14, NOTE_15, NOTE_16, NOTE_17, NOTE_18, NOTE_19,
        NOTE_20, NOTE_21, NOTE_22, NOTE_23, NOTE_24, NOTE_25, NOTE_26, NOTE_27, NOTE_28, NOTE_29,
        NOTE_LAST, // For implementation purposes

        // Rotation counters
                ROT_0_LEFT, ROT_0_RIGHT, ROT_0_PRESS,
        ROT_1_LEFT, ROT_1_RIGHT, ROT_1_PRESS,
        ROT_2_LEFT, ROT_2_RIGHT, ROT_2_PRESS,
        ROT_3_LEFT, ROT_3_RIGHT, ROT_3_PRESS,

        BTN_REC, BTN_STOP, BTN_PLAY,

        BTN_LEFT, BTN_DOWN, BTN_RIGHT, BTN_UP,

        BTN_BACK,


        NONE
    };


}

#endif //NOI_SOFTWARE_UIENUMS_HPP
