#! /bin/bash

set -e

pushd .

# Switch current/working directory to here
cd "${0%/*}"

if [ ! -d "Build" ]; then
    mkdir Build
fi

cd Build

cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..

make -j8

popd
