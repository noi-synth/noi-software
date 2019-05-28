//
// Created by ddl_blue on 26.5.19.
//

#include "../../include/snd/CTimeInfo.hpp"
#include "../../include/snd/NSndConfig.hpp"

using namespace NSnd;

/*----------------------------------------------------------------------*/
CTimeInfo::CTimeInfo(uint64_t globalTime, uint64_t trackTime, uint32_t bpm) : m_globalTime(globalTime),
                                                                              m_trackTime(trackTime), m_bpm(bpm) {}

/*----------------------------------------------------------------------*/
void CTimeInfo::AddTime(uint64_t samples) {
    m_trackTime += samples;
    m_globalTime += samples;
}

/*----------------------------------------------------------------------*/
uint64_t CTimeInfo::GetTickNumber() {
    return m_trackTime / TICK_LENGTH;
}