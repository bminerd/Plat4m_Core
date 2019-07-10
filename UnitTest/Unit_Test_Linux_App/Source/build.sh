#! /bin/bash

cd ..

if [ ! -d "Build" ]; then
    mkdir Build
fi

cd Build

cmake ../Source/ -G"Eclipse CDT4 - Unix Makefiles"

make

ctest ../Source/ --verbose
