//
// Created by ddl_blue on 27.12.17.
//

#ifndef NOI_SOFTWARE_CCHAIN_HPP
#define NOI_SOFTWARE_CCHAIN_HPP

#include <memory>
#include <thread>
#include <vector>

#include "../../include/msc/CLocklessQue.hpp"
#include "CInstrument.hpp"
#include "NSndConfig.hpp"
#include "CEffect.hpp"

using NMsc::CLocklessQue;

namespace NSnd {
    typedef std::shared_ptr<std::vector<AEffect>> AEffectChain;

    class CChain {
    public:
        //TODO implement effect chain, implement ProcessBuffer status flags, idle mechanics
        //TODO UI-SndCore sync mechanism
        explicit CChain(const AInstrument &instr);


        void MakrActive();

        void MarkInactive();

        bool InstrumentChange(const AInstrument &instrument);

        bool EffectChainChange(const std::vector<AEffect> &newEffChain);

        std::vector<AEffect> EffectChainGet();

        AInstrument InstrumentGet();

        bool ReciveMidiMsg(const CMidiMsg &message);

        int ProcessBuffer(const SND_DATA_TYPE *inputBuff, SND_DATA_TYPE *outputBuff, unsigned long buffLen);

    private:

        // Active instrument and effect chain as seen from RT
        AInstrument m_instrument;
        AEffectChain m_effectChain;

        // For changing instrument and effect chain in RT from UI
        CLocklessQue<AInstrument> m_newInstrument;
        CLocklessQue<AInstrument> m_toDeleteInstruments;
        CLocklessQue<AEffectChain> m_newEffectChains;
        CLocklessQue<AEffectChain> m_toDeleteEffectChains;

        // Active instrument and effect chain as seen from UI.
        AInstrument m_uiInstrument;
        AEffectChain m_uiEffectChain;

        std::thread m_thread;
        bool m_active;
    };

    typedef std::shared_ptr<CChain> AChain;
}


#endif //NOI_SOFTWARE_CCHAIN_HPP
