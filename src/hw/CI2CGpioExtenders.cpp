//
// Created by ddl_blue on 5.5.18.
//

#include "../../include/hw/CI2CGpioExtenders.hpp"
#include "../../include/msc/CLogger.hpp"

using namespace NHw;

AI2cGpioExtenders g_gpioExtenderHandler;
CI2cGpioExtenders::CI2cGpioExtenders(std::uint32_t i2cAddress, const std::function<void(std::uint16_t, std::uint16_t)>  &callback)
        :m_address(i2cAddress),  m_lastStatus(0), m_callback(callback) {

    // Set wiringPi and interrupts
    wiringPiSetup();
    wiringPiISR(7, INT_EDGE_FALLING, &GpioExtenderCallbackFunction);

    // Set GPIO extender
    m_i2cHandle = wiringPiI2CSetup(m_address);

    if (m_i2cHandle<0)
        NMsc::CLogger::Log("I2C setup error.");

    // Set 4 pins as inputs
    wiringPiI2CWriteReg8(m_i2cHandle, 0x00, 0x0f);
    // Enable interrupts for them
    wiringPiI2CWriteReg8(m_i2cHandle, 0x04, 0x0f);
    // Interrupt on change
    wiringPiI2CWriteReg8(m_i2cHandle, 0x08, 0x00);
    // Enable pullups
    wiringPiI2CWriteReg8(m_i2cHandle, 0x0c, 0x0f);

    // Actual value
    m_lastStatus = wiringPiI2CReadReg8(m_i2cHandle, 0x12);

    g_gpioExtenderHandler = std::shared_ptr<CI2cGpioExtenders>(this);
}

/*----------------------------------------------------------------------*/

void NHw::GpioExtenderCallbackFunction() {
    if (g_gpioExtenderHandler)
        g_gpioExtenderHandler->InterruptCallback();
    else
        NMsc::CLogger::Log("I2C callback: No handler registered");
}

/*----------------------------------------------------------------------*/

void CI2cGpioExtenders::InterruptCallback() {
    wiringPiI2CReadReg8(m_i2cHandle, 0x12);

    int val, diff;
    val = wiringPiI2CReadReg8(m_i2cHandle, 0x12);

    diff = val^m_lastStatus;
    m_lastStatus = val;

    if (diff && m_callback )
        m_callback(val, diff);
    //else
    //    std::cout << "Error. Diff=" << diff << " Callback=" << (m_callback ? "true" : "false") << " val=" << val << std::endl;
}