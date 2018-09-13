//
// Created by ddl_blue on 10.9.18.
//

#ifndef NOI_SOFTWARE_CPHYSICALINPUTHANDLER_HPP
#define NOI_SOFTWARE_CPHYSICALINPUTHANDLER_HPP

#include <vector>
#include <functional>
#include "../config.hpp"
#include "CI2CGpioExtenders.hpp"
#include "../snd/CMidiMsg.hpp"

// Thread unsafe! (adding output functions and pushing buttons might make it blow up)
namespace NHw {
    class CPhysicalInputHandler {
    public:
        CPhysicalInputHandler();

        void AttachMidiOutput(const std::function<void(NSnd::CMidiMsg)> &outputFunction);

        void ClearMidiOuptuts();

    private:
        std::vector<std::function<void(NSnd::CMidiMsg)>> m_midiOutputs;


        // I2C
#ifndef NO_RPI_HW
        CI2cGpioExtenders m_extenders;
#endif
    };
}

#endif //NOI_SOFTWARE_CPHYSICALINPUTHANDLER_HPP
