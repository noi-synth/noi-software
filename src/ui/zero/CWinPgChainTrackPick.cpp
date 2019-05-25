//
// Created by ddl_blue on 25.5.19.
//

#include "../../../include/msc/Common.hpp"
#include "../../../include/ui/zero/CWinPgChainTrackPick.hpp"
#include "../../../include/ui/zero/CNoiZeroCommunicator.hpp"

using namespace NUi::NZero;

/*----------------------------------------------------------------------*/
CWinPgChainTrackPick::CWinPgChainTrackPick(NUi::WWindowManager windowManager) : CWindow(windowManager) {
    AWindowManager manager = windowManager.lock();
    m_app = manager->GetApp();
}

/*----------------------------------------------------------------------*/
NUi::CInptutEventInfo CWinPgChainTrackPick::ProcessInput(NUi::CInptutEventInfo input) {
    if (!input.IsFnKey())
        return input;

    if (!input.JustPressed())
        return CInptutEventInfo();

    uint32_t fnKeyId = NMsc::Functions::EnumSub(input.m_input, EControlInput::BTN_FN_0);


    // Chains
    if (fnKeyId < 4 && fnKeyId >= 0) {
        std::vector chains = m_app->ChainsGet();

        // Check chains for existence
        if (chains.size() < 4) {
            NMsc::CLogger::Log(NMsc::ELogType::ERROR, "CWinPgChainTrackPick: There are not 4 chains, just %",
                               chains.size());
            return CInptutEventInfo();
        }
        // Select the chain
        if (chains[fnKeyId])
            m_app->ChainActiveSet(chains[fnKeyId]);
    }

    // tracks
    if (fnKeyId < 8 && fnKeyId >= 4) {
        std::vector tracks = m_app->TracksGet();
        fnKeyId -= 4;

        // Check tracks for existence
        if (tracks.size() < 4) {
            NMsc::CLogger::Log(NMsc::ELogType::ERROR, "CWinPgChainTrackPick: There are not 4 tracks, just %",
                               tracks.size());
            return CInptutEventInfo();
        }
        // Select the track
        if (tracks[fnKeyId])
            m_app->TrackActiveSet(tracks[fnKeyId]);
    }

    return CInptutEventInfo();
}

/*----------------------------------------------------------------------*/
void CWinPgChainTrackPick::Draw() {
    CNoiZeroCommunicator *g = CNoiZeroCommunicator::GetInstance();
    if (!g) {
        NMsc::CLogger::Log(NMsc::ELogType::ERROR, "CWinSystemMenu: No CNoiZeroCommunicator found.");
        return;
    }

    // Page color
    g->SetStatusLed(EStatusLed::PAGE, ELedState::ON, NHw::ELedColor::BLUE);

    std::vector<NSnd::ATrack> tracks = m_app->TracksGet();
    std::vector<NSnd::AChain> chains = m_app->ChainsGet();

    NSnd::AChain activeChain = m_app->ChainActiveGet();
    NSnd::ATrack activeTrack = m_app->TrackActiveGet();

    g->ClearFnLeds();

    // Draw chains
    for (int i = 0; i < MIN(4, chains.size()); ++i) {
        g->SetFnLed(i, activeChain == chains[i] ? ELedState::BLINKING : ELedState::ON, NHw::ELedColor::YELLOW);
    }

    // Draw tracks
    for (int i = 0; i < MIN(4, tracks.size()); ++i) {
        g->SetFnLed(4 + i, activeTrack == tracks[i] ? ELedState::BLINKING : ELedState::ON, NHw::ELedColor::BLUE);
    }

}
