////
//// Created by ddl_blue on 27.8.18.
////
//
//#ifndef NOI_SOFTWARE_CTEAM3LOCK_HPP
//#define NOI_SOFTWARE_CTEAM3LOCK_HPP
//
//#include <atomic>
//#include "CTeamLock.hpp"
//
//namespace NMsc {
//    // TODO check if this synchronization idea is not just a huge bullshit
//    class CTeam3Lock {
//    public:
//        bool TryLockBlue();
//        bool TryLockRed();
//        bool TryLockGreen();
//        void Unlock();
//    private:
//        CTeamLock m_blueRedLock;
//        CTeamLock m_blueGreenLock;
//        CTeamLock m_greenRedLock;
//    };
//}
//#endif //NOI_SOFTWARE_CTEAM3LOCK_HPP
