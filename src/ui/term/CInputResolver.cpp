//
// Created by ddl_blue on 31.7.18.
//

#include <ncurses.h>


#include "../../../include/ui/term/CInputResolver.hpp"

using namespace NUi::NTerm;
using namespace NUi;

/*----------------------------------------------------------------------*/
EControlInput CInputResolver::ResolveInput(int input) {
    switch (input) {

        // Playtback
        case 's':
            return EControlInput::BTN_PLAY;
        case 'd':
            return EControlInput::BTN_REC;

            // Arrows
        case KEY_RIGHT:
            return EControlInput::BTN_RIGHT;
        case KEY_UP:
            return EControlInput::BTN_UP;
        case KEY_DOWN:
            return EControlInput::BTN_DOWN;
        case KEY_LEFT:
            return EControlInput::BTN_LEFT;

            // Control
        case '\n':
            return EControlInput::BTN_OK;
        case KEY_BACKSPACE:
            return EControlInput::BTN_BACK;
        case 'q':
            return EControlInput::BTN_SHUTDOWN;
        case 'a':
            return EControlInput::BTN_NC_MENU;


            // Notes
        case 'w':
            return EControlInput::NOTE_0;
        case 'e':
            return EControlInput::NOTE_1;
        case 'r':
            return EControlInput::NOTE_2;
        case 't':
            return EControlInput::NOTE_3;

        default:
            return EControlInput::NONE;
    }
}