#!/bin/bash

set -euo pipefail

if [ $# -ne 1 ]; then
  echo "'$0' install-script needs one argument for installation directory-path"
  exit 1
fi

INSTALL_DIR="${1}"

# refer make.sh for NAME and BUILD_DIR values
NAME=rapidcsv_FilterSort
BUILD_DIR=build-release

if [ ! -d ${BUILD_DIR} ]; then
  ./make.sh tests
fi

[ -d ${INSTALL_DIR}/${NAME} ] && rm -rf ${INSTALL_DIR}/${NAME}
#pushd ${BUILD_DIR}
#make DESTDIR=${INSTALL_DIR} install
#popd
cmake --install ./${BUILD_DIR} --prefix ${INSTALL_DIR}/${NAME}

