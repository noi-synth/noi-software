//
// Created by ddl_blue on 13.5.19.
//

#ifndef NOI_SOFTWARE_CPLUGINFACTORY_HPP
#define NOI_SOFTWARE_CPLUGINFACTORY_HPP

#include "../include/snd/CInstrument.hpp"
#include "../include/snd/CEffect.hpp"

namespace NPlg {
    class CPluginFactory {
    public:

        static NSnd::AInstrument GetInstrument(uint32_t id);

        static NSnd::AEffect GetEffect(uint32_t id);


        static uint32_t GetInstrumentNumber();

        static uint32_t GetEffectNumber();

    private:
    };
}

#endif //NOI_SOFTWARE_CPLUGINFACTORY_HPP
