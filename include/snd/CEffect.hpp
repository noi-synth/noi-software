//
// Created by ddl_blue on 26.12.17.
//

#ifndef NOI_SOFTWARE_CEFFECT_HPP
#define NOI_SOFTWARE_CEFFECT_HPP

#include <memory>
#include "NSndConfig.hpp"
#include "CChainMember.hpp"

namespace NSnd {
    /**
     * Base class for all audio effects.
     */
    class CEffect : public CChainMember {
    public:
        /**
         * Process an audio data buffer. Used in RT thread.
         * @param buffer Buffer that will be processed
         * @param buffLen Length of the buffer in frames
         */
        virtual void ProcessBuffer(SND_DATA_TYPE *buffer, unsigned long buffLen) = 0;

        /**
         * Flushes all sound in internal effect buffers, clears all audio
         */
        virtual void FlushSound() = 0;

    };

    typedef std::shared_ptr<CEffect> AEffect;
}


#endif //NOI_SOFTWARE_CEFFECT_H
