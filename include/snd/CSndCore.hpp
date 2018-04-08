//
// Created by ddl_blue on 7.4.18.
//

#ifndef NOI_SOFTWARE_CSNDCORE_HPP
#define NOI_SOFTWARE_CSNDCORE_HPP

#include <set>
#include<thread>
#include "../msc/CLocklessQue.hpp"
#include "CMidiMsg.hpp"
#include "CChain.hpp"
#include "CAudioDevice.hpp"

using NMsc::CLocklessQue;
namespace NSnd {
    class CSndCore {
    public:

        bool SendMidiMsg(const CMidiMsg &message);


        void ChainSelect(const AChain &chain);

        void ChainClearAllActive();

        AChain ChainGetSelected() = delete; // This is more tricky than it seems to be. Also, should not be needed.


        void Panic();

        void AudioDeviceSet(const AAudioDevice &device);

        void AudioDeviceUnset();

        bool AudioDeviceStart();

        bool AudioDeviceStop();

    private:
        std::thread m_thread;
        CLocklessQue<CMidiMsg> m_midiMsgQue;
        CLocklessQue<AChain> m_newSelectedChain;
        std::set<AChain> m_activeChains;
        AAudioDevice m_audioDevice;
    };

}
#endif //NOI_SOFTWARE_CSNDCORE_HPP
