//
// Created by ddl_blue on 26.8.18.
//

#ifndef NOI_SOFTWARE_CTEAMLOCK_HPP
#define NOI_SOFTWARE_CTEAMLOCK_HPP

#include <atomic>

namespace NMsc {
    // TODO check if this synchronization idea is not just a huge bullshit
    /**
     * Nonblocking synchronisation mechanism. Ment to be used by two threads. One is "Team Red", the other one is "Team Blue".
     * Only one team can hold the lock at any given time. Does not use any OS calls.
     */
    class CTeamLock {
    public:
        CTeamLock();

        /**
         * Try to lock the lock for the Blue Team.
         * @return True if successful
         */
        bool TryLockBlue();

        /**
         * Try to lock the lock for the Red Team.
         * @return True if successful
         */
        bool TryLockRed();

//        int GetOwner();
        /**
         * Unlock the lock
         */
        void Unlock();

//        void UnlockBlue();
//        void UnlockRed();
    private:
        /**
         * Counts the score for each team. Used to tell who holds the lock.
         */
        std::atomic_int m_teamScore;
    };
}

#endif //NOI_SOFTWARE_CTEAMLOCK_HPP
