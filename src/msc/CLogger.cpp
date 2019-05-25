//
// Created by ddl_blue on 7.4.18.
//

#include "../../include/msc/CLogger.hpp"

using namespace NMsc;

std::list<std::function<void(const std::string &)>> CLogger::m_messageTargets;

/*----------------------------------------------------------------------*/
void CLogger::ClearMessageTargets() {
    m_messageTargets.clear();
}

/*----------------------------------------------------------------------*/
void CLogger::AddMessageTarget(std::function<void(const std::string &)> target) {
    m_messageTargets.push_back(target);
}