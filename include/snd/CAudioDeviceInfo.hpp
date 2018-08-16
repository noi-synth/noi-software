//
// Created by ddl_blue on 5.4.18.
//

#ifndef NOI_SOFTWARE_CAUDIODEVICEINFO_HPP
#define NOI_SOFTWARE_CAUDIODEVICEINFO_HPP

#include <string>

namespace NSnd {
    class CAudioDeviceInfo {
    public:
        int m_index;
        double m_latency;
        std::string m_name;
    };
}

#endif //NOI_SOFTWARE_CAUDIODEVICEINFO_HPP
