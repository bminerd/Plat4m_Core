# This Dockerfile is for creating a Plat4m Linux dev image

FROM ubuntu:18.04

RUN apt update &&\
    apt install -y git make cmake gcc g++ wget gdbserver &&\
    wget https://developer.arm.com/-/media/Files/downloads/gnu-rm/8-2019q3/RC1.1/gcc-arm-none-eabi-8-2019-q3-update-linux.tar.bz2 &&\
    tar -xjf gcc-arm-none-eabi-8-2019-q3-update-linux.tar.bz2 &&\
    mv gcc-arm-none-eabi-8-2019-q3-update /usr/local &&\
    rm gcc-arm-none-eabi-8-2019-q3-update-linux.tar.bz2 &&\
    apt autoremove