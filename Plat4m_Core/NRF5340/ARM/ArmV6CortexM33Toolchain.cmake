cmake_minimum_required (VERSION 3.0)

set(CMAKE_SYSTEM_NAME      Generic)
set(CMAKE_SYSTEM_PROCESSOR CortexM33)

set(COMPILER_NAME "ARM_6" CACHE STRING "" FORCE)
set(COMPILER_PATH /usr/local/ARMCompiler6.15)

set(CMAKE_ASM_COMPILER ${COMPILER_PATH}/bin/armasm)
set(CMAKE_C_COMPILER ${COMPILER_PATH}/bin/armclang)
set(CMAKE_CXX_COMPILER ${COMPILER_PATH}/bin/armclang)

set(COMPILER_FLAGS "-mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Wall -Wno-comment --target=arm-arm-none-eabi")

set(CMAKE_ASM_FLAGS_INIT "--cpu Cortex-M4.fp.sp")
set(CMAKE_ASM_FLAGS ${CMAKE_ASM_FLAGS_INIT} CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS_INIT "${COMPILER_FLAGS}")
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS_INIT} CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_INIT "${COMPILER_FLAGS}")
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS_INIT} CACHE STRING "" FORCE)

include_directories(${COMPILER_PATH}/include)
# include_directories(${COMPILER_PATH}/include/libcxx)

# ------ Linking --------

# set(CMAKE_EXE_LINKER_FLAGS_INIT "${COMPILER_FLAGS}")
# unset(CMAKE_EXE_LINKER_FLAGS CACHE)
# set(CMAKE_EXE_LINKER_FLAGS "${COMPILER_FLAGS}" CACHE STRING "" FORCE)

# search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)