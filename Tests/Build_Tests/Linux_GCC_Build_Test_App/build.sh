#! /bin/bash

if [ ! -d "Build" ]; then
    mkdir Build
fi

cd Build

cmake ..

make
