//
// Created by ddl_blue on 9.5.19.
//

#include "../../../include/ui/zero/CWinMain.hpp"
#include "../../../include/ui/zero/CNoiZeroCommunicator.hpp"
#include "../../../include/ui/zero/CWinPgPick.hpp"

using namespace NUi::NZero;

/*----------------------------------------------------------------------*/
CWinMain::CWinMain(NUi::WWindowManager windowManager) : CWindow(windowManager), m_metronom(false), m_octave(2) {
    AWindowManager manager = windowManager.lock();
    m_app = manager->GetApp();
}

/*----------------------------------------------------------------------*/
void CWinMain::Draw() {
    if (CNoiZeroCommunicator::GetInstance()) {
        CNoiZeroCommunicator &g = *CNoiZeroCommunicator::GetInstance();

        //g.SetStatusLed(EStatusLed::PAGE, ELedState::BLINKING, NHw::ELedColor::WHITE);

        g.ClearFnLeds();

        // Play LED
        NHw::ELedColor playCol = NHw::ELedColor::GREEN;
        if (m_app->IsRecording())
            playCol = NHw::ELedColor::RED;

        g.SetStatusLed(EStatusLed::PLAY, m_app->IsPlaying() ? ELedState::ON : ELedState::OFF, playCol);

        g.SetStatusLed(EStatusLed::OCTAVE, ELedState::ON, g.COL_GRADIENT_LONG[m_octave]);
    }
}

/*----------------------------------------------------------------------*/
NUi::CInptutEventInfo CWinMain::ProcessInput(CInptutEventInfo input) {

    AWindowManager manager = m_manager.lock();
    if (!manager) {
        NMsc::CLogger::Log(NMsc::ELogType::ERROR, "CWinMain: Manager not found.");
        return input;
    }

    //NMsc::CLogger::Log(NMsc::ELogType::TMP_DEBUG, "CWinMain: Got input %", control);

    if (input.m_type == EControlInputType::PRESS) {
        switch (input.m_input) {
            case NUi::EControlInput::BTN_PLAY :
                if (m_app->IsPlaying())
                    m_app->PlaybackStop();
                else
                    m_app->PlaybackStart();
                return CInptutEventInfo();

            case NUi::EControlInput::BTN_REC:
                if (m_app->IsRecording())
                    m_app->RecordingStop();
                else
                    m_app->RecordingStart();
                return CInptutEventInfo();

            case NUi::EControlInput::BTN_METRONOME:
                m_metronom = !m_metronom;
                m_app->MetronomeSet(m_metronom);
                return CInptutEventInfo();

            case NUi::EControlInput::BTN_LEFT:
                if (m_octave > 0) {
                    --m_octave;
                    m_app->SetOctave(m_octave + 1);
                }
                return CInptutEventInfo();

            case NUi::EControlInput::BTN_RIGHT:
                if (m_octave < 6) {
                    ++m_octave;
                    m_app->SetOctave(m_octave + 1);
                }
                return CInptutEventInfo();

            case NUi::EControlInput::BTN_PAGE:
                manager->OpenWindowCallback(std::make_shared<CWinPgPick>(m_manager));
                return CInptutEventInfo();


            default:
                return input;

        }
    }

    return input;
}

/*----------------------------------------------------------------------*/
void CWinMain::Init() {
    // todo open a page
}

/*----------------------------------------------------------------------*/
void CWinMain::Update() {
    RequestRedraw();
}


