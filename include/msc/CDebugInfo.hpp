//
// Created by ddl_blue on 17.9.18.
//

#ifndef NOI_SOFTWARE_CDEBUGINFO_HPP
#define NOI_SOFTWARE_CDEBUGINFO_HPP

#define DEBUG

#include <atomic>

namespace NMsc {
    /**
     * Holds temporaly globally some random info. For debug purposes only. Not documented, not an actual part of the app.
     */
    class CDebugInfo {
    public:
        static std::atomic_int m_lastAudioBufferLen;
        static float m_sndLastOutput;
        static float m_sndLastChainOutput;
        static float m_sndLastTrackOutput;
        static float m_sndLastTrackManagerOutput;
        static char m_sndLastTrackCall[7];
        static int m_sndTrackSize;
        static int m_sndPositionDisplacement;
    };
}
#endif //NOI_SOFTWARE_CDEBUGINFO_HPP
