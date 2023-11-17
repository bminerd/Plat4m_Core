cmake_minimum_required (VERSION 2.6)

set(CMAKE_SYSTEM_NAME      Generic)
set(CMAKE_SYSTEM_PROCESSOR Cortex-M33)

set(COMPILER_NAME "GCC" CACHE STRING "" FORCE)
set(COMPILER_PATH /usr/local/gcc-arm-none-eabi-10-2020-q4-major)

set(CMAKE_C_COMPILER ${COMPILER_PATH}/bin/arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER ${COMPILER_PATH}/bin/arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER ${COMPILER_PATH}/bin/arm-none-eabi-gcc)

set(COMPILER_FLAGS "-mthumb -mcpu=cortex-m33 -mfloat-abi=hard -mfpu=fpv5-sp-d16 -O0 -g3 -fstack-usage -Wall")

set(CMAKE_ASM_FLAGS_INIT "${COMPILER_FLAGS}")
set(CMAKE_ASM_FLAGS ${CMAKE_ASM_FLAGS_INIT} CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS_INIT "${COMPILER_FLAGS}")
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS_INIT} CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_INIT "${COMPILER_FLAGS}")
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS_INIT} CACHE STRING "" FORCE)

include_directories(${COMPILER_PATH}/arm-none-eabi/include)

# ------ Linking --------

set(CMAKE_EXE_LINKER_FLAGS_INIT "${COMPILER_FLAGS} -specs=nosys.specs -static -Wl,-cref,-u,Reset_Handler \"-Wl,-Map=${PROJECT_NAME}.map\" -Wl,--gc-sections -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group")
unset(CMAKE_EXE_LINKER_FLAGS CACHE)
set(CMAKE_EXE_LINKER_FLAGS "${COMPILER_FLAGS} -specs=nosys.specs -static -Wl,-cref,-u,Reset_Handler \"-Wl,-Map=${PROJECT_NAME}.map\" -Wl,--gc-sections -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group" CACHE STRING "" FORCE)

# search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)