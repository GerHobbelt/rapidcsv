#!/bin/bash

# ./manualBuild.sh tests test001 -Wall

rm -f ./build-tmp/${2}

g++ --verbose ${3}  --std=gnu++2a -fconcepts -I ./include -o ./build-tmp/${2}  ./${1}/${2}.cpp
#gcc -lstdc++ --verbose ${3}  --std=gnu++2a -fconcepts -I ./include ./${1}/${2}.cpp  -o ./build-tmp/${2} 

