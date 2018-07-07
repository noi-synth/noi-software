#!/bin/bash

echo "OBJS=`find src plg -name '*.cpp' | sed s/\.cpp/\.o/ | tr '\n' ' '`" >Makefile &&
cat "MakefileTemplate" >>Makefile && 
echo -e '\n\n#AUTOMATICALLY GENERATED:'>>Makefile && 
g++ -std=c++11 -MM `find src plg -name '*.cpp'` >>Makefile && 
echo 'Done.' && 
exit 0

echo Error $?. 
