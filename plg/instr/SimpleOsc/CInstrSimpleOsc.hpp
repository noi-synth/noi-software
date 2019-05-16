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
        /**
         * Oscillator instrument
         */
        class CInstrSimpleOsc : public NSnd::CInstrument {
        public:
            CInstrSimpleOsc();


        protected:

            bool m_on;

            class CSimpleOscVoice : public CInstrument::CInstrumentVoice {
            public:
                CSimpleOscVoice(CInstrument &instrument, NSnd::ETones tone);

                int GenerateBuffer(const SND_DATA_TYPE *inputBuff, SND_DATA_TYPE *outputBuff,
                                   unsigned long buffLen) override;

                void Activate(NSnd::CMidiMsg midiMessage) override;

            protected:

                /// Phase of the oscillation
                double m_phase;

                /// Step
                double m_sampleStep;

                /// true if wave tables were initialized
                static bool waveTablesInitialized;

                /// Length of wave tables
                static const int WAVETABLE_LEN = 512;

                /// Sin wave table
                static float SIN_WAVE[WAVETABLE_LEN];
            };


        };
    }
}


#endif //NOI_SOFTWARE_CINSTRSIMPLEOSC_HPP
