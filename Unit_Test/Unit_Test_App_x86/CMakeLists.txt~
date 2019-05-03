cmake_minimum_required (VERSION 2.6)
project (Blinky_STM32F4xx_App C CXX ASM)

set(PLAT4M_CORE_DIR     ${PROJECT_SOURCE_DIR}/../Plat4m_Core)
set(PLAT4M_MATH_DIR     ${PROJECT_SOURCE_DIR}/../Plat4m_Math)
set(PLAT4M_CONTROLS_DIR ${PROJECT_SOURCE_DIR}/../Plat4m_Controls)
set(PLAT4M_CMAKE_DIR    ${PROJECT_SOURCE_DIR}/../Plat4m_CMake)

include(${PLAT4M_CMAKE_DIR}/Plat4mMacros.cmake)

set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -DSTM32F4XX -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000")
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -DSTM32F4XX -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSTM32F4XX -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T\"${PROJECT_SOURCE_DIR}/Atollic/STM32F446RE_FLASH.ld\"")

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/Atollic)
include_directories(${PROJECT_SOURCE_DIR}/../)
include_directories(${PLAT4M_CORE_DIR}/STM32F4xx)
include_directories(${PLAT4M_CORE_DIR}/STM32F4xx/STM32F4xx_DSP_StdPeriph_Lib_V1.0.1/Libraries/CMSIS/Include)
include_directories(${PLAT4M_CORE_DIR}/STM32F4xx/STM32F4xx_DSP_StdPeriph_Lib_V1.0.1/Libraries/CMSIS/Device/ST/STM32F4xx/Include)
include_directories(${PLAT4M_CORE_DIR}/STM32F4xx/STM32F4xx_DSP_StdPeriph_Lib_V1.0.1/Libraries/STM32F4xx_StdPeriph_Driver/inc)

set(source_files main.cpp
                 ${PROJECT_SOURCE_DIR}/Atollic/startup_stm32f446xx.s
                 ${PLAT4M_CORE_DIR}/Module.cpp
                 ${PLAT4M_CORE_DIR}/Processor.cpp
                 ${PLAT4M_CORE_DIR}/GpioPort.h
                 ${PLAT4M_CORE_DIR}/GpioPin.cpp
                 ${PLAT4M_CORE_DIR}/STM32F4xx/ProcessorSTM32F4xx.cpp
                 ${PLAT4M_CORE_DIR}/STM32F4xx/GpioPortSTM32F4xx.cpp
                 ${PLAT4M_CORE_DIR}/STM32F4xx/GpioPinSTM32F4xx.cpp
                 ${PLAT4M_CORE_DIR}/STM32F4xx/STM32F4xx_DSP_StdPeriph_Lib_V1.0.1/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.c
                 ${PLAT4M_CORE_DIR}/STM32F4xx/STM32F4xx_DSP_StdPeriph_Lib_V1.0.1/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.c)

add_executable(Blinky_STM32F4xx_App ${source_files})
