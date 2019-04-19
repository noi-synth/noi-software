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
/*CI2cGpioExtender::CI2cGpioExtender(std::uint32_t i2cAddress, std::uint16_t inputMask, std::uint16_t outputMask, const std::function<void(std::uint16_t, std::uint16_t, std::uint16_t, std::uint16_t )> &callback)
        :m_address(i2cAddress), m_inptutMask(inputMask), m_outputMask(outputMask),  m_lastStatus(0), m_callback(callback), m_stopWorker(false), m_msWaiting(4) {

    // are any ports declared as both inputs and outputs?
    if (inputMask & outputMask){
        std::uint16_t duplicateMask = inputMask & outputMask;
        NMsc::CLogger::Log(NMsc::ELogType::WARNING, "I2CGpoio: Some pins declared as both inputs and outputs. Disabling them. I2C address=%, mask of those ports= %", i2cAddress, duplicateMask);
        m_inptutMask  &= ~duplicateMask;
        m_outputMask  &= ~duplicateMask;
    }


    // Set wiringPi and interrupts
    wiringPiSetup();
    //wiringPiISR(7, INT_EDGE_FALLING, &GpioExtenderCallbackFunction); //todo

    //m_worker = std::thread( [&](){WorkerMethod();} );

    // Set GPIO extender
    m_i2cHandle = wiringPiI2CSetup(m_address);

    if (m_i2cHandle<0)
        NMsc::CLogger::Log(NMsc::ELogType::WARNING, "I2C setup error for address %.", i2cAddress);

    // Set pin direction on bank
    // bank A
    wiringPiI2CWriteReg8(m_i2cHandle, 0x00, m_inptutMask & 0xff);
    // bank B

    wiringPiI2CWriteReg8(m_i2cHandle, 0x01, (m_inptutMask & 0xff00)>>8);
    // Enable interrupts fot inputs
    // bank A
    wiringPiI2CWriteReg8(m_i2cHandle, 0x04, m_inptutMask & 0xFF);
    // bank B
    wiringPiI2CWriteReg8(m_i2cHandle, 0x05, (m_inptutMask & 0xFF00)>>8);

    // Interrupt on change
    wiringPiI2CWriteReg8(m_i2cHandle, 0x08, 0x00);
    wiringPiI2CWriteReg8(m_i2cHandle, 0x09, 0x00);

    // Enable pullups for inputs
    wiringPiI2CWriteReg8(m_i2cHandle, 0x0c, m_inptutMask & 0xFF);
    wiringPiI2CWriteReg8(m_i2cHandle, 0x0d, (m_inptutMask & 0xFF00)>>8);

    // Scan bank B
    m_lastStatus = (uint8_t) wiringPiI2CReadReg8(m_i2cHandle, 0x13) << 8;
    // Scan bank A
    m_lastStatus |= (uint8_t) wiringPiI2CReadReg8(m_i2cHandle, 0x12);*/


/*// Set 4 pins as inputs
wiringPiI2CWriteReg8(m_i2cHandle, 0x00, 0x0f);
// Enable interrupts for them
wiringPiI2CWriteReg8(m_i2cHandle, 0x04, 0x0f);
// Interrupt on change
wiringPiI2CWriteReg8(m_i2cHandle, 0x08, 0x00);
// Enable pullups
wiringPiI2CWriteReg8(m_i2cHandle, 0x0c, 0x0f);*/

// Actual value
//m_lastStatus = (uint8_t) wiringPiI2CReadReg8(m_i2cHandle, 0x12);

/*g_gpioExtenderHandler = std::shared_ptr<CI2cGpioExtender>(this);
}*/
CI2cGpioExtender::CI2cGpioExtender(uint32_t i2cAddress, uint16_t inputMask, uint16_t outputMask)
        : m_address(i2cAddress), m_inptutMask(inputMask), m_outputMask(outputMask) {

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

    // Set pin direction on bank
    // bank A
    wiringPiI2CWriteReg8(m_i2cHandle, 0x00, m_inptutMask & 0xff);
    // bank B
    wiringPiI2CWriteReg8(m_i2cHandle, 0x01, (m_inptutMask & 0xff00) >> 8);

    // Enable interrupts fot inputs
    // bank A
    wiringPiI2CWriteReg8(m_i2cHandle, 0x04, m_inptutMask & 0xFF);
    // bank B
    wiringPiI2CWriteReg8(m_i2cHandle, 0x05, (m_inptutMask & 0xFF00) >> 8);

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
/*void NHw::GpioExtenderCallbackFunction() {
    if (g_gpioExtenderHandler)
        g_gpioExtenderHandler->InterruptCallback();
    else
        NMsc::CLogger::Log(NMsc::ELogType::WARNING, "I2C callback: No handler registered");
}*/

/*----------------------------------------------------------------------*/
/*void CI2cGpioExtender::InterruptCallback() {
    //wiringPiI2CReadReg8(m_i2cHandle, 0x12);

    uint16_t val, diff;
    val = (std::uint16_t) wiringPiI2CReadReg8(m_i2cHandle, 0x12);

    diff = val^m_lastStatus;
    m_lastStatus = val;

    if (diff && m_callback )
        m_callback(val, diff, val, diff); // todo send correct values
}*/

/*----------------------------------------------------------------------*/
/*void CI2cGpioExtender::WorkerMethod() {

    while (!m_stopWorker){
        int res = waitForInterrupt(7, m_msWaiting);

        if (res<0){
            NMsc::CLogger::Log(NMsc::ELogType::ERROR, "GPIO interrupt error.");
            continue;
        }

        // Did interrupt come?
        std::uint16_t val, diff;
        if (res){
            m_msWaiting = MIN_WAITING;

            uint16_t preVal, preDiff;
            // load interrupt-time values for bank B and A
            preVal = (std::uint16_t) wiringPiI2CReadReg8(m_i2cHandle, 0x11) << 8;
            preVal |= (std::uint16_t) wiringPiI2CReadReg8(m_i2cHandle, 0x10);

            preDiff = preVal ^ m_lastStatus;

            // Load current values
            val = (std::uint16_t) wiringPiI2CReadReg8(m_i2cHandle, 0x13) << 8;
            val |= (std::uint16_t) wiringPiI2CReadReg8(m_i2cHandle, 0x12);

            diff = preVal ^ val;

            if ( (diff||preDiff) && m_callback)
                m_callback(preVal, preDiff, val, diff);


        }else{
            if (m_msWaiting < MAX_WAITING) {
                m_msWaiting += STEP_WAITING;
            }

            // Load current values
            val = (std::uint16_t) wiringPiI2CReadReg8(m_i2cHandle, 0x13) << 8;
            val |= (std::uint16_t) wiringPiI2CReadReg8(m_i2cHandle, 0x12);

            diff = val ^ m_lastStatus;

            if (diff && m_callback )
            m_callback(val, diff, val, diff);

        }

        m_lastStatus = val;

    }
}*/
/*----------------------------------------------------------------------*/
void CI2cGpioExtender::UpdateInput(uint16_t &value, uint16_t &diff) {
    // Load current values
    value = (std::uint16_t) wiringPiI2CReadReg8(m_i2cHandle, 0x13) << 8;
    value |= (std::uint16_t) wiringPiI2CReadReg8(m_i2cHandle, 0x12);

    diff = value ^ m_lastStatus;

    m_lastStatus = value;
}

/*----------------------------------------------------------------------*/
void CI2cGpioExtender::UpdateInputAfterInterrupt(uint16_t &preValue, uint16_t &preDiff, uint16_t &value,
                                                 uint16_t &diff) {

    // load interrupt-time values for bank B and A
    preValue = (std::uint16_t) wiringPiI2CReadReg8(m_i2cHandle, 0x11) << 8;
    preValue |= (std::uint16_t) wiringPiI2CReadReg8(m_i2cHandle, 0x10);

    preDiff = preValue ^ m_lastStatus;

    // Load current values
    value = (std::uint16_t) wiringPiI2CReadReg8(m_i2cHandle, 0x13) << 8;
    value |= (std::uint16_t) wiringPiI2CReadReg8(m_i2cHandle, 0x12);

    diff = preValue ^ value;

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