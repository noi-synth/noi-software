#!/bin/bash

echo "OBJS=`find src -name '*.cpp' | sed s/\.cpp/\.o/ | tr '\n' ' '`" >Makefile &&
cat "MakefileTemplate" >>Makefile && 
echo -e '\n\n#AUTOMATICALLY GENERATED:'>>Makefile && 
g++ -std=c++11 -MM src/*.cpp src/*/*.cpp>>Makefile && 
echo 'Done.' && 
exit 0

echo Error $?. 
