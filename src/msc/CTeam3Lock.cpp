////
//// Created by ddl_blue on 27.8.18.
////
//
//#include <iostream>
//#include "../../include/msc/CTeam3Lock.hpp"
//
//using namespace NMsc;
//
///*----------------------------------------------------------------------*/
//bool CTeam3Lock::TryLockBlue() {
//
//    int pl1 = m_blueRedLock.GetOwner();
//    int pl2 = m_blueGreenLock.GetOwner();
//
//    bool l1 = m_blueRedLock.TryLockBlue();
//    bool l2 = m_blueGreenLock.TryLockBlue();
//
////    l1 &= m_blueRedLock.TryLockBlue();
////    l2 &= m_blueGreenLock.TryLockBlue();
//
//    /*if (pl1 && l1)
//        std::cerr << "WTF vole" << std::endl;
//
//    if (pl2 && l2)
//        std::cerr << "WTF vole2" << std::endl;*/
//
//    if (l1 && l2) {
//        /*if (m_greenRedLock.GetOwner())
//            std::cerr << "WTF man" << std::endl;*/
//
//        if (m_blueGreenLock.GetOwner()<=0 || m_blueRedLock.GetOwner() <= 0)
//            std::cerr << "WTF man2" << std::endl;
//
//        return true;
//    }
//
//    m_blueRedLock.UnlockBlue();
//    m_blueGreenLock.UnlockBlue();
//    /*if (l1)
//        m_blueRedLock.Unlock();
//    if (l2)
//        m_blueGreenLock.Unlock();*/
//
//    return false;
//}
//
///*----------------------------------------------------------------------*/
//bool CTeam3Lock::TryLockRed() {
//    bool l1 = m_blueRedLock.TryLockRed();
//    bool l2 = m_greenRedLock.TryLockRed();
//
////    l1 &= m_blueRedLock.TryLockRed();
////    l2 &= m_greenRedLock.TryLockRed();
//
//    if (l1 && l2)
//        return true;
//
//    m_blueRedLock.UnlockRed();
//    m_greenRedLock.UnlockRed();
//    /*if (l1)
//        m_blueRedLock.Unlock();
//    if (l2)
//        m_greenRedLock.Unlock();*/
//
//    return false;
//}
//
///*----------------------------------------------------------------------*/
//bool CTeam3Lock::TryLockGreen() {
//    bool l1 = m_blueGreenLock.TryLockRed();
//    bool l2 = m_greenRedLock.TryLockBlue();
//
////    l1 &= m_blueGreenLock.TryLockRed();
////    l2 &= m_greenRedLock.TryLockBlue();
//
//    if (l1 && l2)
//        return true;
//
//
//    m_blueGreenLock.UnlockRed();
//    m_greenRedLock.UnlockBlue();
//
//    /*if (l1)
//        m_blueGreenLock.Unlock();
//    if (l2)
//        m_greenRedLock.Unlock();*/
//
//    return false;
//}
//
///*----------------------------------------------------------------------*/
//void CTeam3Lock::Unlock() {
//    m_blueGreenLock.Unlock();
//    m_blueRedLock.Unlock();
//    m_greenRedLock.Unlock();
//}
