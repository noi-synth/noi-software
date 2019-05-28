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
#include "CTimeInfo.hpp"

using NMsc::CLocklessQue;

namespace NSnd {
    typedef std::shared_ptr<std::vector<AEffect>> AEffectChain;

    /**
     * Represents one chain in audio core. Contains an instrument and chain of effects.
     */
    class CChain {
    public:
        //TODO implement effect chain, implement ProcessBuffer status flags, idle mechanics
        //TODO UI-SndCore sync mechanism

        /**
         * Constructor
         * @param instr Instrument used in this chain
         */
        explicit CChain(const AInstrument &instr);

//        void MakrActive();
//
//        void MarkInactive();

        /**
         * Change instrument used in this chain
         * @param instrument New instrument
         * @return True if successful
         */
        bool InstrumentChange(const AInstrument &instrument);

        /**
         * Change the effect chain
         * @param newEffChain New chain of effects
         * @return True if successful
         */
        bool EffectChainChange(const std::vector<AEffect> &newEffChain);

        /**
         * Get current effect chain
         * @return Current chain of effects
         */
        std::vector<AEffect> EffectChainGet();

        /**
         * Get current instrument
         * @return Current instrument
         */
        AInstrument InstrumentGet();

        /**
         * Send a MIDI message into the instrument in this chain
         * @param message Midi message
         * @return True if successful
         */
        bool ReciveMidiMsg(const CMidiMsg &message);

        /**
         * Process an audio buffer. Callback from the audio device. Generates audio data.
         * @param inputBuff Buffer with audio data from audio input
         * @param outputBuff Buffer where new audio data will be written to
         * @param buffLen Length of buffers in frames
         * @return 0 on success
         */
        int ProcessBuffer(const SND_DATA_TYPE *inputBuff, SND_DATA_TYPE *outputBuff, unsigned long buffLen,
                          const CTimeInfo &timeInfo);

        /**
         * Apply different midi processor
         * @param processor New midi processor. If nullptr, no processor will be used.
         */
        void ApplyMidiProcessor(AMidiProcessor &processor);

    private:

        // Active instrument and effect chain as seen from RT
        /// Active instrument. Used in RT thread
        AInstrument m_instrument;
        /// Active effect chain. Used in RT thread
        AEffectChain m_effectChain;

        // For changing instrument and effect chain in RT from UI
        /// Instruments that should replace current instrument
        CLocklessQue<AInstrument> m_newInstrument;
        /// Instruments waiting for deletion
        CLocklessQue<AInstrument> m_toDeleteInstruments;
        // Effects that should replace current effectchain
        CLocklessQue<AEffectChain> m_newEffectChains;
        // Effect chain that waits for deletion
        CLocklessQue<AEffectChain> m_toDeleteEffectChains;

        // Active instrument and effect chain as seen from UI.
        /// Active instrument. Used for UI thread.
        AInstrument m_uiInstrument;
        /// Active effect chain. Used for UI thread.
        AEffectChain m_uiEffectChain;

        /* std::thread m_thread;
         bool m_active;*/
    };

    typedef std::shared_ptr<CChain> AChain;
}


#endif //NOI_SOFTWARE_CCHAIN_HPP
