cmake_minimum_required (VERSION 2.6)

set(CMAKE_SYSTEM_NAME      Generic)
set(CMAKE_SYSTEM_PROCESSOR NRF52)

set(COMPILER_PATH /usr/local/gcc-arm-none-eabi-8-2019-q3-update)

set(CMAKE_C_COMPILER ${COMPILER_PATH}/bin/arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER ${COMPILER_PATH}/bin/arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER ${COMPILER_PATH}/bin/arm-none-eabi-gcc)

set(CMAKE_ASM_FLAGS_INIT "-mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -g3 -fstack-usage -Wall -Wno-comment -DNRF52840_XXAA")
set(CMAKE_ASM_FLAGS ${CMAKE_ASM_FLAGS_INIT} CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS_INIT "-mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -g3 -fstack-usage -Wall -Wno-comment -DNRF52840_XXAA")
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS_INIT} CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_INIT "-mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -g3 -fstack-usage -Wall -Wno-comment -DNRF52840_XXAA")
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS_INIT} CACHE STRING "" FORCE)

include_directories(${COMPILER_PATH}/arm-none-eabi/include)

# ------ Linking --------

set(CMAKE_EXE_LINKER_FLAGS_INIT "-mthumb -mcpu=cortex-m4 -specs=nosys.specs -static -Wl,-cref,-u,Reset_Handler \"-Wl,-Map=${PROJECT_NAME}.map\" -Wl,--gc-sections -Wl,--defsym=malloc_getpagesize_P=0x1000 -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group")
unset(CMAKE_EXE_LINKER_FLAGS CACHE)
set(CMAKE_EXE_LINKER_FLAGS "-DNRF52840_XXAA -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -specs=nosys.specs -static -Wl,-cref,-u,Reset_Handler \"-Wl,-Map=${PROJECT_NAME}.map\" -Wl,--gc-sections -Wl,--defsym=malloc_getpagesize_P=0x1000 -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group" CACHE STRING "" FORCE)

# search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)