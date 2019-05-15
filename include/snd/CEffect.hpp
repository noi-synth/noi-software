//
// Created by ddl_blue on 26.12.17.
//

#ifndef NOI_SOFTWARE_CEFFECT_HPP
#define NOI_SOFTWARE_CEFFECT_HPP

#include <memory>
#include "NSndConfig.hpp"
#include "../msc/CLocklessQue.hpp"

namespace NSnd {
    class CEffect {
    public:
        virtual void ProcessBuffer(SND_DATA_TYPE *buffer, unsigned long buffLen) = 0;

        virtual void ScrollParameter(uint32_t paramId, bool directionUp, bool shift);

        virtual void FlushSound() = 0;

    protected:
        struct CParamScroll {
            CParamScroll() = default;

            CParamScroll(uint32_t paramId, bool directionUp, bool shift) : m_paramId(paramId),
                                                                           m_directionUp(directionUp), m_shift(shift) {}

            uint32_t m_paramId;
            bool m_directionUp;
            bool m_shift;
        };

        NMsc::CLocklessQue<CParamScroll> m_paramChanges;

    };

    typedef std::shared_ptr<CEffect> AEffect;
}


#endif //NOI_SOFTWARE_CEFFECT_H
