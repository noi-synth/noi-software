//
// Created by ddl_blue on 31.7.18.
//

#ifndef NOI_SOFTWARE_CAPPSTATE_HPP
#define NOI_SOFTWARE_CAPPSTATE_HPP

#include <vector>

#include "../snd/CSndCore.hpp"

namespace NLgc {
    class CAppState {
    public:


        // Serialized
        std::vector<NSnd::AChain> m_chains;


        // Not serialized
        //NSnd::AInstrument m_instrument;
        int m_chainId;
        NSnd::ASndCore m_soundCore;
    };
}

#endif //NOI_SOFTWARE_CAPPSTATE_HPP
