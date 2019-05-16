//
// Created by ddl_blue on 9.5.19.
//

#ifndef NOI_SOFTWARE_CWINMAIN_HPP
#define NOI_SOFTWARE_CWINMAIN_HPP

#include "../CWindow.hpp"

namespace NUi {
    namespace NZero {
        /**
         * Main window of the NOI Zero UI
         */
        class CWinMain : public CWindow {
        public:
            /**
             * Constructor
             * @param windowManager Parent window manager
             */
            explicit CWinMain(WWindowManager windowManager);

            virtual CInptutEventInfo ProcessInput(CInptutEventInfo input) override;

            virtual void Draw() override;

            void Update() override;

            void Init() override;

        private:
            /// Reference to main app
            NLgc::ANoiApp m_app;

            /// True if metronome is on
            bool m_metronom;

            /// Current octave
            uint32_t m_octave; // todo move to AppState only

            /// Used for temporary LED color change
            uint32_t m_undoBlinkCountdown;

            /// Last beat of playback, used for beat detection
            uint32_t m_lastBeat;
            /// Used for temporary LED color change, on beats
            uint32_t m_beatBlinkCountdown;
        };
    }
}


#endif //NOI_SOFTWARE_CWINMAIN_HPP