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
#include "../include/msc/CTeam3Lock.hpp"
#include "../include/msc/CTeamLock.hpp"
#include "../include/hw/CNoiZeroHw.hpp"
#include "../include/msc/CSerializationNode.hpp"
#include "../include/ui/zero/CZeroUi.hpp"
#include "../include/ui/zero/CNoiZeroCommunicator.hpp"
#include "../include/ui/zero/CNoiZeroCommunicatorReal.hpp"
#include "../include/ui/zero/CNoiZeroCommunicatorFake.hpp"

/**
 * Hande interrupt signals - print them into output
 * @param signum Number of the signal
 */
void signalHandler(int signum) {


    // close ncurses
    NGfx::CNcurses::GetInstance()->FreeInstance();

    std::cout << "Interrupt signal (" << signum << ") received.\n" << std::endl;

    // terminate program

    exit(signum);
}

/**
 * The main function of entire application
 * @param argc Number of arguments
 * @param argv Arguments
 * @return Return code
 */
int main(int argc, const char *argv[]) {

    /*NMsc::ASerializationNode n = NMsc::CSerializationNode::GetNewTopNode();


    n->SerializeString("str1", "lol");

    NMsc::ASerializationNode b = n->GetNewSubNode("bagr");
    b->SerializeInt("minusJednicka", -1);
    b->SerializeBool("Lez", false);
    b->SerializeDouble("pi", 3.14);

    std::vector<int64_t> cisla = {1, 2, 3, 4, 5};
    b->SerializeIntArray("SeznamCisel", cisla);

    n->AddNodeToArray("Arr1");
    NMsc::ASerializationNode q = n->AddNodeToArray("Arr1");
    n->AddNodeToArray("Arr1");

    q->SerializeInt("Answer", 42);

    std::cout << n->Dump() << std::endl;


    std::cout << n->GetPath() << std::endl;
    std::cout << b->GetPath() << std::endl;
    std::cout << q->GetPath() << std::endl;
    return 0;*/


    /*NMsc::CTeamLock lock2;

    std::atomic_int check;
    std::atomic_int cnt;
    std::atomic_int control;

    check = cnt = control = 0;



    std::thread t1([&](){
       while(true){
           if (lock2.TryLockBlue()){
               check |= 1;
               for (int i = 0; i < 100; ++i) {
                   if (check != 1)
                       std::cout << "Error, check = " << check << std::endl;
               }

               check &= ~1;
               lock2.Unlock();

               if (++control & (1<<20)){
                   control = 0;
                   ++cnt;
                   std::cout << "ok" << cnt << std::endl;
               }

           }
       }
    });

    std::thread t2([&](){
        while(true){
            if (lock2.TryLockRed()){
                check |= 2;
                for (int i = 0; i < 100; ++i) {
                    if (check != 2)
                        std::cout << "Error, check = " << check << std::endl;
                }

                check &= ~2;
                lock2.Unlock();

                if (++control & (1<<20)){
                    control = 0;
                    ++cnt;
                    std::cout << "ok" << cnt << std::endl;
                }
            }
        }
    });

    t1.join();
    t2.join();
    return 0;
*/
    /*NMsc::CTeam3Lock lock;

    std::atomic_int check;
    check = 0;

    std::thread t1([&](){
        while (true){
            if (lock.TryLockBlue()){
                check |= 1;

                for (int i = 0; i < 100; ++i) {
                    if (check != 1)
                        std::cout << "error, check = " << check << std::endl;
                }

                check &= ~1;
                lock.Unlock();
            }
        }
    });

    std::thread t2([&](){
        while (true){
            if (lock.TryLockRed()){
                check |= 2;

                for (int i = 0; i < 100; ++i) {
                    if (check != 2)
                        std::cout << "error, check = " << check << std::endl;
                }

                check &= ~2;
                lock.Unlock();
            }
        }
    });

    std::thread t3([&](){
        while (true){
            if (lock.TryLockGreen()){
                check |= 4;

                for (int i = 0; i < 100; ++i) {
                    if (check != 4)
                        std::cout << "error, check = " << check << std::endl;
                }

                check &= ~4;
                lock.Unlock();
            }
        }
    });

    t1.join();
    t2.join();
    t3.join();

    return 0;*/

    signal(SIGSEGV, signalHandler);



    /*--------------------------------*/
    /*NHw::CNoiZeroHw physicalInput;
    NMsc::ALocklessQue<NUi::CInptutEventInfo> q = std::make_shared<NMsc::CLocklessQue<NUi::CInptutEventInfo>>();
    physicalInput.AttachControlOutput(q);

    int col = 0;


    int counter = 0;

    while (42) {
        if (!q->Empty()) {
            NUi::CInptutEventInfo info = q->Pop();
            std::cout << "Recived " << info.m_type << " from " << info.m_input << std::endl;

            if (info.m_type == NUi::EControlInputType::SCROLL_DOWN) {
                --counter;
                std::cout << "-: " << counter << std::endl;
            }

            if (info.m_type == NUi::EControlInputType::SCROLL_UP) {
                ++counter;
                std::cout << "+: " << counter << std::endl;
            }

        }
    }*/

    /*--------------------------------*/

    // Create backend
    NLgc::ANoiApp app = std::make_shared<NLgc::CNoiApp>();

    // Set audio device
    NSnd::AAudioDevice audioIO = std::make_shared<NSnd::CAudioDevicePA>(
            NSnd::CAudioDevicePA::GetAvailableDevices()[NSnd::CAudioDevicePA::GetDefaultDeviceId()]);
    app->AudioDeviceSet(audioIO);
    app->AudioStart();


    // Add tracks to the app
    for (int i = 0; i < 4; ++i) {
        app->TrackCreate();
    }
    app->TrackActiveSet(app->TracksGet()[0]);

    // Create real UI
    NUi::NZero::AZeroUi ZeroUi = std::make_shared<NUi::NZero::CZeroUi>(app);

#ifndef NO_RPI_HW
    NHw::ANoiZeroHw physicalInput = std::make_shared<NHw::CNoiZeroHw>();

        physicalInput->AttachMidiOutput(([app](NSnd::CMidiMsg msg) {
         app->SendMidiMessage(msg);
     }));

    ;

    NUi::NZero::CNoiZeroCommunicatorReal communicator(physicalInput, ZeroUi);

#else /* NO_RPI_HW */
    NUi::NZero::CNoiZeroCommunicatorFake communicator(ZeroUi, app);
#endif /* NO_RPI_HW */

    // Create and run terminal UI
    NUi::NTerm::CTerminalUi Ui(app);
    Ui.Run();

    Ui.WaitForStop();

    // Run real UI
    ZeroUi->Run();
    ZeroUi->WaitForStop();


#ifndef NO_RPI_HW
    ZeroUi->Stop();

    ZeroUi->WaitForStop();
#endif /* NO_RPI_HW */

    // Stop the maintainer, if running
    NMsc::CMaintainer::GetInstance().Stop();

    return 0;

    /*NMsc::ALocklessQue<NUi::CInptutEventInfo> q = std::make_shared<NMsc::CLocklessQue<NUi::CInptutEventInfo>>();
    physicalInput.AttachControlOutput(q);*/

    /*int col = 0;
    int sld = 0;

     while(Ui.IsRunning()){


         if (! q->Empty()){
             NUi::CInptutEventInfo info = q->Pop();
             std::cout << "Recived " << info.m_type << " from " << info.m_input << std::endl;

             if (info.m_type == NUi::EControlInputType::PRESS){
                 ++col;
                 for (int i = 0; i < 8; ++i) {
                     physicalInput.SetLedOutput( (NHw::ELedId) (((int)NHw::ELedId::F0) + i), (NHw::ELedColor)(col & 7));
                 }
                 sld = (sld+1) & 3;
                 physicalInput.SetLedOutput( (NHw::ELedId) (((int)NHw::ELedId::S0) + sld), (NHw::ELedColor)(col & 7));
             }

         }
     }*/


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