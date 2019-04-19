//
// Created by ddl_blue on 5.5.18.
//

#include "../config.hpp"

#ifndef NO_RPI_HW

#ifndef NOI_SOFTWARE_CI2CGPIOEXTENDER_HPP
#define NOI_SOFTWARE_CI2CGPIOEXTENDER_HPP

#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <memory>
#include <cstdint>
#include <thread>


namespace NHw {

    //void GpioExtenderCallbackFunction(void);

    /**
     * Communicates with the MCP23017 GPIO extender over I2C using WiringPi.
     */

    class CI2cGpioExtender {
    public:


        /* CI2cGpioExtender(std::uint32_t i2cAddress, std::uint16_t inputMask, std::uint16_t outputMask,
                          const std::function<void(std::uint16_t, std::uint16_t, std::uint16_t, std::uint16_t )> &callback);*/

        CI2cGpioExtender(uint32_t i2cAddress, uint16_t inputMask, uint16_t outputMask);

        ~CI2cGpioExtender();

        void UpdateInput(uint16_t &value, uint16_t &diff);

        void UpdateInputAfterInterrupt(uint16_t &preValue, uint16_t &preDiff, uint16_t &value, uint16_t &diff);

        void UpdateOutput();

        uint16_t m_output;

    private:

        uint32_t m_address;

        // 0bBBBBBBBBAAAAAAAA
        uint16_t m_inptutMask;
        uint16_t m_outputMask;


        uint16_t m_lastStatus;
        int m_i2cHandle;

        //std::function<void(std::uint16_t, std::uint16_t, std::uint16_t, std::uint16_t )> m_callback;




        //friend void GpioExtenderCallbackFunction();
    };

    typedef std::shared_ptr<CI2cGpioExtender> AI2cGpioExtender;

}


#endif //NOI_SOFTWARE_CI2CGPIOEXTENDER_HPP

#endif //#NO_RPI_HW