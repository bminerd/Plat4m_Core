# This Dockerfile is for creating a Plat4m Linux dev image

FROM ubuntu:18.04

RUN apt update &&\
    apt install -y git make cmake gcc g++ wget gdbserver &&\
    apt autoremove