//
// Created by ddl_blue on 13.5.19.
//

#ifndef NOI_SOFTWARE_CPLUGINFACTORY_HPP
#define NOI_SOFTWARE_CPLUGINFACTORY_HPP

#include "../include/snd/CInstrument.hpp"
#include "../include/snd/CEffect.hpp"

namespace NPlg {
    /**
     * Factory for plugins. Creates instruments and effects.
     */
    class CPluginFactory {
    public:

        /**
         * Create an instrument
         * @param id ID of the instrument
         * @return new instrument
         */
        static NSnd::AInstrument GetInstrument(uint32_t id);

        /**
         * Get effect
         * @param id ID of the effect
         * @return new effect
         */
        static NSnd::AEffect GetEffect(uint32_t id);


        /**
         * @return Number of available instruments
         */
        static uint32_t GetInstrumentNumber();

        /**
         * @return Number of available effects
         */
        static uint32_t GetEffectNumber();

    private:
    };
}

#endif //NOI_SOFTWARE_CPLUGINFACTORY_HPP
