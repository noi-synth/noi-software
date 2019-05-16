//
// Created by ddl_blue on 5.4.18.
//

#ifndef NOI_SOFTWARE_CAUDIODEVICEINFO_HPP
#define NOI_SOFTWARE_CAUDIODEVICEINFO_HPP

#include <string>

namespace NSnd {
    /**
     * Basic info about an audio device
     */
    class CAudioDeviceInfo {
    public:
        /**
         * Index in the device table
         */
        int m_index;

        /**
         * Latency of the device
         */
        double m_latency;

        /**
         * Name of the device
         */
        std::string m_name;
    };
}

#endif //NOI_SOFTWARE_CAUDIODEVICEINFO_HPP
