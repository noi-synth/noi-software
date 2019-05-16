//
// Created by ddl_blue on 13.5.19.
//

#ifndef NOI_SOFTWARE_CEFFFILTER_HPP
#define NOI_SOFTWARE_CEFFFILTER_HPP


#include <cstdint>
#include "../../../include/snd/NSndConfig.hpp"
#include "../../../include/snd/CEffect.hpp"

namespace NPlg::NFilter {
    /**
     * Implements an audio filter
     */
    class CEffFilter : public NSnd::CEffect {
    public:
        CEffFilter();

        virtual void ProcessBuffer(SND_DATA_TYPE *buffer, unsigned long buffLen) override;

        virtual void FlushSound() override;

    private:
        /// Cutoff parameter
        double m_cutoffLP;

        /// Sum of samples in left channel
        double m_sumLpLeft;

        /// Sum of samples in right channel
        double m_sumLpRight;

        /// Position of averging cursor
        uint32_t m_avgPosLP;

        /// Length of avg region
        uint32_t m_avgLenLP;

        // Buffer size
        const static uint32_t BUFF_LEN_EXP = 10;
        constexpr static uint32_t BUFF_LEN = 1 << BUFF_LEN_EXP;

        // Audio buffers
        SND_DATA_TYPE m_lpBuffLeft[BUFF_LEN];
        SND_DATA_TYPE m_lpBuffRight[BUFF_LEN];

    };
}


#endif //NOI_SOFTWARE_CEFFFILTER_HPP
