//
// Created by ddl_blue on 7.7.18.
//

#ifndef NOI_SOFTWARE_CWINDOW_HPP
#define NOI_SOFTWARE_CWINDOW_HPP

#include "UiEnums.hpp"

namespace NUi {

    class CWindow {

        virtual void Activate() = 0;

        virtual void Deactivate() = 0;

        virtual ControlInput ProcessInput(ControlInput input) = 0;

        virtual void Draw() = 0;

    };
}

#endif //NOI_SOFTWARE_CWINDOW_HPP
