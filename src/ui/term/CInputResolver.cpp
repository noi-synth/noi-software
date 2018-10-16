//
// Created by ddl_blue on 31.7.18.
//

#include <ncurses.h>


#include "../../../include/ui/term/CInputResolver.hpp"

using namespace NUi::NTerm;
using namespace NUi;

/*----------------------------------------------------------------------*/
ControlInput CInputResolver::ResolveInput(int input) {
    switch (input) {

        // Playtback
        case 's':
            return ControlInput::BTN_PLAY;
        case 'd':
            return ControlInput::BTN_REC;

            // Arrows
        case KEY_RIGHT:
            return ControlInput::BTN_RIGHT;
        case KEY_UP:
            return ControlInput::BTN_UP;
        case KEY_DOWN:
            return ControlInput::BTN_DOWN;
        case KEY_LEFT:
            return ControlInput::BTN_LEFT;

            // Control
        case '\n':
            return ControlInput::BTN_OK;
        case KEY_BACKSPACE:
            return ControlInput::BTN_BACK;
        case 'q':
            return ControlInput::BTN_SHUTDOWN;

            // Notes
        case 'w':
            return ControlInput::NOTE_0;
        case 'e':
            return ControlInput::NOTE_1;
        case 'r':
            return ControlInput::NOTE_2;
        case 't':
            return ControlInput::NOTE_3;

        default:
            return ControlInput::NONE;
    }
}