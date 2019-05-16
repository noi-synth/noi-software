//
// Created by ddl_blue on 7.4.18.
//

#ifndef NOI_SOFTWARE_CMIDIMSG_HPP
#define NOI_SOFTWARE_CMIDIMSG_HPP

#include <cstdint>
#include "SndEnums.hpp"

namespace NSnd {
    /**
     * Represents a simplified MIDI message. Used for controlling instruments
     */
    class CMidiMsg {
    public:
        CMidiMsg() = default;

        /**
         * Constructor
         * @param type Type of message
         * @param tone Tone
         * @param presure Pressure of the tone
         */
        CMidiMsg(EMidiMsgType type, ETones tone, uint8_t presure);

        //TODO design the concept of these messages in more detail
        /// Type of message
        EMidiMsgType m_type;

        /// Tone
        ETones m_tone;
        /// Pressure of the tone
        uint8_t m_presure;
    };
}

#endif //NOI_SOFTWARE_CMIDIMSG_HPP
