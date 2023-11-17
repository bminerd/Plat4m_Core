#! /bin/bash

set -e

pushd .

# Switch current/working directory to here
cd "${0%/*}"

cd Build

./Acceptance_Test_Linux_App

popd
