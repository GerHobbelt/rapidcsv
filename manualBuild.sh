#!/bin/bash

# ./manualBuild.sh tests test001   > err.log 2>&1
# ./manualBuild.sh tests testTemplateInstanciation -DI_KNOW_WHAT_IAM_DOING

rm -f ./build-tmp/${2}

#if [ "${2}" == "test036" ] || [ "${2}" == "test053" ]; then
#  g++ --verbose -g -Wall -Wextra -Wpedantic -Wshadow -Wpointer-arith \
#                         -Wcast-qual -Wno-missing-braces -Wswitch-default -Wcast-align \
#                         -Wunreachable-code -Wundef -Wuninitialized -Wold-style-cast \
#                         -Wsign-conversion -Weffc++ \
#     -DUSE_CHRONO=0 --std=gnu++2a -fconcepts \
#     -I ./include/ -I ./build-debug/_deps/date-src/include/ -I ./build-debug/_deps/bigfloat-src/ \
#     -o ./build-tmp/${2}  ./${1}/${2}.cpp  ./build-debug/_deps/bigfloat-src/BigFloat.cc
#else
  g++ --verbose -g -Wall -Wextra -Wpedantic -Wshadow -Wpointer-arith \
                         -Wcast-qual -Wno-missing-braces -Wswitch-default -Wcast-align \
                         -Wunreachable-code -Wundef -Wuninitialized -Wold-style-cast \
                         -Wsign-conversion -Weffc++ \
     -DUSE_CHRONO=0 ${3} --std=gnu++2a -fconcepts \
     -I ./include -I ./build-debug/_deps/date-src/include/ \
     -o ./build-tmp/${2}  ./${1}/${2}.cpp
#fi


#gcc -lstdc++ --verbose ${3}  --std=gnu++2a -fconcepts -I ./include ./${1}/${2}.cpp  -o ./build-tmp/${2} 

