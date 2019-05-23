//
// Created by ddl_blue on 13.5.19.
//

#include <cmath>
#include "CEffFilter.hpp"
#include "CChainMember.hpp"

using namespace NPlg::NFilter;

/*----------------------------------------------------------------------*/
CEffFilter::CEffFilter() : m_cutoffLP(0.5), m_sumLpLeft(0), m_sumLpRight(0), m_avgLenLP(0), m_avgPosLP(0) {
    for (uint32_t i = 0; i < BUFF_LEN; ++i) {
        m_lpBuffLeft[i] = m_lpBuffRight[i] = 0;
    }
    m_avgLenLP = (uint32_t) powf(1000, m_cutoffLP);
}

/*----------------------------------------------------------------------*/
void CEffFilter::ProcessBuffer(SND_DATA_TYPE *buffer, unsigned long buffLen) {

    bool lpCutoffChanged = false;

    while (!m_paramChanges.Empty()) {
        CParamScroll scroll = m_paramChanges.Pop();
        if (scroll.m_paramId == 0) {
            if (scroll.m_directionUp && m_cutoffLP < 0.9) {
                m_cutoffLP -= 0.025;
                lpCutoffChanged = true;
            } else if (!scroll.m_directionUp && m_cutoffLP > 0.1) {
                m_cutoffLP += 0.025;
                lpCutoffChanged = true;
            }
        }
    }

    // Recalc stuff on parameter change
    if (lpCutoffChanged || !std::isnormal(m_sumLpLeft) || !std::isnormal(m_sumLpRight)) {
        m_avgLenLP = (uint32_t) powf(1000, m_cutoffLP);

        m_sumLpRight = m_sumLpLeft = 0;

        for (uint32_t i = 0; i < m_avgLenLP; ++i) {
            m_sumLpLeft += m_lpBuffLeft[(m_avgPosLP + i) & (BUFF_LEN - 1)];
            m_sumLpRight += m_lpBuffRight[(m_avgPosLP + i) & (BUFF_LEN - 1)];
        }
    }


    for (uint32_t i = 0; i < buffLen; ++i) {

        // Get rid of old values
        m_sumLpLeft -= m_lpBuffLeft[(m_avgPosLP - m_avgLenLP) & (BUFF_LEN - 1)];
        m_sumLpRight -= m_lpBuffRight[(m_avgPosLP - m_avgLenLP) & (BUFF_LEN - 1)];

        // Count and save new values
        m_sumLpLeft += m_lpBuffLeft[m_avgPosLP] = buffer[i << 1];
        m_sumLpRight += m_lpBuffRight[m_avgPosLP] = buffer[(i << 1) + 1];

        // Move cursor
        m_avgPosLP = (m_avgPosLP + 1) & (BUFF_LEN - 1);

        // Get new values
        buffer[i << 1] = buffer[i << 1] - m_sumLpLeft / m_avgLenLP;
        buffer[(i << 1) + 1] = buffer[(i << 1) + 1] - m_sumLpRight / m_avgLenLP;
    }

    m_sumLpRight *= 0.999;
    m_sumLpLeft *= 0.999;
}

/*----------------------------------------------------------------------*/
void CEffFilter::FlushSound() {
    m_sumLpLeft = m_sumLpRight = 0;
    for (uint32_t i = 0; i < BUFF_LEN; ++i) {
        m_lpBuffLeft[i] = m_lpBuffRight[i] = 0;
    }
}
















