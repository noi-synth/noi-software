//
// Created by ddl_blue on 6.4.18.
//

#include <stdio.h>
#include <map>
#include <csignal>

#include "../include/snd/CAudioDevicePA.hpp"
#include "../include/msc/CLogger.hpp"
#include "../include/snd/CSndCore.hpp"
#include "../plg/instr/SimpleOsc/CInstrSimpleOsc.hpp"
#include "../include/config.hpp"
#include "../include/gfx/CNcurses.hpp"
#include "../include/ui/term/CTerminalUi.hpp"
#include "../include/msc/CMaintainer.hpp"

uint8_t tn;

int callback(const SND_DATA_TYPE *in, SND_DATA_TYPE *out,
             unsigned long num, void *userData) {
    for (uint32_t i = 0; i < num; ++i) {
        out[i * 2] = out[i * 2 + 1] = ((unsigned char) ++tn) / 256.0f;
    }


}

void signalHandler(int signum) {


    // close ncurses
    NGfx::CNcurses::GetInstance()->FreeInstance();

    std::cout << "Interrupt signal (" << signum << ") received.\n" << std::endl;

    // terminate program

    exit(signum);
}

int main(int argc, const char *argv[]) {

    signal(SIGSEGV, signalHandler);

    NLgc::ANoiApp app = std::make_shared<NLgc::CNoiApp>();

    NUi::NTerm::CTerminalUi Ui(app);
    Ui.Run();

    Ui.WaitForStop();

    NMsc::CMaintainer::GetInstance().Stop();

    return 0;
/*

    NGfx::CNcurses *gfx = NGfx::CNcurses::GetInstance();

    gfx->ClearScreen();

    gfx->SetFrame(30, 15);
    gfx->DrawEmptyWindow(NGfx::CNcurses::ColorPair::WHITE_BLUE);
    gfx->DrawText(2, 2, "lol", NGfx::CNcurses::ColorPair::RED_BLACK);
    gfx->DrawTextCentered(5, "CENTERED TEXT VOLE", NGfx::CNcurses::ColorPair::WHITE_BLACK);
    gfx->Update();

    while (gfx->GetInput() != 'q') {

    }

    gfx->FreeInstance();

    //return 0;


    NSnd::AAudioDevice dev = std::make_shared<NSnd::CAudioDevicePA>(NSnd::CAudioDeviceInfo());

    std::cout << "AudioDevice CREATED" << std::endl;

    //dev->BindCallback(callback, 0);

    NSnd::CSndCore *core = new NSnd::CSndCore();

    core->AudioDeviceSet(dev);

    std::cout << "AudioDeviceStart: " << core->AudioDeviceStart() << std::endl;

    NSnd::AInstrument instr = std::make_shared<NPlg::NInstr::CInstrSimpleOsc>();

    NSnd::AChain chain = std::make_shared<NSnd::CChain>(instr);

    core->ChainSelect(chain);

    std::map<char, NSnd::ETones> toneKeys;

    toneKeys.insert(std::make_pair('w', NSnd::ETones::C5));
    toneKeys.insert(std::make_pair('e', NSnd::ETones::D5));
    toneKeys.insert(std::make_pair('r', NSnd::ETones::E5));
    toneKeys.insert(std::make_pair('t', NSnd::ETones::F5));


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
*/
}