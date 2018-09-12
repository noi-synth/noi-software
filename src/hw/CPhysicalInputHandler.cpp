//
// Created by ddl_blue on 10.9.18.
//

#include "../../include/hw/CPhysicalInputHandler.hpp"

using namespace NHw;

/*----------------------------------------------------------------------*/
CPhysicalInputHandler::CPhysicalInputHandler()
#ifndef NO_RPI_HW
        : m_extenders(0x20, [this](std::uint16_t val, std::uint16_t diff) {


    for (int i = 0; i < 4; ++i) {
        for (const auto &midiOutput : m_midiOutputs) {
            if (diff & 1) {
                NSnd::CMidiMsg msg(((val & 1) ? NSnd::EMidiMsgType::NOTE_ON : NSnd::EMidiMsgType::NOTE_OFF),
                                   (NSnd::ETones) (NSnd::ETones::C4 + i), 255);
            }
        }
    }

})
#endif
{

}

/*----------------------------------------------------------------------*/
void CPhysicalInputHandler::AttachMidiOutput(const std::function<void(NSnd::CMidiMsg)> &outputFunction) {
    m_midiOutputs.push_back(outputFunction);
}

/*----------------------------------------------------------------------*/
void CPhysicalInputHandler::ClearMidiOuptuts() {
    m_midiOutputs.clear();
}

