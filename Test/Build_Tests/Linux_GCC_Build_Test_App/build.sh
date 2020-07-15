#! /bin/bash

pushd .

# Switch current/working directory to here
cd "${0%/*}"

if [ ! -d "Build" ]; then
    mkdir Build
fi

cd Build

cmake ..

make -j8

popd
