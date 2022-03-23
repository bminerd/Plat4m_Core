cmake_minimum_required (VERSION 2.6)
project (Unit_Test_Linux_App C CXX ASM)

set(PLAT4M_CORE_DIR ${PROJECT_SOURCE_DIR}/../../../Plat4m_Core)

set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -pthread")
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")

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
                 ${PLAT4M_CORE_DIR}/TimeStamp.cpp
                 ${PLAT4M_CORE_DIR}/UnitTest/UnitTest.cpp
                 ${PLAT4M_CORE_DIR}/UnitTest/ArrayUnitTest.cpp
                 ${PLAT4M_CORE_DIR}/UnitTest/ArrayNUnitTest.cpp
                 ${PLAT4M_CORE_DIR}/UnitTest/BufferUnitTest.cpp
                 ${PLAT4M_CORE_DIR}/UnitTest/ByteArrayUnitTest.cpp
                 ${PLAT4M_CORE_DIR}/UnitTest/ModuleUnitTest.cpp
                 ${PLAT4M_CORE_DIR}/Linux/SystemLinux.cpp
                 ${PLAT4M_CORE_DIR}/Linux/ProcessorLinux.cpp
                 ${PLAT4M_CORE_DIR}/Linux/ThreadLinux.cpp
                 ${PLAT4M_CORE_DIR}/Linux/MutexLinux.cpp
                 ${PLAT4M_CORE_DIR}/Linux/WaitConditionLinux.cpp
                 ${PLAT4M_CORE_DIR}/Linux/QueueDriverLinux.cpp
                 ${PLAT4M_CORE_DIR}/Linux/SemaphoreLinux.cpp)

include(CTest)

add_executable(Unit_Test_Linux_App ${source_files})
add_test(Unit_Test_Linux_App Unit_Test_Linux_App)
set_tests_properties(Unit_Test_Linux_App PROPERTIES
                     FAIL_REGULAR_EXPRESSION ".*FAIL.*")
