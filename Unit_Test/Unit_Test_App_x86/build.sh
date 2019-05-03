#! /bin/bash

if [ ! -d "Build" ]; then
    mkdir Build
fi

cd Build

#cmake .. -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=../Plat4m_CMake/GccArmCortexMToolchain.cmake
cmake .. -G"Eclipse CDT4 - Unix Makefiles"

make
