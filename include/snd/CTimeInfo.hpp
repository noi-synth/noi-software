//
// Created by ddl_blue on 26.5.19.
//

#ifndef NOI_SOFTWARE_CTIMEINFO_HPP
#define NOI_SOFTWARE_CTIMEINFO_HPP

#include <cstdint>
// #include <atomic> // todo atomic values?

namespace NSnd {
    class CTimeInfo {
    public:

        CTimeInfo(uint64_t globalTime, uint64_t trackTime, uint32_t bpm = 120);

        void AddTime(uint64_t samples);

        uint64_t GetTickNumber();

        uint64_t m_globalTime;
        uint64_t m_trackTime;
        uint32_t m_bpm;
    };
}

#endif //NOI_SOFTWARE_CTIMEINFO_HPP
