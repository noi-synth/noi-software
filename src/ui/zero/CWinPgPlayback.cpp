//
// Created by ddl_blue on 10.5.19.
//

#include "../../../include/ui/zero/CWinPgPlayback.hpp"
#include "../../../include/ui/zero/CNoiZeroCommunicator.hpp"

using namespace NUi::NZero;

/*----------------------------------------------------------------------*/
CWinPgPlayback::CWinPgPlayback(NUi::WWindowManager windowManager) : CWindow(windowManager) {
    AWindowManager manager = windowManager.lock();
    m_app = manager->GetApp();
}

/*----------------------------------------------------------------------*/
void CWinPgPlayback::Update() {
    m_currentBeat = m_app->PlaybackGetPositionBeats() / 4;
}

/*----------------------------------------------------------------------*/
void CWinPgPlayback::Draw() {
    CNoiZeroCommunicator *g = CNoiZeroCommunicator::GetInstance();
    if (!g) {
        NMsc::CLogger::Log(NMsc::ELogType::ERROR, "CWinPgPlayback: No CNoiZeroCommunicator found.");
        return;
    }

    // Page color
    g->SetStatusLed(EStatusLed::PAGE, ELedState::ON, NHw::ELedColor::WHITE);

    // Beat visualisation
    uint32_t beat1 = m_currentBeat % 8;
    uint32_t beat8 = (m_currentBeat / 8) % 8;
    uint32_t beat64 = (m_currentBeat / 64) % 8;

    for (uint32_t i = 0; i < 8; ++i) {
        NHw::ELedColor col = NHw::ELedColor::BLACK;
        col = NMsc::Functions::EnumOr(col, (beat1 == i ? NHw::ELedColor::RED : NHw::ELedColor::BLACK));
        col = NMsc::Functions::EnumOr(col, (beat8 == i ? NHw::ELedColor::GREEN : NHw::ELedColor::BLACK));
        col = NMsc::Functions::EnumOr(col, (beat64 == i ? NHw::ELedColor::BLUE : NHw::ELedColor::BLACK));

        g->SetFnLed(i, ELedState::ON, col);
    }

}

/*----------------------------------------------------------------------*/
NUi::CInptutEventInfo CWinPgPlayback::ProcessInput(NUi::CInptutEventInfo input) {

    if (input.IsScroll()) {
        if (input.m_input == EControlInput::ROT_3) {
            uint32_t bpm = m_app->BpmGet();
            if (input.m_type == EControlInputType::SCROLL_UP)
                bpm = (bpm < 450) ? bpm + 2 : bpm;
            else
                bpm = (bpm > 30) ? bpm - 2 : bpm;

            m_app->BpmSet(bpm);

        }
    }

    if (input.IsFnKey()) {
        if (input.m_type == EControlInputType::PRESS) {
            uint32_t beat1 = m_currentBeat % 8;
            uint32_t beat8 = (m_currentBeat / 8) % 8;
            uint32_t beat64 = (m_currentBeat / 64) % 8;

            if (input.m_shift) {
                beat8 = NMsc::Functions::EnumSub(input.m_input, EControlInput::BTN_FN_0);
            } else {
                beat1 = NMsc::Functions::EnumSub(input.m_input, EControlInput::BTN_FN_0);
            }

            m_currentBeat = beat1 + beat8 * 8 + beat64 * 64;
            m_app->PlaybaclSetPositionBeats(m_currentBeat * 4);

        } else
            return CInptutEventInfo();
    } else
        return input;
}