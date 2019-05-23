//
// Created by ddl_blue on 13.5.19.
//

#include "../../../include/ui/zero/CWinPgEffects.hpp"
#include "../../../include/ui/zero/CNoiZeroCommunicator.hpp"
#include "../../../plg/CPluginFactory.hpp"
#include "CChainMember.hpp"

using namespace NUi::NZero;

/*----------------------------------------------------------------------*/
CWinPgEffects::CWinPgEffects(NUi::WWindowManager windowManager) : CWindow(windowManager) {
    AWindowManager manager = windowManager.lock();
    m_app = manager->GetApp();
}

/*----------------------------------------------------------------------*/
NUi::CInptutEventInfo CWinPgEffects::ProcessInput(NUi::CInptutEventInfo input) {

    NSnd::AChain chain = m_app->ChainActiveGet();

    if (!chain)
        return input;

    std::vector<NSnd::AEffect> effects = chain->EffectChainGet();

    // Make sure eff chain is long enough.
    if (effects.size() < 8) {
        effects.insert(effects.end(), 8 - effects.size(), nullptr);
    }


    if (input.IsFnKey()) {

        if (input.m_type != EControlInputType::PRESS)
            return CInptutEventInfo();

        int32_t fnId = NMsc::Functions::EnumSub(input.m_input, EControlInput::BTN_FN_0);

        if (input.m_shift) {
            // Set or reset effect
            if (effects[fnId])
                effects[fnId] = NPlg::CPluginFactory::GetEffect(1);
            else
                effects[fnId] = NPlg::CPluginFactory::GetEffect(0);

            chain->EffectChainChange(effects);

        } else {
            // Change editing effect
            if (effects[fnId])
                m_editingEffect = effects[fnId];
            else
                m_editingEffect.reset();
        }

        return CInptutEventInfo();
    }

    // Parameter scroll
    if (input.IsScroll()) {
        if (m_editingEffect) {
            m_editingEffect->ScrollParameter(
                    NMsc::Functions::EnumSub(input.m_input, EControlInput::ROT_0),
                    input.m_type == EControlInputType::SCROLL_UP,
                    input.m_shift
            );
        }
        return CInptutEventInfo();
    }


    return input;
}


/*----------------------------------------------------------------------*/
void CWinPgEffects::Draw() {
    CNoiZeroCommunicator *g = CNoiZeroCommunicator::GetInstance();
    if (!g) {
        NMsc::CLogger::Log(NMsc::ELogType::ERROR, "CWinPgEffects: No CNoiZeroCommunicator found.");
        return;
    }

    // page color
    g->SetStatusLed(EStatusLed::PAGE, ELedState::ON, NHw::ELedColor::MAGENTA);

    // Effect chain
    g->ClearFnLeds();

    //todo tell effect type

    NSnd::AChain chain = m_app->ChainActiveGet();

    if (!chain)
        return;

    std::vector<NSnd::AEffect> effects = chain->EffectChainGet();


    int i = 0;
    for (const auto &effect : effects) {
        if (effect) {
            if (effect == m_editingEffect)
                g->SetFnLed(i, ELedState::BLINKING, NHw::ELedColor::MAGENTA);
            else
                g->SetFnLed(i, ELedState::ON, NHw::ELedColor::MAGENTA);

        } else {
            g->SetFnLed(i, ELedState::OFF, NHw::ELedColor::BLACK);
        }

        ++i;
    }

}









