#!/bin/bash

set -e

pushd . > /dev/null

# Switch current/working directory to here
cd "${0%/*}"

image="bminerd/plat4m_linux_dev:2.0.0"
rm_args="--rm"

docker run \
    --privileged \
    $rm_args \
    -i -t -e "TERM=xterm-256color" \
    -p 2331:2331 \
    --mount type=bind,source="$(pwd)",target="/home/${PWD##*/}" \
    --workdir "/home/${PWD##*/}" \
    $image bash $*

popd > /dev/null
