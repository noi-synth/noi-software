//
// Created by ddl_blue on 17.9.18.
//

#ifndef NOI_SOFTWARE_CDEBUGINFO_HPP
#define NOI_SOFTWARE_CDEBUGINFO_HPP

#define DEBUG

#include <atomic>

namespace NMsc {
    class CDebugInfo {
    public:
        static std::atomic_int m_lastAudioBufferLen;

    };
}
#endif //NOI_SOFTWARE_CDEBUGINFO_HPP
