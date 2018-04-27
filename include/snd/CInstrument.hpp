//);
// Created by ddl_blue on 15.11.17.
//
#include <cstdint>
#include <memory>
#include <list>
#include "NSndConfig.hpp"
#include "../../include/msc/CLocklessQue.hpp"
#include "CMidiMsg.hpp"

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
        GenerateBuffer(const SND_DATA_TYPE *inputBuff, SND_DATA_TYPE *outputBuff, unsigned long buffLen) = 0;

        void ReciveMidiMsg(const CMidiMsg &msg);


    protected:

        std::list<CMidiMsg> m_newMidiMsg;

        virtual void Tick() = 0;

        uint32_t m_sambleFromTickCounter;

    };

    typedef std::shared_ptr<CInstrument> AInstrument;
}

#endif // NOI_SOFTWARE_CINSTRUMENT_HPP