#!/bin/bash

# ./manualBuild.sh tests test001   > err.log 2>&1
# ./manualBuild.sh tests testTemplateInstantiation -DENABLE_STD_TtoS
# ./manualBuild.sh tests checkMacroPrefixMap

mkdir ./build-tmp/

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
     -DUSE_CHRONO=0 -DUSE_MACROPREFIXMAP=1 -fmacro-prefix-map="$(pwd)/"= ${3} --std=gnu++2a -fconcepts \
     -I "$(pwd)/include" -I "$(pwd)/build-debug/_deps/date-src/include/" \
     -o ./build-tmp/${2}  "$(pwd)/${1}/${2}.cpp"
#fi

#     -DUSE_CHRONO=0 -DUSE_MACROPREFIXMAP=0 -DRAPIDCSV_SOURCE_PATH_SIZE=$(pwd | wc -c) ${3} --std=gnu++2a -fconcepts \
#                                                                         echo "$(pwd)/" not using this as
#                                                               echo "12345" | wc -c -> 6   as it counts '\0'


#gcc -lstdc++ --verbose ${3}  --std=gnu++2a -fconcepts -I ./include ./${1}/${2}.cpp  -o ./build-tmp/${2} 

