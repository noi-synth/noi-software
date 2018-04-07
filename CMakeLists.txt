cmake_minimum_required(VERSION 3.7)
project(noi_software)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp src/snd/CEffect.cpp include/snd/CEffect.hpp src/snd/CEffectChain.cpp include/snd/CEffectChain.hpp src/snd/CChain.cpp include/snd/CChain.hpp src/snd/CAudioDevice.cpp include/snd/CAudioDevice.hpp src/snd/CAudioDeviceConfig.cpp include/snd/CAudioDeviceConfig.hpp include/snd/NSndConfig.hpp src/main.cpp src/msc/CLogger.cpp include/msc/CLogger.hpp)

add_executable(noi_software ${SOURCE_FILES})
