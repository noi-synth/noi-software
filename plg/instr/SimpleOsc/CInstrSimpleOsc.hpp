//
// Created by ddl_blue on 8.4.18.
//

#ifndef NOI_SOFTWARE_CINSTRSIMPLEOSC_HPP
#define NOI_SOFTWARE_CINSTRSIMPLEOSC_HPP

#include "../../../include/snd/CInstrument.hpp"
#include "../../../include/snd/NSndConfig.hpp"

namespace NPlg {
    namespace NInstr {
        //TODO plugin logic
        //TODO implement ticks and everything
        class CInstrSimpleOsc : public NSnd::CInstrument {
        public:
            CInstrSimpleOsc();

            virtual int
            GenerateBuffer(const SND_DATA_TYPE *inputBuff, SND_DATA_TYPE *outputBuff, unsigned long buffLen) override;

        protected:
            virtual void Tick() override;

            uint32_t m_phase;
            bool m_on;
        };
    }
}


#endif //NOI_SOFTWARE_CINSTRSIMPLEOSC_HPP
