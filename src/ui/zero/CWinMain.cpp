//
// Created by ddl_blue on 9.5.19.
//

#include "../../../include/ui/zero/CWinMain.hpp"
#include "../../../include/ui/zero/CNoiZeroCommunicator.hpp"

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
NUi::EControlInput CWinMain::ProcessInput(NUi::EControlInput control, NUi::EControlInputType type) {

    //NMsc::CLogger::Log(NMsc::ELogType::TMP_DEBUG, "CWinMain: Got input %", control);

    if (type == EControlInputType::PRESS) {
        switch (control) {
            case NUi::EControlInput::BTN_PLAY :
                if (m_app->IsPlaying())
                    m_app->PlaybackStop();
                else
                    m_app->PlaybackStart();

                return NUi::EControlInput::NONE;

            case NUi::EControlInput::BTN_REC:
                if (m_app->IsRecording())
                    m_app->RecordingStop();
                else
                    m_app->RecordingStart();

                return NUi::EControlInput::NONE;

            case NUi::EControlInput::BTN_METRONOME:
                m_metronom = !m_metronom;
                m_app->MetronomeSet(m_metronom);

                return NUi::EControlInput::NONE;

            case NUi::EControlInput::BTN_LEFT:
                if (m_octave > 0) {
                    --m_octave;
                    m_app->SetOctave(m_octave + 1);
                }

                return NUi::EControlInput::NONE;

            case NUi::EControlInput::BTN_RIGHT:
                if (m_octave < 6) {
                    ++m_octave;
                    m_app->SetOctave(m_octave + 1);
                }

                return NUi::EControlInput::NONE;

            default:
                return control;

        }
    }


    return control;
}

/*----------------------------------------------------------------------*/
void CWinMain::Init() {
    // todo open a page
}

/*----------------------------------------------------------------------*/
void CWinMain::Update() {
    RequestRedraw();
}


