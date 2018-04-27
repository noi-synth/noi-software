OBJS=src/snd/CChain.o src/snd/CSndCore.o src/snd/CEffect.o src/snd/CAudioDevice.o src/snd/CAudioDeviceConfig.o src/snd/CMidiMsg.o src/snd/CInstrument.o src/msc/MscFunctions.o src/msc/CLogger.o src/main.o plg/instr/SimpleOsc/CInstrSimpleOsc.o 
CXX=g++
LD=g++
CXXFLAGS= -std=c++17 -Wall -pedantic -Wextra -O3
LIBS=lib/libportaudio.a -lrt -lm -lasound -pthread -lncurses
PROGRAM=noi_software


all: $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(LD) -o $@ $^ $(LIBS)

clean:
	rm -f $(PROGRAM)
	rm -f src/*/*.o
	rm -f src/*.o

run: $(PROGRAM)
	./$(PROGRAM)

%o: %cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<



#AUTOMATICALLY GENERATED:
CChain.o: src/snd/CChain.cpp src/snd/../../include/snd/CChain.hpp \
 src/snd/../../include/snd/../../include/msc/CLocklessQue.hpp \
 src/snd/../../include/snd/../../include/snd/CInstrument.hpp \
 src/snd/../../include/snd/../../include/snd/NSndConfig.hpp \
 src/snd/../../include/snd/../../include/snd/../../include/msc/CLocklessQue.hpp \
 src/snd/../../include/snd/../../include/snd/CMidiMsg.hpp \
 src/snd/../../include/snd/../../include/snd/SndEnums.hpp \
 src/snd/../../include/snd/NSndConfig.hpp \
 src/snd/../../include/msc/CLogger.hpp
CSndCore.o: src/snd/CSndCore.cpp src/snd/../../include/snd/CSndCore.hpp \
 src/snd/../../include/snd/../msc/CLocklessQue.hpp \
 src/snd/../../include/snd/CMidiMsg.hpp \
 src/snd/../../include/snd/SndEnums.hpp \
 src/snd/../../include/snd/CChain.hpp \
 src/snd/../../include/snd/../../include/msc/CLocklessQue.hpp \
 src/snd/../../include/snd/../../include/snd/CInstrument.hpp \
 src/snd/../../include/snd/../../include/snd/NSndConfig.hpp \
 src/snd/../../include/snd/../../include/snd/../../include/msc/CLocklessQue.hpp \
 src/snd/../../include/snd/../../include/snd/CMidiMsg.hpp \
 src/snd/../../include/snd/NSndConfig.hpp \
 src/snd/../../include/snd/CAudioDevice.hpp \
 src/snd/../../include/snd/CAudioDeviceConfig.hpp \
 src/snd/../../include/snd/../../include/lib/portaudio.h
CEffect.o: src/snd/CEffect.cpp src/snd/../../include/snd/CEffect.hpp
CAudioDevice.o: src/snd/CAudioDevice.cpp \
 src/snd/../../include/snd/CAudioDevice.hpp \
 src/snd/../../include/snd/CAudioDeviceConfig.hpp \
 src/snd/../../include/snd/NSndConfig.hpp \
 src/snd/../../include/snd/../../include/lib/portaudio.h \
 src/snd/../../include/msc/CLogger.hpp
CAudioDeviceConfig.o: src/snd/CAudioDeviceConfig.cpp \
 src/snd/../../include/snd/CAudioDeviceConfig.hpp
CMidiMsg.o: src/snd/CMidiMsg.cpp src/snd/../../include/snd/CMidiMsg.hpp \
 src/snd/../../include/snd/SndEnums.hpp
CInstrument.o: src/snd/CInstrument.cpp \
 src/snd/../../include/snd/CInstrument.hpp \
 src/snd/../../include/snd/NSndConfig.hpp \
 src/snd/../../include/snd/../../include/msc/CLocklessQue.hpp \
 src/snd/../../include/snd/CMidiMsg.hpp \
 src/snd/../../include/snd/SndEnums.hpp
MscFunctions.o: src/msc/MscFunctions.cpp
CLogger.o: src/msc/CLogger.cpp src/msc/../../include/msc/CLogger.hpp
main.o: src/main.cpp src/../include/snd/CAudioDevice.hpp \
 src/../include/snd/CAudioDeviceConfig.hpp \
 src/../include/snd/NSndConfig.hpp \
 src/../include/snd/../../include/lib/portaudio.h \
 src/../include/msc/CLogger.hpp src/../include/snd/CSndCore.hpp \
 src/../include/snd/../msc/CLocklessQue.hpp \
 src/../include/snd/CMidiMsg.hpp src/../include/snd/SndEnums.hpp \
 src/../include/snd/CChain.hpp \
 src/../include/snd/../../include/msc/CLocklessQue.hpp \
 src/../include/snd/../../include/snd/CInstrument.hpp \
 src/../include/snd/../../include/snd/NSndConfig.hpp \
 src/../include/snd/../../include/snd/../../include/msc/CLocklessQue.hpp \
 src/../include/snd/../../include/snd/CMidiMsg.hpp \
 src/../include/snd/CAudioDevice.hpp \
 src/../plg/instr/SimpleOsc/CInstrSimpleOsc.hpp \
 src/../plg/instr/SimpleOsc/../../../include/snd/CInstrument.hpp \
 src/../plg/instr/SimpleOsc/../../../include/snd/NSndConfig.hpp \
 src/../plg/instr/SimpleOsc/../../../include/snd/../../include/msc/CLocklessQue.hpp \
 src/../plg/instr/SimpleOsc/../../../include/snd/CMidiMsg.hpp \
 src/../plg/instr/SimpleOsc/../../../include/snd/NSndConfig.hpp
