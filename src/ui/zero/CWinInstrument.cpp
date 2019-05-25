//
// Created by ddl_blue on 23.5.19.
//

#include "../../../include/ui/zero/CWinInstrument.hpp"
#include "../../../include/ui/zero/CNoiZeroCommunicator.hpp"

using namespace NUi::NZero;

/*----------------------------------------------------------------------*/
CWinInstrument::CWinInstrument(NUi::WWindowManager windowManager) : CWindow(windowManager) {
    AWindowManager manager = windowManager.lock();
    m_app = manager->GetApp();
}

/*----------------------------------------------------------------------*/
void CWinInstrument::Draw() {
    CNoiZeroCommunicator *g = CNoiZeroCommunicator::GetInstance();
    if (!g) {
        NMsc::CLogger::Log(NMsc::ELogType::ERROR, "CWinPgEffects: No CNoiZeroCommunicator found.");
        return;
    }

    g->ClearFnLeds();

    g->SetStatusLed(EStatusLed::PAGE, ELedState::ON, NHw::ELedColor::CYAN);
    g->SetFnLed(0, ELedState::BLINKING, NHw::ELedColor::YELLOW);
}

/*----------------------------------------------------------------------*/
NUi::CInptutEventInfo CWinInstrument::ProcessInput(NUi::CInptutEventInfo input) {
    NSnd::AChain chain = m_app->ChainActiveGet();
    if (!chain)
        return input;

    NSnd::AInstrument activeInstrument = chain->InstrumentGet();
    if (!activeInstrument)
        return input;

    if (!input.IsScroll())
        return input;


    uint32_t knobId = input.m_input == NUi::EControlInput::ROT_0 ? 0 :
                      input.m_input == NUi::EControlInput::ROT_1 ? 1 :
                      input.m_input == NUi::EControlInput::ROT_2 ? 2 : 3;


    activeInstrument->ScrollParameter(knobId, input.m_type == NUi::EControlInputType::SCROLL_UP,
                                      input.m_shift);
}
