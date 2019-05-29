//
// Created by ddl_blue on 29.5.19.
//

#ifndef NOI_SOFTWARE_CLIMITER_HPP
#define NOI_SOFTWARE_CLIMITER_HPP

#include "NSndConfig.hpp"

namespace NSnd {
    class CLimiter {
    public:
        CLimiter(double release);

        void ProcessBuffer(SND_DATA_TYPE *buffer, uint32_t len);

    private:
        double m_volume;
        double m_release;

    };
}


#endif //NOI_SOFTWARE_CLIMITER_HPP
