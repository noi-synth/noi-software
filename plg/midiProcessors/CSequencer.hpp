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

        CSequencer();

        virtual void Tick(NSnd::CInstrument &instrument, const NSnd::CTimeInfo &timeInfo) override;

        virtual void RecieveMidiMessage(const NSnd::CMidiMsg &midiMsg, NSnd::CInstrument &instrument) override;

        void ChangeSequence(const CToneSequence &sequence);

        CToneSequence GetCurrentSequence();

        std::atomic_int m_speed;

    private:

        AToneSequence m_sequence;
        CToneSequence m_uiSequence;
        NMsc::CLocklessQue<AToneSequence> m_newSequence;

        uint32_t m_sequencePosition;
        uint64_t m_time;
        uint64_t m_sequenceStartTime;

        bool m_playing;

        NSnd::ETones m_baseTone;

    };

    typedef std::shared_ptr<CSequencer> ASequencer;
}

#endif //NOI_SOFTWARE_CSEQUENCER_HPP
