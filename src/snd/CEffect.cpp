//
// Created by ddl_blue on 26.12.17.
//

#include "../../include/snd/CEffect.hpp"

using namespace NSnd;

/*----------------------------------------------------------------------*/
void CEffect::ScrollParameter(uint32_t paramId, bool directionUp, bool shift) {
    m_paramChanges.Push(CParamScroll(paramId, directionUp, shift));
}

