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
    /**
     * Represents an audio I/O device.
     */
    class CAudioDevice {
    public:
        /**
         * Starts an audio stream. Stream must be open.
         * @return True if successful
         */
        virtual bool StartStream() = 0;

        /**
         * Stops an audio stream.
         * @return True if successful
         */
        virtual bool StopStream() = 0;

        //static void StreamFinishedCallback(void *userData);

        /**
         * Open the audio stream.
         * @return  True if successful
         */
        virtual bool Open() = 0;

        /**
         * Bind a callback that will be called by the devide to process audio data. Device must be stopped.
         * @param callback Fucntion to be called
         * @return True if successful
         */
        virtual bool
        BindCallback(const std::function<int(const SND_DATA_TYPE *, SND_DATA_TYPE *, unsigned long)> &callback) = 0;

    };

    typedef std::shared_ptr<CAudioDevice> AAudioDevice;
}
#endif //NOI_SOFTWARE_CAUDIODEVICE_HPP
