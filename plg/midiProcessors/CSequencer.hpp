//
// Created by ddl_blue on 27.5.19.
//

#ifndef NOI_SOFTWARE_CSEQUENCER_HPP
#define NOI_SOFTWARE_CSEQUENCER_HPP

#include "../../include/snd/CMidiProcessor.hpp"

#include <vector>
#include <atomic>

namespace NPlg::NSequencer {
    class CSequencer : public NSnd::CMidiProcessor {
    public:
        typedef std::vector<std::vector<int32_t>> CToneSequence;
        typedef std::shared_ptr<CToneSequence> AToneSequence;

        CSequencer(AToneSequence &sequence);

        virtual void Tick(NSnd::CInstrument &instrument, const NSnd::CTimeInfo &timeInfo) override;

        virtual void RecieveMidiMessage(const NSnd::CMidiMsg &midiMsg, NSnd::CInstrument &instrument) override;

        void ChangeSequence(const AToneSequence &sequence);

    private:

        AToneSequence m_sequence;
        NMsc::CLocklessQue<AToneSequence> m_newSequence;

        uint32_t m_sequencePosition;
        uint64_t m_time;
        uint64_t m_sequenceStartTime;

        uint32_t m_speed;

        bool m_playing;

        NSnd::ETones m_baseTone;

    };
}

#endif //NOI_SOFTWARE_CSEQUENCER_HPP
