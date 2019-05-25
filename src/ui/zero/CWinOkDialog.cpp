//
// Created by ddl_blue on 23.5.19.
//

#include "../../../include/ui/zero/CWinOkDialog.hpp"
#include "../../../include/ui/zero/CNoiZeroCommunicator.hpp"
#include "../../../include/msc/Common.hpp"

using namespace NUi::NZero;

/*----------------------------------------------------------------------*/
CWinOkDialog::CWinOkDialog(NUi::WWindowManager windowManager, uint64_t requestId) : CWindow(windowManager),
                                                                                    m_animationSlowdown(0),
                                                                                    m_requestId(requestId),
                                                                                    m_welcomeAnimation(7),
                                                                                    m_progress(0), m_endingAnimation(0),
                                                                                    m_result(false) {
    AWindowManager manager = windowManager.lock();
    m_app = manager->GetApp();
}

/*----------------------------------------------------------------------*/
NUi::CInptutEventInfo CWinOkDialog::ProcessInput(NUi::CInptutEventInfo input) {

    // Ignore inputs on animations
    if (m_welcomeAnimation || m_endingAnimation)
        return NUi::CInptutEventInfo();

    if (input.m_type == EControlInputType::PRESS) {
        if (input.m_input == EControlInput::BTN_RIGHT) {
            m_progress += 2;


            // Dialog OK success
            if (m_progress >= 8) {
                ++m_endingAnimation;
                m_result = true;
            }
        } else {
            m_progress = 0;
            ++m_endingAnimation;
            m_result = false;
        }
    } else
        return NUi::CInptutEventInfo();
}

/*----------------------------------------------------------------------*/
void CWinOkDialog::Draw() {
    CNoiZeroCommunicator *g = CNoiZeroCommunicator::GetInstance();
    if (!g) {
        return;
    }

    g->SetStatusLed(EStatusLed::PAGE, ELedState::BLINKING, NHw::ELedColor::YELLOW);

    // Welcome animation
    if (m_welcomeAnimation > 0) {
        g->ClearFnLeds();
        g->SetFnLed(m_welcomeAnimation, ELedState::ON, NHw::ELedColor::YELLOW);
        return;
    }

    // Ending animation
    if (m_endingAnimation > 0) {
        g->ClearFnLeds();
        for (uint32_t i = 0; i < MIN(m_endingAnimation, 7); ++i) {
            g->SetFnLed(m_result ? m_endingAnimation : 7 - m_endingAnimation,
                        ELedState::ON,
                        m_result ? NHw::ELedColor::GREEN : NHw::ELedColor::RED);
        }
        return;
    }

    // Progress bar
    g->ClearFnLeds();
    for (uint32_t i = 0; i < m_progress; ++i) {
        g->SetFnLed(i, ELedState::ON, NHw::ELedColor::GREEN);
    }
    g->SetFnLed(m_progress, ELedState::BLINKING, NHw::ELedColor::YELLOW);

}

/*----------------------------------------------------------------------*/
void CWinOkDialog::Update() {
    ++m_animationSlowdown;

    if (m_welcomeAnimation > 0) {
        if (m_animationSlowdown & ANIMATION_SLOWDOWN_MASK)
            --m_welcomeAnimation;

        return;
    }

    if (m_endingAnimation > 0) {
        if (m_animationSlowdown & ANIMATION_SLOWDOWN_MASK)
            ++m_endingAnimation;

        // End of dialog
        if (m_endingAnimation >= 8) {
            DoWithManager([&](AWindowManager manager) {
                // Return result
                NMsc::ASerializationNode ret = NMsc::CSerializationNode::GetNewTopNode();
                ret->SerializeBool("ok", m_result);
                manager->ReturnValue(m_requestId, ret);

                // Close this window
                manager->CloseTopWindowCallback();
            });
        }
    }
}