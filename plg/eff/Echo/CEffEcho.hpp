//
// Created by ddl_blue on 14.5.19.
//

#ifndef NOI_SOFTWARE_CEFFECHO_HPP
#define NOI_SOFTWARE_CEFFECHO_HPP

#include "../../../include/snd/CEffect.hpp"
#include "../../../include/snd/CLimiter.hpp"

namespace NPlg::NEcho {
    /**
     * Implemets reverb and delay effect
     */
    class CEffEcho : public NSnd::CEffect {
    public:
        CEffEcho();

        virtual void ProcessBuffer(SND_DATA_TYPE *buffer, unsigned long buffLen) override;

        virtual void FlushSound() override;


    private:


        /// Cursor in circle buffers
        uint32_t m_buffCursor;

        /// Size parameter
        double m_size;
        /// Delay size parameter
        double m_delaySize;
        /// Absorbtion parameter
        double m_absorbtion;
        /// Changes the ratio between reverb and delay
        double m_reverbPortion;

        // Buffer sizes
        const static uint32_t LONG_BUFF_EXPONENT = 16;
        const static uint32_t SHORT_BUFF_EXPONENT = 13;
        constexpr static uint32_t LONG_BUFF_LEN = 1 << LONG_BUFF_EXPONENT;
        constexpr static uint32_t SHORT_BUFF_LEN = 1 << SHORT_BUFF_EXPONENT;

        // Sound buffers for all the walls
        SND_DATA_TYPE m_buffAr[LONG_BUFF_LEN];
        SND_DATA_TYPE m_buffBr[SHORT_BUFF_LEN];
        SND_DATA_TYPE m_buffCr[SHORT_BUFF_LEN];

        SND_DATA_TYPE m_buffAl[LONG_BUFF_LEN];
        SND_DATA_TYPE m_buffBl[SHORT_BUFF_LEN];
        SND_DATA_TYPE m_buffCl[SHORT_BUFF_LEN];

        // NEW
        SND_DATA_TYPE m_buffLeft[LONG_BUFF_LEN];
        SND_DATA_TYPE m_buffRight[LONG_BUFF_LEN];
        NSnd::CLimiter m_limiter;

    };
}


#endif //NOI_SOFTWARE_CEFFECHO_HPP
