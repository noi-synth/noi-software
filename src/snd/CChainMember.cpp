//
// Created by ddl_blue on 20.5.19.
//

#include "../../include/snd/CEffect.hpp"
#include "../../include/snd/CChainMember.hpp"

using namespace NSnd;

/*----------------------------------------------------------------------*/
void NSnd::CChainMember::ScrollParameter(uint32_t paramId, bool directionUp, bool shift) {
    m_paramChanges.Push(CParamScroll(paramId, directionUp, shift));
}