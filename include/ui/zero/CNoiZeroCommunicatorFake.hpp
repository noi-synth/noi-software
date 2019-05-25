//
// Created by ddl_blue on 18.5.19.
//

#ifndef NOI_SOFTWARE_CNOIZEROCOMMUNICATORFAKE_HPP
#define NOI_SOFTWARE_CNOIZEROCOMMUNICATORFAKE_HPP

#include "CNoiZeroCommunicator.hpp"
#include "../../gfx/CNcurses.hpp"

namespace NUi::NZero {
    /**
     * Fake NOI Zero graphical output. Allows running ZeroUI in terminal.
     */
    class CNoiZeroCommunicatorFake : public CNoiZeroCommunicator {
    public:
        /**
         * Constructor
         * @param ui
         * @param app
         */
        CNoiZeroCommunicatorFake(AZeroUi ui, NLgc::ANoiApp app);

        virtual void SetFnLed(uint32_t ledId, ELedState state, NHw::ELedColor color) override;


        virtual void SetStatusLed(EStatusLed ledId, ELedState state, NHw::ELedColor color) override;


        virtual void ClearFnLeds() override;


        virtual void ClearStatusLeds() override;


        virtual void Update() override;

    private:

        void DebugMessage(const std::string &message);
        AZeroUi m_ui;
        NLgc::ANoiApp m_app;

        bool m_shift;
        bool m_pgButton;
        uint16_t m_knobPos[4];

        uint16_t m_blinkCounter;

        NGfx::CNcurses::ColorPair TranslateColor(NHw::ELedColor col);

        void DrawKnoabs();

        static const uint32_t WIDTH = 29;
        static const uint32_t HEIGHT = 13;

        static const uint32_t FN_LED_X = 10;
        static const uint32_t FN_LED_Y = 4;
        static const uint32_t FN_LED_X_STEP = 2;

        static const uint32_t STAT_LED_X = 2;
        static const uint32_t STAT_LED_Y = 4;
        static const uint32_t STAT_LED_X_STEP = 2;
        static const uint32_t STAT_LED_Y_STEP = 2;

        static const uint32_t KNOB_X = 10;
        static const uint32_t KNOB_Y = 2;
        static const uint32_t KNOB_X_STEP = 4;

        static const uint32_t NOTE_CNT = 12;
        bool m_pressedNotes[NOTE_CNT];

        const char KNOB_CHARS[4] = {'-', '\\', '|', '/'};

        std::list<std::string> m_debugMessages;
    };
}


#endif //NOI_SOFTWARE_CNOIZEROCOMMUNICATORFAKE_HPP
