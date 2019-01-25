//
// Created by ddl_blue on 28.10.18.
//

#include "CParameter.hpp"
#include "../msc/CLogger.hpp"

using namespace NLgc;

/*----------------------------------------------------------------------*/

CParameter::CParameter(uint32_t value) : m_type(EType::INT) {
    m_value.m_int = value;
}

CParameter::CParameter(float value) : m_type(EType::FLOAT) {
    m_value.m_float = value;
}

CParameter::CParameter(bool value) : m_type(EType::BOOL) {
    m_value.m_bool = value;
}

CParameter::CParameter(std::string value) : m_type(EType::STRING) {
    m_value.m_string = value;
}

/*----------------------------------------------------------------------*/
CParameter::EType CParameter::GetType() {
    return m_type;
}

/*----------------------------------------------------------------------*/
void CParameter::SetValue(uint32_t value) {
    if (m_type == EType::INT)
        m_value.m_int = value;
    else
        NMsc::CLogger::Log(NMsc::ELogType::ERROR, "CParemeter: SetValue: Type mismatch, setting int (%) to %.",
                           EType::INT, m_type);
}

/*----------------------------------------------------------------------*/
void CParameter::SetValue(float value) {
    if (m_type == EType::FLOAT)
        m_value.m_float = value;
    else
        NMsc::CLogger::Log(NMsc::ELogType::ERROR, "CParemeter: SetValue: Type mismatch, setting float (%) to %.",
                           EType::FLOAT, m_type);
}

/*----------------------------------------------------------------------*/
void CParameter::SetValue(bool value) {
    if (m_type == EType::BOOL)
        m_value.m_bool = value;
    else
        NMsc::CLogger::Log(NMsc::ELogType::ERROR, "CParemeter: SetValue: Type mismatch, setting bool (%) to %.",
                           EType::BOOL, m_type);
}

/*----------------------------------------------------------------------*/
void CParameter::SetValue(std::string value) {
    if (m_type == EType::STRING)
        m_value.m_string = value;
    else
        NMsc::CLogger::Log(NMsc::ELogType::ERROR, "CParemeter: SetValue: Type mismatch, setting string (%) to %.",
                           EType::STRING, m_type);
}


/*----------------------------------------------------------------------*/
int32_t CParameter::GetIntValue() {
    if (m_type == EType::INT)
        return m_value.m_int;
    else
        NMsc::CLogger::Log(NMsc::ELogType::ERROR, "CParemeter: GetIntValue: Type mismatch, getting int (%) from %.",
                           EType::INT, m_type);

    return 0;
}

/*----------------------------------------------------------------------*/
float CParameter::GetFloatValue() {
    if (m_type == EType::FLOAT)
        return m_value.m_float;
    else
        NMsc::CLogger::Log(NMsc::ELogType::ERROR, "CParemeter: GetFloatValue: Type mismatch, getting float (%) from %.",
                           EType::FLOAT, m_type);

    return .0;
}

/*----------------------------------------------------------------------*/
bool CParameter::GetBoolValue() {
    if (m_type == EType::BOOL)
        return m_value.m_bool;
    else
        NMsc::CLogger::Log(NMsc::ELogType::ERROR, "CParemeter: GetBoolValue: Type mismatch, getting bool (%) from %.",
                           EType::BOOL, m_type);

    return false;
}

/*----------------------------------------------------------------------*/
std::string CParameter::GetStringValue() {
    if (m_type == EType::STRING)
        return m_value.m_string;
    else
        NMsc::CLogger::Log(NMsc::ELogType::ERROR,
                           "CParemeter: GetStringValue: Type mismatch, getting string (%) from %.", EType::STRING,
                           m_type);

    return "CPARAM ERROR!";
}