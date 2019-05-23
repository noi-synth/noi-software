//
// Created by ddl_blue on 20.5.19.
//

#ifndef NOI_SOFTWARE_CCHAINMEMBER_HPP
#define NOI_SOFTWARE_CCHAINMEMBER_HPP

#include "../msc/CLocklessQue.hpp"
#include <memory>

namespace NSnd {
    /**
     * Represents one member of CChain structure, such as instruments or effects. Implements parameter change logic.
     */
    class CChainMember {

    public:
        /**
        * Changes a parameter value
        * @param paramId ID of the parameter
        * @param directionUp Direction of change
        * @param shift Is shift pressed?
        */
        virtual void ScrollParameter(uint32_t paramId, bool directionUp, bool shift);

    protected:
        /**
        * Represents info about parameter change
        */
        struct CParamScroll {
            CParamScroll() = default;

            /**
             * Constructir
             * @param paramId ID of the parameter
             * @param directionUp Direction of change
             * @param shift Is shift pressed?
             */
            CParamScroll(uint32_t paramId, bool directionUp, bool shift) : m_paramId(paramId),
                                                                           m_directionUp(directionUp), m_shift(shift) {}

            /// paramId ID of the parameter
            uint32_t m_paramId;
            /// directionUp Direction of change
            bool m_directionUp;
            /// shift Is shift pressed?
            bool m_shift;
        };

        /// Changes of the parameters. Will be applied in RT thread.
        NMsc::CLocklessQue<CParamScroll> m_paramChanges;
    };
}


#endif //NOI_SOFTWARE_CCHAINMEMBER_HPP
