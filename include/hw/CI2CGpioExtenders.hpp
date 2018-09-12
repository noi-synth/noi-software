//
// Created by ddl_blue on 5.5.18.
//

#include "../config.hpp"

#ifndef NO_RPI_HW

#ifndef NOI_SOFTWARE_CI2CGPIOEXTENDERS_HPP
#define NOI_SOFTWARE_CI2CGPIOEXTENDERS_HPP

#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <memory>
#include <cstdint>

namespace NHw {

    void GpioExtenderCallbackFunction(void);

    class CI2cGpioExtenders {
    public:


        CI2cGpioExtenders(uint32_t i2cAddress, const std::function<void(std::uint16_t, std::uint16_t)> &callback );

    private:

        void InterruptCallback();

        std::uint32_t m_address;
        std::uint8_t m_lastStatus;
        std::function<void(std::uint16_t, std::uint16_t)> m_callback;
        int m_i2cHandle;

        friend void GpioExtenderCallbackFunction();
    };
    typedef std::shared_ptr<CI2cGpioExtenders> AI2cGpioExtenders;

}


#endif //NOI_SOFTWARE_CI2CGPIOEXTENDERS_HPP

#endif //#NO_RPI_HW