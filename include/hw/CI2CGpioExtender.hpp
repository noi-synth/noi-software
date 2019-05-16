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

    /**
     * Communicates with the MCP23017 GPIO extender over I2C using WiringPi.
     */
    class CI2cGpioExtender {
    public:

        /**
         * Constructor. No pins can be marked as both inputs and outputs.
         * @param i2cAddress I2C address of the MCP23017 chip
         * @param inputMask Mask, that tells, which pins are defined as inputs
         * @param outputMask Mask, that tells, which pins are defined as outputs
         * @param interruptIgnoreMask Mask, that tells, which input pins should NOT cause change of the chip's interrupt pin
         */
        CI2cGpioExtender(uint32_t i2cAddress, uint16_t inputMask, uint16_t outputMask,
                         uint16_t interruptIgnoreMask = 0);

        ~CI2cGpioExtender();

        /**
         * Get current state of input pins
         * @param[out] value Values of all input pins
         * @param[out] diff Difference of current values to last values
         */
        void UpdateInput(uint16_t &value, uint16_t &diff);

        /**
         * Get current and interrupt-time state of input pins
         * @param[out] preValue Interrupt-time state of input pins
         * @param[out] preDiff Difference between interrupt-time values and last values
         * @param[out] value Values of all input pins
         * @param[out] diff Difference between interrupt-time values and current values
         */
        void UpdateInputAfterInterrupt(uint16_t &preValue, uint16_t &preDiff, uint16_t &value, uint16_t &diff);

        /**
         * Updates state of output pins
         */
        void UpdateOutput();

        /**
         * State of output pins. Does not have effect until UpdateOutput() method is called.
         */
        uint16_t m_output;

    private:

        /**
         * I2C address of the MCP23017 chip
         */
        uint32_t m_address;

        // 0bBBBBBBBBAAAAAAAA
        /**
         * Mask that defines inputs
         */
        uint16_t m_inptutMask;

        /**
         * Mask that defines outputs
         */
        uint16_t m_outputMask;

        /**
         * Mask that defines inputs that does not cause change of interrupt pin value
         */
        uint16_t m_interruptIgnoreMask;

        /**
         * State of pins at last update
         */
        uint16_t m_lastStatus;

        /**
         * WiringPi handle for I2C communication
         */
        int m_i2cHandle;

    };

    typedef std::shared_ptr<CI2cGpioExtender> AI2cGpioExtender;

}


#endif //NOI_SOFTWARE_CI2CGPIOEXTENDER_HPP

#endif //#NO_RPI_HW