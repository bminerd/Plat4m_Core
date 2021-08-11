#! /bin/bash

set -e

pushd .

# Switch current/working directory to here
cd "${0%/*}"

cd Build

ctest .. --verbose

popd
