//
// Created by ddl_blue on 31.7.18.
//

#ifndef NOI_SOFTWARE_CAPPSTATE_HPP
#define NOI_SOFTWARE_CAPPSTATE_HPP

#include <vector>

#include "../snd/CSndCore.hpp"
#include "../msc/CSerializationNode.hpp"

namespace NLgc {
    /**
     * Represents a current state of the app. Holds information about current project.
     */
    class CAppState {
    public:

        CAppState() = default;

        CAppState(NMsc::ASerializationNode &node);

        /**
         * Serialize app state
         * @param node Node for serialization
         */
        void Serialize(NMsc::ASerializationNode &node);

        // Serialized
        /**
         * All chains in the project
         */
        std::vector<NSnd::AChain> m_chains;
        /**
         * All tracks in the project
         */
        std::vector<NSnd::ATrack> m_tracks;
        /**
         * Track BPM
         */
        int m_bpm;

        // Not serialized

        //int m_chainId;
        /**
         * track currently active for recording
         */
        NSnd::ATrack m_activeTrack;
        /**
         * Currently active chain
         */
        NSnd::AChain m_activeChain;
    };
}

#endif //NOI_SOFTWARE_CAPPSTATE_HPP
