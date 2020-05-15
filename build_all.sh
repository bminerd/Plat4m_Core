#!/bin/bash

bash ./git_clone_dependencies.sh

pushd .

cd UnitTest/Unit_Test_Linux_App/Source
rm -rf ../Build
bash ./build.sh

popd
pushd .

cd Tests/Build_Tests/Linux_GCC_Build_Test_App
rm -rf Build
bash ./build.sh

popd
pushd .

cd Tests/Build_Tests/ARM_Cortex_M_GCC_Build_Test_App
rm -rf Build
bash ./build.sh

popd
