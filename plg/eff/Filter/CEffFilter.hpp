//
// Created by ddl_blue on 13.5.19.
//

#ifndef NOI_SOFTWARE_CEFFFILTER_HPP
#define NOI_SOFTWARE_CEFFFILTER_HPP


#include <cstdint>
#include "../../../include/snd/NSndConfig.hpp"
#include "../../../include/snd/CEffect.hpp"

namespace NPlg::NFilter {
    class CEffFilter : public NSnd::CEffect {
    public:
        CEffFilter();

        virtual void ProcessBuffer(SND_DATA_TYPE *buffer, unsigned long buffLen) override;

        virtual void FlushSound() override;

    private:
        double m_cutoffLP;
        double m_sumLpLeft;
        double m_sumLpRight;

        uint32_t m_avgPosLP;
        uint32_t m_avgLenLP;

        const static uint32_t BUFF_LEN_EXP = 10;
        constexpr static uint32_t BUFF_LEN = 1 << BUFF_LEN_EXP;

        SND_DATA_TYPE m_lpBuffLeft[BUFF_LEN];
        SND_DATA_TYPE m_lpBuffRight[BUFF_LEN];

    };
}


#endif //NOI_SOFTWARE_CEFFFILTER_HPP
