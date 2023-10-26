cmake_minimum_required (VERSION 3.0)

set(CMAKE_SYSTEM_NAME      Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-m4)

set(CMAKE_C_COMPILER   $ENV{GCC_ARM_COMPILER_PATH}/bin/arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER $ENV{GCC_ARM_COMPILER_PATH}/bin/arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER $ENV{GCC_ARM_COMPILER_PATH}/bin/arm-none-eabi-gcc)

set(COMPILER_FLAGS "-mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -g -fstack-usage -Wall")

set(CMAKE_ASM_FLAGS ${COMPILER_FLAGS} CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS ${COMPILER_FLAGS} CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS ${COMPILER_FLAGS} CACHE STRING "" FORCE)
set(CMAKE_EXE_LINKER_FLAGS "${COMPILER_FLAGS} -specs=nosys.specs -Wl,--gc-sections" CACHE STRING "" FORCE)

include_directories(${COMPILER_PATH}/arm-none-eabi/include)

# search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
