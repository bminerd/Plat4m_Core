#! /bin/bash

set -e

pushd . > /dev/null

# Switch current/working directory to here
cd "${0%/*}"

if [ ! -d "Build" ]; then
    mkdir Build
fi

cd Build

cmake -DCMAKE_TOOLCHAIN_FILE=../GccArmCortexM4Toolchain.cmake ..

make -j16

popd > /dev/null
