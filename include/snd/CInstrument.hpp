//);
// Created by ddl_blue on 15.11.17.
//
#ifndef NOI_SOFTWARE_CINSTRUMENT_HPP
#define NOI_SOFTWARE_CINSTRUMENT_HPP

#include <atomic>
#include <cstdint>
#include <memory>
#include <list>
#include <vector>

namespace NSnd {
    class CInstrument;

    typedef std::shared_ptr<CInstrument> AInstrument;
}

#include "NSndConfig.hpp"
#include "../msc/CLocklessQue.hpp"
#include "CMidiMsg.hpp"
#include "CChainMember.hpp"
#include "CMidiProcessor.hpp"
#include "CTimeInfo.hpp"
#include "CAdsrSettings.hpp"
#include "CAdsr.hpp"
//#include "../lgc/CParameter.hpp"



using NMsc::CLocklessQue;

namespace NSnd {

    /**
     * Base class for all instruments
     */
    class CInstrument : public CChainMember {
    public:

        //TODO implement TICK logic
        //TODO MIDI msg helpers
        //TODO parameter logic

        CInstrument();

        /**
         * Generate buffer of audio output
         * @param inputBuff Buffer with audio input from audio device
         * @param outputBuff Buffer to fill produced audio to
         * @param buffLen Length of buffers in frames.
         * @return 0 if succesfull
         */
        virtual int
        GenerateBuffer(const SND_DATA_TYPE *inputBuff, SND_DATA_TYPE *outputBuff, unsigned long buffLen,
                       const CTimeInfo &timeInfo);

        /**
         * Send midi message into this instrument.
         * @param msg Midi message
         * @param midiProcessed Midi True if message already went through midi processor. Must be true for calls from MIDI processors.
         */
        void ReceiveMidiMsg(const CMidiMsg &msg, bool midiProcessed);

        /**
         * Apply different midi processor
         * @param processor New midi processor. If nullptr, no processor will be used.
         */
        void ApplyMidiProcessor(AMidiProcessor &processor);

        AAdsrSettings GetAdsr();

    protected:


        virtual void Tick(const CTimeInfo &timeInfo);

        virtual void AsyncTick();

        virtual void ProcessInputChanges();

        /**
         * One polyphony voice
         */
        class CInstrumentVoice {
        public:
            /**
             * Constructor
             * @param instrument Instrument it belongs to
             * @param tone it is playing
             */
            CInstrumentVoice(CInstrument &instrument, ETones tone);

            /**
             * Generate sound.
             * @param inputBuff Sound from device input
             * @param outputBuff Buffer where it outputs the sound
             * @param buffLen Length of buffers in frames
             * @return 0 on success
             */
            virtual int
            GenerateBuffer(const SND_DATA_TYPE *inputBuff, SND_DATA_TYPE *outputBuff, unsigned long buffLen,
                           const CTimeInfo &timeInfo) = 0;

            /**
             * Stop this voice
             */
            virtual void Deactivate();

            /**
             * Activate this voice with midi message
             * @param midiMessage Instructions. What shoud the voice play?
             */
            virtual void Activate(CMidiMsg midiMessage);

            /**
             * Is this voice active?
             * @return True if active
             */
            bool IsActive();

            void Tick();

        protected:

            /* /// Is this voice active?
             bool m_active;*/

            /// Parent instrument
            CInstrument &m_instrument;

            /// Tone the voice is playing
            ETones m_tone;

            AAdsr m_adsr;

        };

        typedef std::shared_ptr<CInstrumentVoice> AInstrumentVoice;

        //NMsc::CLocklessQue<NLgc::CParameter> m_newParameters;

        /// Notes to be played
        NMsc::CLocklessQue<CMidiMsg> m_newMidiMsg; //todo is list a good idea?

        /// Number of samples from last tick
        uint32_t m_sambleFromTickCounter;

        /// Voices in this instrument
        std::vector<std::pair<ETones, AInstrumentVoice >> m_voices;

        /// How tones should be played
        EInstrumentMode m_mode;

        /// Temporary buffer, used for voice output mixing
        SND_DATA_TYPE m_tmpBuffer[INTERNAL_BUFFERS_LEN]; // todo get rid of that

        /// Midi processor in the instrument
        AMidiProcessor m_midiProcessor;
        NMsc::CLocklessQue<AMidiProcessor> m_newMidiProcessor;

        AAdsrSettings m_adsrInfo;
    };
}

#endif // NOI_SOFTWARE_CINSTRUMENT_HPP