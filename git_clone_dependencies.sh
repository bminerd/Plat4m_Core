#!/bin/bash

cd ..

if [ ! -d "Plat4m_Math" ]; then
    git clone https://github.com/bminerd/Plat4m_Math.git
fi

if [ ! -d "Plat4m_Controls" ]; then
    git clone https://github.com/bminerd/Plat4m_Controls.git
fi

if [ ! -d "Eigen" ]; then
    git clone https://gitlab.com/libeigen/eigen.git Eigen
fi

if [ ! -d "Plat4m_CMake" ]; then
    git clone https://github.com/bminerd/Plat4m_CMake.git
fi
