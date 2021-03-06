//);
// Created by ddl_blue on 15.11.17.
//
#include <cstdint>
#include <memory>
#include <list>
#include <vector>
#include "NSndConfig.hpp"
#include "../msc/CLocklessQue.hpp"
#include "CMidiMsg.hpp"
#include "CChainMember.hpp"
//#include "../lgc/CParameter.hpp"

#ifndef NOI_SOFTWARE_CINSTRUMENT_HPP
#define NOI_SOFTWARE_CINSTRUMENT_HPP

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
        GenerateBuffer(const SND_DATA_TYPE *inputBuff, SND_DATA_TYPE *outputBuff, unsigned long buffLen);

        /**
         * Send midi message into this instrument
         * @param msg Midi message
         */
        void ReciveMidiMsg(const CMidiMsg &msg);

        //void ReciveParameter(const NLgc::CParameter &parameter);


    protected:


        virtual void Tick();

        virtual void AsyncTick();

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
            GenerateBuffer(const SND_DATA_TYPE *inputBuff, SND_DATA_TYPE *outputBuff, unsigned long buffLen) = 0;

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

        protected:
            /// Is this voice active?
            bool m_active;
            /// Parent instrument
            CInstrument &m_instrument;
            /// Tone the voice is playing
            ETones m_tone;
        };

        typedef std::shared_ptr<CInstrumentVoice> AInstrumentVoice;

        //NMsc::CLocklessQue<NLgc::CParameter> m_newParameters;

        /// Notes to be played
        std::list<CMidiMsg> m_newMidiMsg; //todo is list a good idea?
        /// Number of samples from last tick
        uint32_t m_sambleFromTickCounter;
        /// Voices in this instrument
        std::vector<std::pair<ETones, AInstrumentVoice >> m_voices;
        /// How tones should be played
        EInstrumentMode m_mode;
        /// Temporary buffer, used for voice output mixing
        SND_DATA_TYPE m_tmpBuffer[INTERNAL_BUFFERS_LEN]; // todo get rid of that



    };

    typedef std::shared_ptr<CInstrument> AInstrument;
}

#endif // NOI_SOFTWARE_CINSTRUMENT_HPP