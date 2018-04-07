OBJS=src/snd/CChain.o src/snd/CEffect.o src/snd/CAudioDevice.o src/snd/CEffectChain.o src/snd/CAudioDeviceConfig.o src/snd/CInstrument.o src/msc/CLogger.o src/main.o 
CXX=g++
LD=g++
CXXFLAGS= -std=c++11 -Wall -pedantic -Wextra -O3
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
main.o: src/main.cpp src/../include/snd/CAudioDevice.hpp \
 src/../include/snd/CAudioDeviceConfig.hpp \
 src/../include/snd/NSndConfig.hpp \
 src/../include/snd/../../include/lib/portaudio.h \
 src/../include/msc/CLogger.hpp
CLogger.o: src/msc/CLogger.cpp src/msc/../../include/msc/CLogger.hpp
CAudioDeviceConfig.o: src/snd/CAudioDeviceConfig.cpp \
 src/snd/../../include/snd/CAudioDeviceConfig.hpp
CAudioDevice.o: src/snd/CAudioDevice.cpp \
 src/snd/../../include/snd/CAudioDevice.hpp \
 src/snd/../../include/snd/CAudioDeviceConfig.hpp \
 src/snd/../../include/snd/NSndConfig.hpp \
 src/snd/../../include/snd/../../include/lib/portaudio.h \
 src/snd/../../include/msc/CLogger.hpp
CEffect.o: src/snd/CEffect.cpp src/snd/../../include/snd/CEffect.hpp
CEffectChain.o: src/snd/CEffectChain.cpp \
 src/snd/../../include/snd/CEffectChain.hpp
CChain.o: src/snd/CChain.cpp src/snd/../../include/snd/CChain.hpp
CInstrument.o: src/snd/CInstrument.cpp \
 src/snd/../../include/snd/CInstrument.hpp
