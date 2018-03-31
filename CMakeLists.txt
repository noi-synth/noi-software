cmake_minimum_required(VERSION 3.7)
project(noi_software)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp src/Snd/CEffect.cpp include/Snd/CEffect.hpp src/Snd/CEffectChain.cpp include/Snd/CEffectChain.hpp src/Snd/CChain.cpp include/Snd/CChain.hpp)
add_executable(noi_software ${SOURCE_FILES})