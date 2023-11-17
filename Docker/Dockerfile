# This Dockerfile is for creating a Plat4m Linux dev image

FROM ubuntu:20.04

ARG USERNAME=plat4m
ARG USER_HOME=/home/$USERNAME
ARG USER_UID=1000
ARG USER_GID=$USER_UID

ENV GCC_ARM_COMPILER_NAME=arm-gnu-toolchain-12.3.rel1-x86_64-arm-none-eabi
ENV GCC_ARM_COMPILER_PATH=$USER_HOME/$GCC_ARM_COMPILER_NAME

COPY bashrc_additions.txt /root/temp/
COPY git-prompt.sh /root/temp/

ENV TZ=America/New_York
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# Add plat4m user
RUN groupadd --gid $USER_GID $USERNAME && \
    useradd --uid $USER_UID --gid $USER_GID -m --shell /bin/bash $USERNAME &&\
    apt update &&\
    apt install -y sudo &&\
    echo $USERNAME ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$USERNAME &&\
    chmod 0440 /etc/sudoers.d/$USERNAME &&\
    mkdir $USER_HOME/temp &&\
    mv /root/temp/* $USER_HOME/temp &&\
    ls $USER_HOME/temp &&\
    cat $USER_HOME/temp/bashrc_additions.txt >> $USER_HOME/.bashrc &&\
    mv $USER_HOME/temp/git-prompt.sh $USER_HOME &&\
    chown -R $USERNAME:$USERNAME $USER_HOME/temp

# All commands from here down are now run with plat4m username
USER $USERNAME

# Install general dependencies
RUN sudo apt update &&\
    sudo apt install -y unzip bash-completion git make cmake ninja-build gcc g++ wget gdbserver gdb libncurses5 libncursesw5

# Install clang-12
RUN sudo apt install -y gnupg &&\
    echo "deb http://apt.llvm.org/focal/ llvm-toolchain-focal-12 main" | sudo tee -a /etc/apt/sources.list &&\
    wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | sudo apt-key add - &&\
    sudo apt update &&\
    sudo apt install -y clang-12

# Install gcovr
RUN sudo apt install -y python3 python3-pip &&\
    sudo pip3 install gcovr

# Install ARM GCC compiler
RUN cd ~/temp &&\
    wget https://developer.arm.com/-/media/Files/downloads/gnu/12.3.rel1/binrel/arm-gnu-toolchain-12.3.rel1-x86_64-arm-none-eabi.tar.xz &&\
    tar -xf arm-gnu-toolchain-12.3.rel1-x86_64-arm-none-eabi.tar.xz &&\
    mv $GCC_ARM_COMPILER_NAME ~/ &&\
    sudo ln -s $GCC_ARM_COMPILER_PATH /usr/local/$GCC_ARM_COMPILER_NAME

# Clean-up
RUN cd ~ &&\
    rm -rf temp &&\
    sudo apt autoremove &&\
    sudo apt autoclean
