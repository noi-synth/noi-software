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

        //TODO working thread
        //TODO Mixing
        //TODO tracks
        //TODO more todos

        bool ReciveMidiMsg(const CMidiMsg &message);

        bool ChainSelect(AChain &chain);

        void ChainClearAllActive();

        AChain ChainGetSelected() = delete; // This is more tricky than it seems to be. Also, should not be needed.

        void Panic();

        bool AudioDeviceSet(AAudioDevice &device);

        void AudioDeviceUnset();

        bool AudioDeviceStart();

        bool AudioDeviceStop();

    private:

        int AudioDevCallback(const SND_DATA_TYPE *inputBuffer, SND_DATA_TYPE *outputBuffer,
                             unsigned long sampleCnt);

        /*static int AudioDevCallbackFunction(const SND_DATA_TYPE * inputBuffer, SND_DATA_TYPE * outputBuffer,
                                     unsigned long sampleCnt, void * caller);*/

        std::thread m_thread;
        /// que of MIDI msgs to be sent to selected instrument
        CLocklessQue<CMidiMsg> m_midiMsgQue;
        /// que of instruments, that should replace the selected one. Happens on callback for race condition reasons.
        CLocklessQue<AChain> m_newSelectedChain;
        std::set<AChain> m_activeChains;
        bool m_shouldClearActiveCHains; //todo
        AChain m_selectedCHain;
        AAudioDevice m_audioDevice;
    };

    typedef std::shared_ptr<CSndCore> ASndCore;
}
#endif //NOI_SOFTWARE_CSNDCORE_HPP
