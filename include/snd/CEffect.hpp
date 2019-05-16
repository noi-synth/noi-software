//
// Created by ddl_blue on 26.12.17.
//

#ifndef NOI_SOFTWARE_CEFFECT_HPP
#define NOI_SOFTWARE_CEFFECT_HPP

#include <memory>
#include "NSndConfig.hpp"
#include "../msc/CLocklessQue.hpp"

namespace NSnd {
    /**
     * Base class for all audio effects.
     */
    class CEffect {
    public:
        /**
         * Process an audio data buffer. Used in RT thread.
         * @param buffer Buffer that will be processed
         * @param buffLen Length of the buffer in frames
         */
        virtual void ProcessBuffer(SND_DATA_TYPE *buffer, unsigned long buffLen) = 0;

        /**
         * Changes a parameter value
         * @param paramId ID of the parameter
         * @param directionUp Direction of change
         * @param shift Is shift pressed?
         */
        virtual void ScrollParameter(uint32_t paramId, bool directionUp, bool shift);

        /**
         * Flushes all sound in internal effect buffers, clears all audio
         */
        virtual void FlushSound() = 0;

    protected:
        /**
         * Represents info about parameter change
         */
        struct CParamScroll {
            CParamScroll() = default;

            /**
             * Constructir
             * @param paramId ID of the parameter
             * @param directionUp Direction of change
             * @param shift Is shift pressed?
             */
            CParamScroll(uint32_t paramId, bool directionUp, bool shift) : m_paramId(paramId),
                                                                           m_directionUp(directionUp), m_shift(shift) {}

            /// paramId ID of the parameter
            uint32_t m_paramId;
            /// directionUp Direction of change
            bool m_directionUp;
            /// shift Is shift pressed?
            bool m_shift;
        };

        /// Changes of the parameters. Will be applied in RT thread.
        NMsc::CLocklessQue<CParamScroll> m_paramChanges;

    };

    typedef std::shared_ptr<CEffect> AEffect;
}


#endif //NOI_SOFTWARE_CEFFECT_H
