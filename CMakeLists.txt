cmake_minimum_required(VERSION 3.7)
project(noi_software)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp src/snd/CInstrument.cpp include/snd/CInstrument.hpp src/snd/CEffect.cpp include/snd/CEffect.hpp src/snd/CChain.cpp include/snd/CChain.hpp src/snd/CAudioDevice.cpp include/snd/CAudioDevice.hpp src/snd/CAudioDeviceConfig.cpp include/snd/CAudioDeviceConfig.hpp include/snd/NSndConfig.hpp src/main.cpp src/msc/CLogger.cpp include/msc/CLogger.hpp src/snd/CSndCore.cpp include/snd/CSndCore.hpp include/msc/CLocklessQue.hpp src/snd/CMidiMsg.cpp include/snd/CMidiMsg.hpp include/snd/SndEnums.hpp src/msc/MscFunctions.cpp plg/instr/SimpleOsc/CInstrSimpleOsc.cpp plg/instr/SimpleOsc/CInstrSimpleOsc.hpp)

add_executable(noi_software ${SOURCE_FILES})
