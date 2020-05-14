#!/bin/bash

pushd .

cd UnitTest/Unit_Test_Linux_App/Source
rm -rf ../Build
bash ./build.sh

popd
