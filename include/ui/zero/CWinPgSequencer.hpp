//
// Created by ddl_blue on 29.5.19.
//

#ifndef NOI_SOFTWARE_CWINPGSEQUENCER_HPP
#define NOI_SOFTWARE_CWINPGSEQUENCER_HPP

#include "../CWindow.hpp"
#include "../../../plg/midiProcessors/CSequencer.hpp"

namespace NUi::NZero {
    class CWinPgSequencer : public CWindow {
    public:
        /**
         * Constructor
         * @param windowManager Parent window manager
         */
        explicit CWinPgSequencer(WWindowManager windowManager);

        virtual CInptutEventInfo ProcessInput(CInptutEventInfo input) override;

        virtual void Draw() override;

        virtual void Init() override;

        virtual void Update() override;

        virtual void Activate() override;

        virtual void Deactivate() override;

    private:
        /// Reference to main App class
        NLgc::ANoiApp m_app;

        NUi::EControlInput m_baseTone;

        NPlg::NSequencer::CSequencer::CToneSequence m_sequence;
        NPlg::NSequencer::ASequencer m_editedSequencer;

        std::set<int32_t> m_thisPositionTones;
        uint32_t m_pressedTonesCnt;

        bool m_shiftPressed;


    };

}

#endif //NOI_SOFTWARE_CWINPGSEQUENCER_HPP
