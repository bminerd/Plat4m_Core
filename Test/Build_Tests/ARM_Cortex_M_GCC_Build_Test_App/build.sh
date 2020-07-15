#! /bin/bash

if [ ! -d "Build" ]; then
    mkdir Build
fi

cd Build

cmake .. -DCMAKE_TOOLCHAIN_FILE=./GccArmCortexMToolchain.cmake

make
