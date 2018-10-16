//
// Created by ddl_blue on 17.9.18.
//

#include "../../include/msc/CDebugInfo.hpp"

using namespace NMsc;

std::atomic_int NMsc::CDebugInfo::m_lastAudioBufferLen;
float NMsc::CDebugInfo::m_sndLastOutput;
float NMsc::CDebugInfo::m_sndLastChainOutput;
float NMsc::CDebugInfo::m_sndLastTrackOutput;
float NMsc::CDebugInfo::m_sndLastTrackManagerOutput;
char NMsc::CDebugInfo::m_sndLastTrackCall[] = "-----\0";
int NMsc::CDebugInfo::m_sndTrackSize;
int NMsc::CDebugInfo::m_sndPositionDisplacement;