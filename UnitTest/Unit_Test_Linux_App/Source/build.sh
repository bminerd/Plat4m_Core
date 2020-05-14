#! /bin/bash

cd ..

if [ ! -d "Build" ]; then
    mkdir Build
fi

cd Build

cmake ../Source/

make
