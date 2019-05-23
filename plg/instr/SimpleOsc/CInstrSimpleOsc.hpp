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

            /// Ratio of both oscillator volume
            double m_oscillatorRatio;

            /// Phase difference between oscillators
            double m_phaseShift;

            /// Index of first oscillator
            uint32_t m_firstOscIndex;

            /// Index of second oscillator
            uint32_t m_secondOscIndex;

            /// Used so some parameters are not being changed each knob step
            uint32_t m_paramChangeDelay;

            class CSimpleOscVoice : public CInstrument::CInstrumentVoice {
            public:
                CSimpleOscVoice(CInstrSimpleOsc &instrument, NSnd::ETones tone);

                int GenerateBuffer(const SND_DATA_TYPE *inputBuff, SND_DATA_TYPE *outputBuff,
                                   unsigned long buffLen) override;

                void Activate(NSnd::CMidiMsg midiMessage) override;

            protected:
                // todo initialize all the values in constructor

                /// Phase of the oscillation
                double m_phase;

                /// Instrument that owns this voice
                CInstrSimpleOsc &m_owner;

                /// Step
                double m_sampleStep;

                /// true if wave tables were initialized
                static bool waveTablesInitialized;

                /// Length of wave tables
                static const int WAVETABLE_LEN = 512;

                /// Sin wave table
                static float SIN_WAVE[WAVETABLE_LEN];

                /// Triangle wave table
                static float TRIANGLE_WAVE[WAVETABLE_LEN];

                /// Saw wave table
                static float SAW_WAVE[WAVETABLE_LEN];

                /// Square 50% wave table
                static float SQUARE50_WAVE[WAVETABLE_LEN];

                /// Square 25%table
                static float SQUARE25_WAVE[WAVETABLE_LEN];

                /// Square 10%table
                static float SQUARE10_WAVE[WAVETABLE_LEN];

                /// Number of waves
                static const uint32_t WAVE_CNT = 6;
            };


        };
    }
}


#endif //NOI_SOFTWARE_CINSTRSIMPLEOSC_HPP
