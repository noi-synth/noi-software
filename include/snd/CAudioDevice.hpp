//
// Created by ddl_blue on 13.8.18.
//

#ifndef NOI_SOFTWARE_CAUDIODEVICE_HPP
#define NOI_SOFTWARE_CAUDIODEVICE_HPP


#include <functional>
#include <memory>
#include <atomic>
#include "NSndConfig.hpp"

namespace NSnd {
    class CAudioDevice {
    public:
        virtual bool StartStream() = 0;

        virtual bool StopStream() = 0;

        //static void StreamFinishedCallback(void *userData);

        virtual bool Open() = 0;

        virtual bool
        BindCallback(const std::function<int(const SND_DATA_TYPE *, SND_DATA_TYPE *, unsigned long)> &callback) = 0;

        std::atomic_int m_LastFrameSize;
    };

    typedef std::shared_ptr<CAudioDevice> AAudioDevice;
}
#endif //NOI_SOFTWARE_CAUDIODEVICE_HPP
