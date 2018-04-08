//
// Created by ddl_blue on 15.11.17.
//
#include <cstdint>
#include <memory>
#include "NSndConfig.hpp"
#include "../../include/msc/CLocklessQue.hpp"
#include "CMidiMsg.hpp"

#ifndef NOI_SOFTWARE_CINSTRUMENT_HPP
#define NOI_SOFTWARE_CINSTRUMENT_HPP

using NMsc::CLocklessQue;

namespace NSnd {
    class CInstrument {
    public:
        CInstrument();

        virtual void GenerateBuffer(const SND_DATA_TYPE *, SND_DATA_TYPE *, unsigned long);

        bool SendMidiMsg(const CMidiMsg &msg);


    protected:

        CLocklessQue<CMidiMsg> m_newMidiMsg;

        virtual void Tick();

        uint32_t m_sambleFromTickCounter;

    };

    typedef std::shared_ptr<CInstrument> AInstrument;
}

#endif // NOI_SOFTWARE_CINSTRUMENT_HPP