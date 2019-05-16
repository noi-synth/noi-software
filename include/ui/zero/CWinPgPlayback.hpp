//
// Created by ddl_blue on 10.5.19.
//

#ifndef NOI_SOFTWARE_CWINPGPLAYBACK_HPP
#define NOI_SOFTWARE_CWINPGPLAYBACK_HPP

#include "../CWindow.hpp"

namespace NUi::NZero {
    /**
     * Page for playback control and track position seeking
     */
    class CWinPgPlayback : public CWindow {
    public:
        /**
         * Constructor
         * @param windowManager Parent window manager
         */
        explicit CWinPgPlayback(WWindowManager windowManager);

        virtual CInptutEventInfo ProcessInput(CInptutEventInfo input) override;

        virtual void Draw() override;

        void Update() override;

        //void Init() override;

    private:
        /// Reference to main App class
        NLgc::ANoiApp m_app;
        /// Current beat of playback
        uint32_t m_currentBeat;
    };
}


#endif //NOI_SOFTWARE_CWINPGPLAYBACK_HPP
