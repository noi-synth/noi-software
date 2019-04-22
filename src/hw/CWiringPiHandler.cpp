//
// Created by ddl_blue on 19.4.19.
//

#include "../../include/config.hpp"

#ifndef NO_RPI_HW
#include <wiringPi.h>

#endif /* NO_RPI_HW */

#include "../../include/hw/CWiringPiHandler.hpp"
#include "../../include/msc/CLogger.hpp"

using namespace NHw;

bool CWiringPiHandler::s_isInitialized = false;

/*----------------------------------------------------------------------*/
void CWiringPiHandler::Init() {
    if (!s_isInitialized) {
        s_isInitialized = true;

        // Set wiringPi and interrupts
#ifndef NO_RPI_HW
        int ret = wiringPiSetup();
        NMsc::CLogger::Log(NMsc::ELogType::NOTE, "CWiringPiHandler: WiringPi initialized with return code %. ", ret);
        //wiringPiISR(7, INT_EDGE_FALLING, &GpioExtenderCallbackFunction); //todo
#endif /* NO_RPI_HW */
    }
}

