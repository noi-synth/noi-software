//
// Created by ddl_blue on 7.7.18.
//

#ifndef NOI_SOFTWARE_CWINDOWMANAGER_HPP
#define NOI_SOFTWARE_CWINDOWMANAGER_HPP

#include <stack>
#include "CWindow.hpp"

namespace NUi {
    class CWindowManager {
        std::stack<CWindow> m_windows;
    };
}

#endif //NOI_SOFTWARE_CWINDOWMANAGER_HPP
