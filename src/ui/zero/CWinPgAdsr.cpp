//
// Created by ddl_blue on 28.6.19.
//

#include "../../../include/ui/zero/CWinPgAdsr.hpp"
#include "../../../include/ui/zero/CNoiZeroCommunicator.hpp"


using namespace NUi::NZero;

/*----------------------------------------------------------------------*/
CWinPgAdsr::CWinPgAdsr(NUi::WWindowManager windowManager) : CWindow(windowManager) {
    AWindowManager manager = windowManager.lock();
    m_app = manager->GetApp();
}

/*----------------------------------------------------------------------*/
NUi::CInptutEventInfo CWinPgAdsr::ProcessInput(NUi::CInptutEventInfo input) {
    if (input.IsFnKey())
        return CInptutEventInfo();

    if (!input.IsScroll())
        return input;

    NSnd::AChain chain = m_app->ChainActiveGet();

    if (!chain) {
        NMsc::CLogger::Log(NMsc::ELogType::ERROR, "CWinPgAdsr: No chain active");
        return CInptutEventInfo();
    }

    NSnd::AAdsrSettings adsr = chain->GetAdsr();

    if (!adsr) {
        NMsc::CLogger::Log(NMsc::ELogType::ERROR, "CWinPgAdsr: No adsr active");
        return CInptutEventInfo();
    }

    bool up = input.m_type == EControlInputType::SCROLL_UP;

    switch (input.m_input) {
        case EControlInput::ROT_0:
            if (up) {
                if (adsr->GetAttack() < 4)
                    adsr->SetAttack(adsr->GetAttack() + 0.1);
            } else {
                if (adsr->GetAttack() >= 0.1)
                    adsr->SetAttack(adsr->GetAttack() - 0.1);
            }
            break;

        case EControlInput::ROT_1:
            if (up) {
                if (adsr->GetDecay() < 4)
                    adsr->SetDecay(adsr->GetDecay() + 0.1);
            } else {
                if (adsr->GetDecay() >= 0.1)
                    adsr->SetDecay(adsr->GetDecay() - 0.1);
            }
            break;

        case EControlInput::ROT_2:
            if (up) {
                if (adsr->GetSustain() < 1)
                    adsr->SetSustain(adsr->GetSustain() + 0.1);
            } else {
                if (adsr->GetSustain() >= 0.1)
                    adsr->SetSustain(adsr->GetSustain() - 0.1);
            }
            break;

        case EControlInput::ROT_3:
            if (up) {
                if (adsr->GetRelease() < 4)
                    adsr->SetRelease(adsr->GetRelease() + 0.1);
            } else {
                if (adsr->GetRelease() >= 0.1)
                    adsr->SetRelease(adsr->GetRelease() - 0.1);
            }
            break;


        default:
            NMsc::CLogger::Log(NMsc::ELogType::ERROR, "CWinPgAdsr: Unexpected control %", input.m_input);
            return CInptutEventInfo();
    }

    return CInptutEventInfo();


}

/*----------------------------------------------------------------------*/
void CWinPgAdsr::Draw() {
    CNoiZeroCommunicator *g = CNoiZeroCommunicator::GetInstance();
    if (!g) {
        NMsc::CLogger::Log(NMsc::ELogType::ERROR, "CWinPgAdsr: No CNoiZeroCommunicator found.");
        return;
    }

    g->SetStatusLed(EStatusLed::PAGE, ELedState::ON, NHw::ELedColor::GREEN);

    g->ClearFnLeds();
}