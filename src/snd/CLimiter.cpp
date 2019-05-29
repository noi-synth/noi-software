//
// Created by ddl_blue on 29.5.19.
//

#include <cmath>
#include "../../include/snd/CLimiter.hpp"
#include "../../include/msc/Common.hpp"

#include "../../include/msc/CLogger.hpp"

using namespace NSnd;

/*----------------------------------------------------------------------*/
CLimiter::CLimiter(double release) : m_volume(1), m_release(release) {}

/*----------------------------------------------------------------------*/
void CLimiter::ProcessBuffer(SND_DATA_TYPE *buffer, uint32_t len) {
    for (uint32_t i = 0; i < len; ++i) {
        // L
        if (*buffer != 0)
            m_volume = MIN(0.99, MIN(m_volume, std::fabs(0.95 / (*buffer))));
        *buffer = *buffer * m_volume;
        ++buffer;

        // R
        if (*buffer != 0)
            m_volume = MIN(0.99, MIN(m_volume, std::fabs(0.95 / (*buffer))));
        *buffer = *buffer * m_volume;
        ++buffer;

        // release
        m_volume = MIN(0.99, m_volume * (1 + (1 / m_release / SAMPLE_RATE * 500)));
    }

    NMsc::CLogger::Log(NMsc::ELogType::TMP_DEBUG, "Volume = %", m_volume);
}