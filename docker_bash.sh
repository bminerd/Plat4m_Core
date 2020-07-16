#!/bin/bash

if [ ! -z "$1" ]; then # If command is not null, run the command and exit

    # Create and run a container based on bminerd/plat4m_dev_linux:1.0.0 image and remove on exit
    docker run --rm --name plat4m_linux_dev_container --privileged \
        --mount type=bind,source="$(pwd)",target="/home/${PWD##*/}" \
        --workdir "/home/${PWD##*/}" \
        bminerd/plat4m_linux_dev:1.0.0 bash -c "$*"

else # If command is null, enter container

    # Create and run a container based on bminerd/plat4m_dev_linux:1.0.0 image and remove on exitt
    docker run --rm --name plat4m_linux_dev_container --privileged \
        -i -t \
        --mount type=bind,source="$(pwd)",target="/home/${PWD##*/}" \
        --workdir "/home/${PWD##*/}" \
        bminerd/plat4m_linux_dev:1.0.0 bash

fi