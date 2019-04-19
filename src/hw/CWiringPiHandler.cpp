//
// Created by ddl_blue on 19.4.19.
//

#include <wiringPi.h>
#include "../../include/hw/CWiringPiHandler.hpp"
#include "../../include/msc/CLogger.hpp"

using namespace NHw;

bool CWiringPiHandler::s_isInitialized = false;

/*----------------------------------------------------------------------*/
void CWiringPiHandler::Init() {
    if (!s_isInitialized) {
        s_isInitialized = true;

        // Set wiringPi and interrupts
        int ret = wiringPiSetup();
        //wiringPiISR(7, INT_EDGE_FALLING, &GpioExtenderCallbackFunction); //todo

        NMsc::CLogger::Log(NMsc::ELogType::NOTE, "CWiringPiHandler: WiringPi initialized with return code %. ", ret);
    }
}