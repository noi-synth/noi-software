//
// Created by ddl_blue on 7.4.18.
//

#ifndef NOI_SOFTWARE_CMIDIMSG_HPP
#define NOI_SOFTWARE_CMIDIMSG_HPP

#include <cstdint>
#include "SndEnums.hpp"

namespace NSnd {
    class CMidiMsg {
    public:
        CMidiMsg() = default;

        CMidiMsg(EMidiMsgType type, ETones tone, uint8_t presure);

        //TODO design the concept of these messages in more detail
        EMidiMsgType m_type;
        ETones m_tone;
        uint8_t m_presure;
    };
}

#endif //NOI_SOFTWARE_CMIDIMSG_HPP
