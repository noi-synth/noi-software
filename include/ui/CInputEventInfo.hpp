//
// Created by ddl_blue on 14.4.19.
//

#ifndef NOI_SOFTWARE_CINPUTEVENTINFO_HPP
#define NOI_SOFTWARE_CINPUTEVENTINFO_HPP

#include "UiEnums.hpp"

namespace NUi {
    /**
     * Represents internal input event info.
     */
    struct CInptutEventInfo {
        /**
         * Constructor
         * @param input What input it was
         * @param type What happened to the input
         * @param shift Was shift key pressed?
         */
        CInptutEventInfo(EControlInput input, EControlInputType type, bool shift) : m_input(input), m_type(type),
                                                                                    m_shift(shift) {}

        CInptutEventInfo() : m_input(EControlInput::NONE), m_type(EControlInputType::NO_TYPE), m_shift(false) {};

        /// True if the event means knobs rotating
        bool IsScroll() { return m_type == EControlInputType::SCROLL_DOWN || m_type == EControlInputType::SCROLL_UP; }

        /// True if event happened to function keys
        bool IsFnKey() { return m_input >= EControlInput::BTN_FN_0 && m_input <= EControlInput::BTN_FN_7; }

        /// What input it was
        EControlInput m_input;

        /// What happenned to the input
        EControlInputType m_type;

        /// Was shift key pressed?
        bool m_shift;
    };
}
#endif //NOI_SOFTWARE_CINPUTEVENTINFO_HPP
