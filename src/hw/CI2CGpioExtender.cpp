//
// Created by ddl_blue on 5.5.18.
//

#include <functional>
//#include <linux/input.h>
#include <cstdint>
#include "../../include/config.hpp"

#ifndef NO_RPI_HW

#include "../../include/hw/CI2CGpioExtender.hpp"
#include "../../include/msc/CLogger.hpp"
#include "../../include/hw/CWiringPiHandler.hpp"

using namespace NHw;

AI2cGpioExtender g_gpioExtenderHandler;


CI2cGpioExtender::CI2cGpioExtender(uint32_t i2cAddress, uint16_t inputMask, uint16_t outputMask,
                                   uint16_t interruptIgnoreMask)
        : m_output(0), m_address(i2cAddress), m_inptutMask(inputMask), m_outputMask(outputMask),
          m_interruptIgnoreMask(interruptIgnoreMask), m_lastStatus(0) {

    // are any ports declared as both inputs and outputs?
    if (inputMask & outputMask) {
        uint16_t duplicateMask = inputMask & outputMask;
        NMsc::CLogger::Log(NMsc::ELogType::WARNING,
                           "I2CGpoio: Some pins are declared as both inputs and outputs. Disabling them. I2C address = %, mask of those ports = %",
                           i2cAddress, duplicateMask);
        m_inptutMask &= ~duplicateMask;
        m_outputMask &= ~duplicateMask;
    }

    // Initialize wiringPi
    NHw::CWiringPiHandler::Init();

    // Set GPIO extender
    m_i2cHandle = wiringPiI2CSetup(m_address);

    if (m_i2cHandle < 0)
        NMsc::CLogger::Log(NMsc::ELogType::ERROR, "I2C setup error for address %.", i2cAddress);

    // Config IOCON
    wiringPiI2CWriteReg8(m_i2cHandle, 0x0A, 0b01101100);

    // Set pin direction on bank
    // bank A
    wiringPiI2CWriteReg8(m_i2cHandle, 0x00, m_inptutMask & 0xff);
    // bank B
    wiringPiI2CWriteReg8(m_i2cHandle, 0x01, (m_inptutMask & 0xff00) >> 8);

    // Enable interrupts fot inputs
    // bank A
    wiringPiI2CWriteReg8(m_i2cHandle, 0x04, m_inptutMask & (~m_interruptIgnoreMask) & 0xFF);
    // bank B
    wiringPiI2CWriteReg8(m_i2cHandle, 0x05, (m_inptutMask & (~m_interruptIgnoreMask) & 0xFF00) >> 8);

    // Interrupt on change
    wiringPiI2CWriteReg8(m_i2cHandle, 0x08, 0x00);
    wiringPiI2CWriteReg8(m_i2cHandle, 0x09, 0x00);

    // Enable pullups for inputs
    wiringPiI2CWriteReg8(m_i2cHandle, 0x0c, m_inptutMask & 0xFF);
    wiringPiI2CWriteReg8(m_i2cHandle, 0x0d, (m_inptutMask & 0xFF00) >> 8);

    // Scan bank B
    m_lastStatus = (uint8_t) wiringPiI2CReadReg8(m_i2cHandle, 0x13) << 8;
    // Scan bank A
    m_lastStatus |= (uint8_t) wiringPiI2CReadReg8(m_i2cHandle, 0x12);
}

/*----------------------------------------------------------------------*/
CI2cGpioExtender::~CI2cGpioExtender() = default;

/*----------------------------------------------------------------------*/
void CI2cGpioExtender::UpdateInput(uint16_t &value, uint16_t &diff) {
    // No inputs
    if (!m_inptutMask) {
        value = diff = 0;
        return;
    }

    // Load current values
    value = (std::uint16_t) wiringPiI2CReadReg8(m_i2cHandle, 0x13) << 8;
    value |= (std::uint16_t) wiringPiI2CReadReg8(m_i2cHandle, 0x12);

    diff = (value ^ m_lastStatus) & m_inptutMask;

    m_lastStatus = value;
}

/*----------------------------------------------------------------------*/
void CI2cGpioExtender::UpdateInputAfterInterrupt(uint16_t &preValue, uint16_t &preDiff, uint16_t &value,
                                                 uint16_t &diff) {
    // No inputs
    if (!m_inptutMask) {
        value = diff = preDiff = preValue = 0;
        return;
    }

    // load interrupt-time values for bank B and A
    preValue = (std::uint16_t) wiringPiI2CReadReg8(m_i2cHandle, 0x11) << 8;
    preValue |= (std::uint16_t) wiringPiI2CReadReg8(m_i2cHandle, 0x10);

    preDiff = (preValue ^ m_lastStatus) & m_inptutMask;

    // Load current values
    value = (std::uint16_t) wiringPiI2CReadReg8(m_i2cHandle, 0x13) << 8;
    value |= (std::uint16_t) wiringPiI2CReadReg8(m_i2cHandle, 0x12);

    diff = (preValue ^ value) & m_inptutMask;

    m_lastStatus = value;
}

/*----------------------------------------------------------------------*/
void CI2cGpioExtender::UpdateOutput() {
    uint16_t actualOutput = m_lastStatus & m_outputMask;
    uint16_t desiredOutput = m_output & m_outputMask;

    if (actualOutput != desiredOutput) {

        // Write A value
        wiringPiI2CWriteReg8(m_i2cHandle, 0x14, desiredOutput & 0xFF);
        // Write B value
        wiringPiI2CWriteReg8(m_i2cHandle, 0x15, (desiredOutput >> 8) & 0xFF);

        // Clear output values in "last value" variable
        m_lastStatus &= ~m_outputMask;
        // Set new values of outputs to "last value" variable
        m_lastStatus |= desiredOutput;
    }
}

#endif // NO_RPI_HW