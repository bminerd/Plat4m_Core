#! /bin/bash

set -e

pushd . > /dev/null

# Switch current/working directory to here
cd "${0%/*}"

image_name="Unit_Test_Linux_App"

if [ ! -d "Build" ]; then
    mkdir Build
fi

cd Build

cmake ..
make -j16

objdump -S --disassemble ${image_name} > ${image_name}.dump
size ${image_name}

popd > /dev/null
