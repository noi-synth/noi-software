//
// Created by ddl_blue on 14.4.19.
//

#ifndef NOI_SOFTWARE_CINPUTEVENTINFO_HPP
#define NOI_SOFTWARE_CINPUTEVENTINFO_HPP

#include "UiEnums.hpp"

namespace NUi {
    struct CInptutEventInfo {
        CInptutEventInfo(EControlInput input, EControlInputType type, bool shift) : m_input(input), m_type(type),
                                                                                    m_shift(shift) {}

        CInptutEventInfo() : m_input(EControlInput::NONE), m_type(EControlInputType::NO_TYPE), m_shift(false) {};

        bool IsScroll() { return m_type == EControlInputType::SCROLL_DOWN || m_type == EControlInputType::SCROLL_UP; }

        bool IsFnKey() { return m_input >= EControlInput::BTN_FN_0 && m_input <= EControlInput::BTN_FN_7; }

        EControlInput m_input;
        EControlInputType m_type;
        bool m_shift;
    };
}
#endif //NOI_SOFTWARE_CINPUTEVENTINFO_HPP
