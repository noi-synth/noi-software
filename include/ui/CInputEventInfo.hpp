//
// Created by ddl_blue on 14.4.19.
//

#ifndef NOI_SOFTWARE_CINPUTEVENTINFO_HPP
#define NOI_SOFTWARE_CINPUTEVENTINFO_HPP

#include "UiEnums.hpp"

namespace NUi {
    struct CInptutEventInfo {
        CInptutEventInfo(EControlInput input, EControlInputType type) : m_input(input), m_type(type) {}

        CInptutEventInfo() = default;

        EControlInput m_input;
        EControlInputType m_type;
    };
}
#endif //NOI_SOFTWARE_CINPUTEVENTINFO_HPP
