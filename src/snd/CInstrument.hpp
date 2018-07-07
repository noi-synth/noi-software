//);
// Created by ddl_blue on 15.11.17.
//
#include <cstdint>
#include <memory>
#include <list>
#include <vector>
#include "../../include/snd/NSndConfig.hpp"
#include "../../include/msc/CLocklessQue.hpp"
#include "../../include/snd/CMidiMsg.hpp"

#ifndef NOI_SOFTWARE_CINSTRUMENT_HPP
#define NOI_SOFTWARE_CINSTRUMENT_HPP

using NMsc::CLocklessQue;

namespace NSnd {
    class CInstrument {
    public:

        //TODO implement TICK logic
        //TODO MIDI msg helpers
        //TODO parameter logic

        CInstrument();

        virtual int
        GenerateBuffer(const SND_DATA_TYPE *inputBuff, SND_DATA_TYPE *outputBuff, unsigned long buffLen);

        void ReciveMidiMsg(const CMidiMsg &msg);


    protected:


        virtual void Tick();

        virtual void AsyncTick();

        class CInstrumentVoice {
        public:
            CInstrumentVoice(CInstrument &instrument, ETones tone);

            virtual int
            GenerateBuffer(const SND_DATA_TYPE *inputBuff, SND_DATA_TYPE *outputBuff, unsigned long buffLen) = 0;

            virtual void Deactivate();

            virtual void Activate(CMidiMsg midiMessage);

            bool IsActive();

        protected:
            bool m_active;
            CInstrument &m_instrument;
            ETones m_tone;
        };

        typedef std::shared_ptr<CInstrumentVoice> AInstrumentVoice;

        std::list<CMidiMsg> m_newMidiMsg; //todo is list a good idea?
        uint32_t m_sambleFromTickCounter;
        std::vector<std::pair<ETones, AInstrumentVoice >> m_voices;
        EInstrumentMode m_mode;
        SND_DATA_TYPE m_tmpBuffer[INTERNAL_BUFFERS_LEN];



    };

    typedef std::shared_ptr<CInstrument> AInstrument;
}

#endif // NOI_SOFTWARE_CINSTRUMENT_HPP