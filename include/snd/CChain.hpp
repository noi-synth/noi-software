//
// Created by ddl_blue on 27.12.17.
//

#ifndef NOI_SOFTWARE_CCHAIN_HPP
#define NOI_SOFTWARE_CCHAIN_HPP

#include <memory>
#include <thread>

#include "../../include/msc/CLocklessQue.hpp"
#include "../../include/snd/CInstrument.hpp"
#include "NSndConfig.hpp"

using NMsc::CLocklessQue;

namespace NSnd {
    class CChain {
    public:
        //TODO implement effect chain, implement ProcessBuffer status flags, idle mechanics
        //TODO polyphonic/monophonic support
        explicit CChain(const AInstrument &instr);

        void MakrActive();

        void MarkInactive();

        bool InstrumentChange(const AInstrument &instrument);

        bool ReciveMidiMsg(const CMidiMsg &message);

        int ProcessBuffer(const SND_DATA_TYPE *inputBuff, SND_DATA_TYPE *outputBuff, unsigned long buffLen);

    private:

        AInstrument m_instrument;
        CLocklessQue<AInstrument> m_newInstrument;
        CLocklessQue<AInstrument> m_toDeleteInstruments;
        std::thread m_thread;
        bool m_active;
    };

    typedef std::shared_ptr<CChain> AChain;
}


#endif //NOI_SOFTWARE_CCHAIN_HPP
