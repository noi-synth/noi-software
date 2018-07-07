//
// Created by ddl_blue on 6.4.18.
//

#include <stdio.h>
#include <map>

#include "../include/snd/CAudioDevice.hpp"
#include "../include/msc/CLogger.hpp"
#include "../include/snd/CSndCore.hpp"
#include "../plg/instr/SimpleOsc/CInstrSimpleOsc.hpp"
#include "../include/config.hpp"

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

    std::map<char, NSnd::ETones> toneKeys;

    toneKeys.insert(std::make_pair('w', NSnd::ETones::C4));
    toneKeys.insert(std::make_pair('e', NSnd::ETones::D4));
    toneKeys.insert(std::make_pair('r', NSnd::ETones::E4));
    toneKeys.insert(std::make_pair('t', NSnd::ETones::F4));


    char tmp;

    while (1) {
        std::cin >> tmp;
        if (tmp == 'q') {
            delete core;
            return 0;
        }
        chain->ReciveMidiMsg(NSnd::CMidiMsg(NSnd::EMidiMsgType::NOTE_ON, toneKeys[tmp], 255));

        std::cin >> tmp;
        if (tmp == 'q') {
            delete core;
            return 0;
        }
        chain->ReciveMidiMsg(NSnd::CMidiMsg(NSnd::EMidiMsgType::NOTE_OFF, toneKeys[tmp], 255));

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