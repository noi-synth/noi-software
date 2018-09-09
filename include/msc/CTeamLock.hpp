//
// Created by ddl_blue on 26.8.18.
//

#ifndef NOI_SOFTWARE_CTEAMLOCK_HPP
#define NOI_SOFTWARE_CTEAMLOCK_HPP

#include <atomic>

namespace NMsc {
    // TODO check if this synchronization idea is not just a huge bullshit
    class CTeamLock {
    public:
        CTeamLock();

        bool TryLockBlue();

        bool TryLockRed();

//        int GetOwner();
        void Unlock();

//        void UnlockBlue();
//        void UnlockRed();
    private:
        std::atomic_int m_teamScore;
    };
}

#endif //NOI_SOFTWARE_CTEAMLOCK_HPP
