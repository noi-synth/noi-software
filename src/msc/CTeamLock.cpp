//
// Created by ddl_blue on 26.8.18.
//

#include "../../include/msc/CTeamLock.hpp"

using namespace NMsc;

static const int SCORE_GOAL = 10;

/*----------------------------------------------------------------------*/
CTeamLock::CTeamLock() : m_teamScore(0) {}

/*----------------------------------------------------------------------*/
bool CTeamLock::TryLockBlue() {
    while (m_teamScore < SCORE_GOAL) {
        // Other team holds the lock or is trying to lock it
        if (m_teamScore < 0)
            return false;
        // One step closer to get the lock
        ++m_teamScore;
    }
    //m_teamScore = 1000000;
    return true;
}

/*----------------------------------------------------------------------*/
bool CTeamLock::TryLockRed() {
    while (m_teamScore > -SCORE_GOAL) {
        // Other team holds the lock or is trying to lock it
        if (m_teamScore > 0)
            return false;
        // One step closer to get the lock
        --m_teamScore;
    }
    //m_teamScore = -1000000;
    return true;
}

///*----------------------------------------------------------------------*/
//int CTeamLock::GetOwner(){
//    if (m_teamScore>SCORE_GOAL-3)
//        return 1;
//    if (m_teamScore<-SCORE_GOAL-3)
//        return 1;
//    return 0;
//}

/*----------------------------------------------------------------------*/
void CTeamLock::Unlock() {
    m_teamScore = 0;
}

///*----------------------------------------------------------------------*/
//void CTeamLock::UnlockBlue() {
//    if (m_teamScore>0)
//        m_teamScore = 0;
//}
//
///*----------------------------------------------------------------------*/
//void CTeamLock::UnlockRed() {
//    if (m_teamScore<0)
//        m_teamScore = 0;
//}