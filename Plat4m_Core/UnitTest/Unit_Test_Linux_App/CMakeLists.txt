cmake_minimum_required (VERSION 2.6)
project (Unit_Test_Linux_App C CXX ASM)

include(CMakePrintHelpers)
cmake_print_variables(CMAKE_BUILD_TYPE)
cmake_print_variables(CMAKE_TOOLCHAIN_FILE)

set(PLAT4M_CORE_DIR ${PROJECT_SOURCE_DIR}/../../../Plat4m_Core)

set(COMPILER_FLAGS "-O0 -g -pthread -Wall -Wno-comment")

set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} ${COMPILER_FLAGS}")
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} ${COMPILER_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMPILER_FLAGS}")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Xlinker -Map=${PROJECT_NAME}.map")

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PLAT4M_CORE_DIR}/../)

set(source_files ${PROJECT_SOURCE_DIR}/main.cpp
                 ${PROJECT_SOURCE_DIR}/ApplicationUnitTestLinuxApp.cpp
                 ${PLAT4M_CORE_DIR}/UnitTest/ApplicationUnitTestApp.cpp
                 ${PLAT4M_CORE_DIR}/Application.cpp
                 ${PLAT4M_CORE_DIR}/Array.h
                 ${PLAT4M_CORE_DIR}/Buffer.h
                 ${PLAT4M_CORE_DIR}/ByteArray.cpp
                 ${PLAT4M_CORE_DIR}/Module.cpp
                 ${PLAT4M_CORE_DIR}/System.cpp
                 ${PLAT4M_CORE_DIR}/Processor.cpp
                 ${PLAT4M_CORE_DIR}/AllocationMemory.cpp
                 ${PLAT4M_CORE_DIR}/Thread.cpp
                 ${PLAT4M_CORE_DIR}/ThreadPolicy.cpp
                 ${PLAT4M_CORE_DIR}/ThreadPolicyManager.cpp
                 ${PLAT4M_CORE_DIR}/Mutex.cpp
                 ${PLAT4M_CORE_DIR}/WaitCondition.cpp
                 ${PLAT4M_CORE_DIR}/QueueDriver.cpp
                 ${PLAT4M_CORE_DIR}/Semaphore.cpp
                 ${PLAT4M_CORE_DIR}/Base.cpp
                 ${PLAT4M_CORE_DIR}/ErrorBase.cpp
                 ${PLAT4M_CORE_DIR}/MemoryManager.cpp
                 ${PLAT4M_CORE_DIR}/ErrorManager.cpp
                 ${PLAT4M_CORE_DIR}/Printer.cpp
                 ${PLAT4M_CORE_DIR}/PrinterConsole.cpp
                 ${PLAT4M_CORE_DIR}/GlobalErrorPrinter.cpp
                 ${PLAT4M_CORE_DIR}/UnitTest/UnitTest.cpp
                 ${PLAT4M_CORE_DIR}/UnitTest/ArrayUnitTest.cpp
                 ${PLAT4M_CORE_DIR}/UnitTest/ArrayNUnitTest.cpp
                 ${PLAT4M_CORE_DIR}/UnitTest/BufferUnitTest.cpp
                 ${PLAT4M_CORE_DIR}/UnitTest/ByteArrayUnitTest.cpp
                 ${PLAT4M_CORE_DIR}/UnitTest/ModuleUnitTest.cpp
                 ${PLAT4M_CORE_DIR}/UnitTest/TimeUnitTest.cpp
                 ${PLAT4M_CORE_DIR}/UnitTest/ListUnitTest.cpp
                 ${PLAT4M_CORE_DIR}/UnitTest/ErrorUnitTest.cpp
                 ${PLAT4M_CORE_DIR}/UnitTest/QuantityUnitTest.cpp
                 ${PLAT4M_CORE_DIR}/Linux/SystemLinux.cpp
                 ${PLAT4M_CORE_DIR}/Linux/ProcessorLinux.cpp
                 ${PLAT4M_CORE_DIR}/Posix/ThreadPosix.cpp
                 ${PLAT4M_CORE_DIR}/Posix/MutexPosix.cpp
                 ${PLAT4M_CORE_DIR}/Posix/WaitConditionPosix.cpp
                 ${PLAT4M_CORE_DIR}/Posix/QueueDriverPosix.cpp
                 ${PLAT4M_CORE_DIR}/Posix/SemaphorePosix.cpp)

add_executable(${PROJECT_NAME} ${source_files})
target_link_libraries(${PROJECT_NAME} rt)
