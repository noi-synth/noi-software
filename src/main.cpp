//
// Created by ddl_blue on 6.4.18.
//

#include <stdio.h>

#include "../include/snd/CAudioDevice.hpp"
#include "../include/msc/CLogger.hpp"
#include "../include/snd/CSndCore.hpp"
#include "../plg/instr/SimpleOsc/CInstrSimpleOsc.hpp"
#include "../include/hw/CI2CGpioExtenders.hpp"


uint8_t tn;

int callback(const SND_DATA_TYPE *in, SND_DATA_TYPE *out,
             unsigned long num, void *userData) {
    for (uint32_t i = 0; i < num; ++i) {
        out[i * 2] = out[i * 2 + 1] = ((unsigned char) ++tn) / 256.0f;
    }


}

int main(int argc, const char *argv[]) {
    NSnd::AAudioDevice dev = std::make_shared<NSnd::CAudioDevice>(NSnd::CAudioDeviceConfig());

    std::cout << "AudioDevice CREATED" << std::endl;

    //dev->BindCallback(callback, 0);

    NSnd::CSndCore *core = new NSnd::CSndCore();

    core->AudioDeviceSet(dev);

    std::cout << "AudioDeviceStart: " << core->AudioDeviceStart() << std::endl;

    NSnd::AInstrument instr = std::make_shared<NPlg::NInstr::CInstrSimpleOsc>();

    NSnd::AChain chain = std::make_shared<NSnd::CChain>(instr);

    core->ChainSelect(chain);


    NHw::CI2cGpioExtenders extender(0x20, [=](std::uint16_t val, std::uint16_t diff){
        chain->ReciveMidiMsg(NSnd::CMidiMsg(
                (val&1) ? NSnd::EMidiMsgType::NOTE_ON : NSnd::EMidiMsgType::NOTE_OFF , NSnd::ETones::C4, 255));
    } );


    char tmp;

    while (1) {
        std::cin >> tmp;
        chain->ReciveMidiMsg(NSnd::CMidiMsg(NSnd::EMidiMsgType::NOTE_ON, NSnd::ETones::C4, 255));
        std::cin >> tmp;
        chain->ReciveMidiMsg(NSnd::CMidiMsg(NSnd::EMidiMsgType::NOTE_OFF, NSnd::ETones::C4, 255));

        if (tmp == 'q') {
            delete core;
            return 0;
        }
    }


    if (dev->Open()) {
        std::cout << "AudioDevice OPENNED" << std::endl;
        dev->StartStream();
        std::cout << "AudioDevice STREAM STARTED" << std::endl;
        NMsc::CLogger::Log("Stream started.");



    } else {
        NMsc::CLogger::Log("FAIL");
    }


    return 0;

}