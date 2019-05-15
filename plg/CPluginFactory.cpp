//
// Created by ddl_blue on 13.5.19.
//

#include "CPluginFactory.hpp"
#include "instr/SimpleOsc/CInstrSimpleOsc.hpp"
#include "eff/Filter/CEffFilter.hpp"
#include "eff/Echo/CEffEcho.hpp"

using namespace NPlg;

/*----------------------------------------------------------------------*/
NSnd::AInstrument CPluginFactory::GetInstrument(uint32_t id) {
    switch (id) {
        case 0:
            return std::make_shared<NPlg::NInstr::CInstrSimpleOsc>();

        default:
            NMsc::CLogger::Log(NMsc::ELogType::ERROR, "CPluginFactory: There is no instrument with number %", id);
            return nullptr;
    }
}

/*----------------------------------------------------------------------*/
uint32_t CPluginFactory::GetInstrumentNumber() {
    return 1;
}

/*----------------------------------------------------------------------*/
NSnd::AEffect CPluginFactory::GetEffect(uint32_t id) {
    switch (id) {
        case 0:
            return std::make_shared<NPlg::NEcho::CEffEcho>();
        case 1:
            return std::make_shared<NPlg::NFilter::CEffFilter>();

        default:
            NMsc::CLogger::Log(NMsc::ELogType::ERROR, "CPluginFactory: There is no instrument with number %", id);
            return nullptr;
    }
}

/*----------------------------------------------------------------------*/
uint32_t CPluginFactory::GetEffectNumber() {
    return 2;
}
