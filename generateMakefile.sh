#!/bin/bash

echo 'Generating makefile is depricated. You do not need to do that anymore.'
exit 0


##############

echo "OBJS=`find src plg -name '*.cpp' | sed s/\.cpp/\.o/ | tr '\n' ' '`" >Makefile &&
cat "MakefileTemplate" >>Makefile && 
echo -e '\n\n#AUTOMATICALLY GENERATED:'>>Makefile && 
g++ -std=c++11 -MM `find src plg -name '*.cpp'` >>Makefile && 
echo 'Done.' && 
exit 0

echo Error $?. 
