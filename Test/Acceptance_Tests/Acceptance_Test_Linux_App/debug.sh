#!/bin/bash

set -e

pushd .

# Switch current/working directory to here
cd "${0%/*}"

gdbserver --once --remote-debug localhost:2331 Build/Acceptance_Test_Linux_App

popd
