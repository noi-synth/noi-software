//
// Created by ddl_blue on 9.5.19.
//


#include "../../../include/ui/zero/CNoiZeroCommunicator.hpp"


using namespace NUi::NZero;

/*----------------------------------------------------------------------*/
CNoiZeroCommunicator *CNoiZeroCommunicator::m_instance;

/*----------------------------------------------------------------------*/
CNoiZeroCommunicator::CNoiZeroCommunicator() {
    if (m_instance)
        NMsc::CLogger::Log(NMsc::ELogType::NOTE, "CNoiZeroCommunicator: Replacing instance from % to %.", m_instance,
                           this);

    m_instance = this;
}

/*----------------------------------------------------------------------*/
CNoiZeroCommunicator::~CNoiZeroCommunicator() {
    if (m_instance)
        NMsc::CLogger::Log(NMsc::ELogType::NOTE, "CNoiZeroCommunicator: Deleting instance %.", this);
    m_instance = nullptr;
}

/*----------------------------------------------------------------------*/
CNoiZeroCommunicator *CNoiZeroCommunicator::GetInstance() {
    return m_instance;
}
