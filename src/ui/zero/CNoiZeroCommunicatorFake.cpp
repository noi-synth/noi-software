//
// Created by ddl_blue on 18.5.19.
//

#include "../../../include/ui/zero/CNoiZeroCommunicatorFake.hpp"
#include "../../../include/gfx/CNcurses.hpp"

using namespace NUi::NZero;
typedef NGfx::CNcurses::ColorPair Color;

/*----------------------------------------------------------------------*/
CNoiZeroCommunicatorFake::CNoiZeroCommunicatorFake(NUi::NZero::AZeroUi ui, NLgc::ANoiApp app) : m_ui(ui), m_app(app),
                                                                                                m_shift(false),
                                                                                                m_pgButton(false) {

    for (int i = 0; i < NOTE_CNT; ++i) {
        m_pressedNotes[i] = false;
    }


    for (auto &knobPo : m_knobPos) {
        knobPo = 0;
    }

    NGfx::CNcurses *g = NGfx::CNcurses::GetInstance();

    g->SetFrame(WIDTH, HEIGHT);

    g->DrawEmptyWindow(Color::WHITE_BLACK);

//    for (int i = 0; i < 4; ++i) {
//        g->DrawPoint(STAT_LED_X, STAT_LED_Y+STAT_LED_Y_STEP*i, '#', Color::WHITE_BLACK);
//    }
    ClearStatusLeds();
    ClearFnLeds();
    DrawKnoabs();


    g->DrawPoint(STAT_LED_X + STAT_LED_X_STEP * 1, STAT_LED_Y, 'P', Color::WHITE_BLACK);
    g->DrawPoint(STAT_LED_X + STAT_LED_X_STEP * 2, STAT_LED_Y, 'M', Color::WHITE_BLACK);

    g->DrawPoint(STAT_LED_X + STAT_LED_X_STEP * 1, STAT_LED_Y + STAT_LED_Y_STEP * 1, 'R', Color::WHITE_BLACK);
    g->DrawPoint(STAT_LED_X + STAT_LED_X_STEP * 2, STAT_LED_Y + STAT_LED_Y_STEP * 1, 'P', Color::WHITE_BLACK);

    g->DrawPoint(STAT_LED_X + STAT_LED_X_STEP * 1, STAT_LED_Y + STAT_LED_Y_STEP * 2, 'U', Color::WHITE_BLACK);
    g->DrawPoint(STAT_LED_X + STAT_LED_X_STEP * 2, STAT_LED_Y + STAT_LED_Y_STEP * 2, 'S', Color::WHITE_BLACK);

    g->DrawPoint(STAT_LED_X + STAT_LED_X_STEP * 1, STAT_LED_Y + STAT_LED_Y_STEP * 3, '<', Color::WHITE_BLACK);
    g->DrawPoint(STAT_LED_X + STAT_LED_X_STEP * 2, STAT_LED_Y + STAT_LED_Y_STEP * 3, '>', Color::WHITE_BLACK);

//    for (int i = 0; i < 8; ++i) {
//        g->DrawPoint(FN_LED_X+ FN_LED_X_STEP*i, FN_LED_Y, '#', Color::WHITE_BLACK);
//    }

}

/*----------------------------------------------------------------------*/
void CNoiZeroCommunicatorFake::SetFnLed(uint32_t ledId, NUi::NZero::ELedState state, NHw::ELedColor color) {
    NGfx::CNcurses *g = NGfx::CNcurses::GetInstance();

    Color gfxCol = TranslateColor(color);
    if (state == ELedState::OFF)
        gfxCol = Color::WHITE_BLACK;

    g->DrawPoint(FN_LED_X + FN_LED_X_STEP * ledId, FN_LED_Y, state == ELedState::BLINKING ? '!' : '#', gfxCol);
}

/*----------------------------------------------------------------------*/
void CNoiZeroCommunicatorFake::SetStatusLed(NUi::NZero::EStatusLed ledId, NUi::NZero::ELedState state,
                                            NHw::ELedColor color) {
    NGfx::CNcurses *g = NGfx::CNcurses::GetInstance();

    Color gfxCol = TranslateColor(color);
    if (state == ELedState::OFF)
        gfxCol = Color::WHITE_BLACK;

    g->DrawPoint(STAT_LED_X, STAT_LED_Y + STAT_LED_Y_STEP * ledId, state == ELedState::BLINKING ? '!' : '#', gfxCol);
}

/*----------------------------------------------------------------------*/
void CNoiZeroCommunicatorFake::ClearFnLeds() {
    for (int i = 0; i < 8; ++i) {
        SetFnLed(i, ELedState::OFF, NHw::ELedColor::BLACK);
    }
}

/*----------------------------------------------------------------------*/
void CNoiZeroCommunicatorFake::ClearStatusLeds() {
    for (int i = 0; i < 4; ++i) {
        SetStatusLed((EStatusLed) i, ELedState::OFF, NHw::ELedColor::BLACK);
    }
}

/*----------------------------------------------------------------------*/
void CNoiZeroCommunicatorFake::Update() {
    NGfx::CNcurses *g = NGfx::CNcurses::GetInstance();

    g->Update();

    int chInput = g->GetInput();

    switch (chInput) {
        case '1':
            m_pgButton = !m_pgButton;
            g->DrawPoint(STAT_LED_X + STAT_LED_X_STEP * 1, STAT_LED_Y, 'P',
                         m_pgButton ? Color::BLACK_RED : Color::WHITE_BLACK);
            m_ui->ProcessInput(CInptutEventInfo(EControlInput::BTN_PAGE,
                                                m_pgButton ? EControlInputType::PRESS : EControlInputType::RELEASE,
                                                m_shift));
            break;

        case '2':
            m_ui->ProcessInput(CInptutEventInfo(EControlInput::BTN_METRONOME, EControlInputType::PRESS, m_shift));
            m_ui->ProcessInput(CInptutEventInfo(EControlInput::BTN_METRONOME, EControlInputType::RELEASE, m_shift));
            break;

        case 'q':
            m_ui->ProcessInput(CInptutEventInfo(EControlInput::BTN_PLAY, EControlInputType::PRESS, m_shift));
            m_ui->ProcessInput(CInptutEventInfo(EControlInput::BTN_PLAY, EControlInputType::RELEASE, m_shift));
            break;

        case 'w':
            m_ui->ProcessInput(CInptutEventInfo(EControlInput::BTN_REC, EControlInputType::PRESS, m_shift));
            m_ui->ProcessInput(CInptutEventInfo(EControlInput::BTN_REC, EControlInputType::RELEASE, m_shift));
            break;


        case 'a':
            m_ui->ProcessInput(CInptutEventInfo(EControlInput::BTN_UNDO, EControlInputType::PRESS, m_shift));
            m_ui->ProcessInput(CInptutEventInfo(EControlInput::BTN_UNDO, EControlInputType::RELEASE, m_shift));
            break;

        case 's':
            m_shift = !m_shift;
            g->DrawPoint(STAT_LED_X + STAT_LED_X_STEP * 2, STAT_LED_Y + STAT_LED_Y_STEP * 2, 'S',
                         m_shift ? Color::BLACK_RED : Color::WHITE_BLACK);
            m_ui->ProcessInput(CInptutEventInfo(EControlInput::BTN_SHIFT,
                                                m_shift ? EControlInputType::PRESS : EControlInputType::RELEASE,
                                                m_shift));
            break;

        case 'z':
            m_ui->ProcessInput(CInptutEventInfo(EControlInput::BTN_LEFT, EControlInputType::PRESS, m_shift));
            m_ui->ProcessInput(CInptutEventInfo(EControlInput::BTN_LEFT, EControlInputType::RELEASE, m_shift));
            break;

        case 'x':
            m_ui->ProcessInput(CInptutEventInfo(EControlInput::BTN_RIGHT, EControlInputType::PRESS, m_shift));
            m_ui->ProcessInput(CInptutEventInfo(EControlInput::BTN_RIGHT, EControlInputType::RELEASE, m_shift));
            break;

            // FN keys

        case '3':
            m_ui->ProcessInput(CInptutEventInfo(EControlInput::BTN_FN_0, EControlInputType::PRESS, m_shift));
            m_ui->ProcessInput(CInptutEventInfo(EControlInput::BTN_FN_0, EControlInputType::RELEASE, m_shift));
            break;

        case '4':
            m_ui->ProcessInput(CInptutEventInfo(EControlInput::BTN_FN_1, EControlInputType::PRESS, m_shift));
            m_ui->ProcessInput(CInptutEventInfo(EControlInput::BTN_FN_1, EControlInputType::RELEASE, m_shift));
            break;

        case '5':
            m_ui->ProcessInput(CInptutEventInfo(EControlInput::BTN_FN_2, EControlInputType::PRESS, m_shift));
            m_ui->ProcessInput(CInptutEventInfo(EControlInput::BTN_FN_2, EControlInputType::RELEASE, m_shift));
            break;

        case '6':
            m_ui->ProcessInput(CInptutEventInfo(EControlInput::BTN_FN_3, EControlInputType::PRESS, m_shift));
            m_ui->ProcessInput(CInptutEventInfo(EControlInput::BTN_FN_3, EControlInputType::RELEASE, m_shift));
            break;

        case '7':
            m_ui->ProcessInput(CInptutEventInfo(EControlInput::BTN_FN_4, EControlInputType::PRESS, m_shift));
            m_ui->ProcessInput(CInptutEventInfo(EControlInput::BTN_FN_4, EControlInputType::RELEASE, m_shift));
            break;

        case '8':
            m_ui->ProcessInput(CInptutEventInfo(EControlInput::BTN_FN_5, EControlInputType::PRESS, m_shift));
            m_ui->ProcessInput(CInptutEventInfo(EControlInput::BTN_FN_5, EControlInputType::RELEASE, m_shift));
            break;

        case '9':
            m_ui->ProcessInput(CInptutEventInfo(EControlInput::BTN_FN_6, EControlInputType::PRESS, m_shift));
            m_ui->ProcessInput(CInptutEventInfo(EControlInput::BTN_FN_6, EControlInputType::RELEASE, m_shift));
            break;

        case '0':
            m_ui->ProcessInput(CInptutEventInfo(EControlInput::BTN_FN_7, EControlInputType::PRESS, m_shift));
            m_ui->ProcessInput(CInptutEventInfo(EControlInput::BTN_FN_7, EControlInputType::RELEASE, m_shift));
            break;

            // Notes
        case 'c':
            m_pressedNotes[0] = !m_pressedNotes[0];
            m_ui->ProcessInput(CInptutEventInfo(EControlInput::NOTE_0, m_pressedNotes[0] ? EControlInputType::PRESS
                                                                                         : EControlInputType::RELEASE,
                                                m_shift));
            m_app->SendMidiMessage(
                    NSnd::CMidiMsg(m_pressedNotes[0] ? NSnd::EMidiMsgType::NOTE_ON : NSnd::EMidiMsgType::NOTE_OFF,
                                   NSnd::ETones::C0, 255));
            break;

        case 'e':
            m_ui->ProcessInput(CInptutEventInfo(EControlInput::ROT_0, EControlInputType::SCROLL_DOWN, m_shift));
            m_knobPos[0] = (m_knobPos[0] - 1) & 3;
            DrawKnoabs();
            break;


        case 'r':
            m_ui->ProcessInput(CInptutEventInfo(EControlInput::ROT_0, EControlInputType::SCROLL_UP, m_shift));
            m_knobPos[0] = (m_knobPos[0] + 1) & 3;
            DrawKnoabs();
            break;

        case 't':
            m_ui->ProcessInput(CInptutEventInfo(EControlInput::ROT_1, EControlInputType::SCROLL_DOWN, m_shift));
            m_knobPos[1] = (m_knobPos[1] - 1) & 3;
            DrawKnoabs();
            break;

        case 'y':
            m_ui->ProcessInput(CInptutEventInfo(EControlInput::ROT_1, EControlInputType::SCROLL_UP, m_shift));
            m_knobPos[1] = (m_knobPos[1] + 1) & 3;
            DrawKnoabs();
            break;

        case 'u':
            m_ui->ProcessInput(CInptutEventInfo(EControlInput::ROT_2, EControlInputType::SCROLL_DOWN, m_shift));
            m_knobPos[2] = (m_knobPos[2] - 1) & 3;
            DrawKnoabs();
            break;

        case 'i':
            m_ui->ProcessInput(CInptutEventInfo(EControlInput::ROT_2, EControlInputType::SCROLL_UP, m_shift));
            m_knobPos[2] = (m_knobPos[2] + 1) & 3;
            DrawKnoabs();
            break;

        case 'o':
            m_ui->ProcessInput(CInptutEventInfo(EControlInput::ROT_3, EControlInputType::SCROLL_DOWN, m_shift));
            m_knobPos[3] = (m_knobPos[3] - 1) & 3;
            DrawKnoabs();
            break;

        case 'p':
            m_ui->ProcessInput(CInptutEventInfo(EControlInput::ROT_3, EControlInputType::SCROLL_UP, m_shift));
            m_knobPos[3] = (m_knobPos[3] + 1) & 3;
            DrawKnoabs();
            break;

        default:
            break;
    }

}

/*----------------------------------------------------------------------*/
NGfx::CNcurses::ColorPair CNoiZeroCommunicatorFake::TranslateColor(NHw::ELedColor col) {
    switch (col) {
        case NHw::ELedColor::BLACK:
            return Color::WHITE_BLACK;

        case NHw::ELedColor::RED:
            return Color::BLACK_RED;

        case NHw::ELedColor::GREEN:
            return Color::BLACK_GREEN;

        case NHw::ELedColor::BLUE:
            return Color::BLACK_BLUE;

        case NHw::ELedColor::YELLOW:
            return Color::BLACK_YELLOW;

        case NHw::ELedColor::CYAN:
            return Color::BLACK_CYAN;

        case NHw::ELedColor::MAGENTA:
            return Color::BLACK_MAGENTA;

        case NHw::ELedColor::WHITE:
            return Color::BLACK_WHITE;

        default:
            NMsc::CLogger::Log(NMsc::ELogType::WARNING, "CNoiZeroCommunicatorFake: Unknown color %", col);
            return Color::WHITE_BLACK;
    }
}

/*----------------------------------------------------------------------*/
void CNoiZeroCommunicatorFake::DrawKnoabs() {

    NGfx::CNcurses *g = NGfx::CNcurses::GetInstance();

    for (int i = 0; i < 4; ++i) {
        g->DrawPoint(KNOB_X + i * KNOB_X_STEP, KNOB_Y, KNOB_CHARS[m_knobPos[i] & 3], Color::RED_BLACK);
    }


}