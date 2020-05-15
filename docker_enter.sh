#!/bin/bash

# Create and run a container based on bminerd/plat4m_dev_linux:1.0.0 image and remove on exit
docker run --rm --name plat4m_linux_dev_container --privileged -i -t \
    --mount type=bind,source="$(pwd)",target="/home/${PWD##*/}" \
    --workdir "/home/${PWD##*/}" \
    bminerd/plat4m_linux_dev:1.0.0 bash
