//
// Created by ddl_blue on 27.12.17.
//

#ifndef NOI_SOFTWARE_CCHAIN_HPP
#define NOI_SOFTWARE_CCHAIN_HPP

#include <memory>

#include "../../include/msc/CLocklessQue.hpp"
#include "../../include/snd/CInstrument.hpp"
#include "NSndConfig.hpp"

using NMsc::CLocklessQue;

namespace NSnd {
    class CChain {
    public:

        void MakrActive();

        void MarkInactive();

        void InstrumentChange(const AInstrument &instrument);

        void ProcessBuffer(const SND_DATA_TYPE *, SND_DATA_TYPE *, unsigned long);

    private:

        AInstrument m_instrument;
        CLocklessQue<AInstrument> m_newInstrument;
        bool m_active;
    };

    typedef std::shared_ptr<CChain> AChain;
}


#endif //NOI_SOFTWARE_CCHAIN_HPP
