//
// Created by ddl_blue on 31.7.18.
//

#include "../../include/lgc/CAppState.hpp"

using namespace NLgc;

void CAppState::Serialize(NMsc::ASerializationNode &node) {

    node->SerializeInt("bpm", m_bpm);

    for (const auto &track : m_tracks) {
        NMsc::ASerializationNode trackNode = node->AddNodeToArray("tracks");
        track->Serialize(trackNode);
    }
}

/*----------------------------------------------------------------------*/
CAppState::CAppState(NMsc::ASerializationNode &node) {
    m_bpm = node->GetInt("bpm");
    std::vector<NMsc::ASerializationNode> tracks = node->GetSubnodeArray("tracks");

    for (auto &track : tracks) {
        m_tracks.push_back(std::make_shared<NSnd::CTrack>(track));
    }

}