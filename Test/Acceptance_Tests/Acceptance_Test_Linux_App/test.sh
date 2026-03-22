#! /bin/bash

set -e

pushd . > /dev/null

# Switch current/working directory to here
cd "${0%/*}"

cd Build

./Acceptance_Test_Linux_App

popd > /dev/null
