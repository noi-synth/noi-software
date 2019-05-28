//
// Created by ddl_blue on 26.5.19.
//

#ifndef NOI_SOFTWARE_CMIDIPROCESSOR_HPP
#define NOI_SOFTWARE_CMIDIPROCESSOR_HPP

#include <memory>

namespace NSnd {
    class CMidiProcessor;

    typedef std::shared_ptr<CMidiProcessor> AMidiProcessor;
}


#include "CMidiMsg.hpp"
#include "CInstrument.hpp"
#include "CTimeInfo.hpp"

namespace NSnd {
    class CMidiProcessor {
    public:
        virtual void Tick(CInstrument &instrument, const CTimeInfo &timeInfo) = 0;

        virtual void RecieveMidiMessage(const CMidiMsg &midiMsg, CInstrument &instrument) = 0;

    private:


    };


}

#endif //NOI_SOFTWARE_CMIDIPROCESSOR_HPP
