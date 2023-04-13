//------------------------------------------------------------------------------
//       _______    __                           ___
//      ||  ___ \  || |             __          //  |
//      || |  || | || |   _______  || |__      //   |    _____  ___
//      || |__|| | || |  // ___  | ||  __|    // _  |   ||  _ \/ _ \
//      ||  ____/  || | || |  || | || |      // /|| |   || |\\  /\\ \
//      || |       || | || |__|| | || |     // /_|| |_  || | || | || |
//      || |       || |  \\____  | || |__  //_____   _| || | || | || |
//      ||_|       ||_|       ||_|  \\___|       ||_|   ||_| ||_| ||_|
//
//
// The MIT License (MIT)
//
// Copyright (c) 2017-2023 Benjamin Minerd
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------------

///
/// @file SystemFreeRtos.cpp
/// @author Ben Minerd
/// @date 12/28/2017
/// @brief SystemFreeRtos class.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <FreeRTOS-Kernel/include/FreeRTOS.h>
#include <FreeRTOS-Kernel/include/task.h>

#include <Plat4m_Core/SystemFreeRtos/SystemFreeRtos.h>
#include <Plat4m_Core/SystemFreeRtos/ThreadFreeRtos.h>
#include <Plat4m_Core/SystemFreeRtos/MutexFreeRtos.h>
#include <Plat4m_Core/SystemFreeRtos/WaitConditionFreeRtos.h>
#include <Plat4m_Core/SystemFreeRtos/QueueDriverFreeRtos.h>
#include <Plat4m_Core/SystemFreeRtos/SemaphoreFreeRtos.h>
#include <Plat4m_Core/Processor.h>
#include <Plat4m_Core/MemoryAllocator.h>

using namespace std;
using namespace Plat4m;

//------------------------------------------------------------------------------
// External functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
extern "C" void vApplicationTickHook(void)
{

}

//------------------------------------------------------------------------------
extern "C" void vApplicationIdleHook(void)
{

}

//------------------------------------------------------------------------------
extern "C" void vApplicationStackOverflowHook(TaskHandle_t xTask,
                                              char * pcTaskName)
{

}

//------------------------------------------------------------------------------
// Public virtual methods overridden for System
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Thread& SystemFreeRtos::driverCreateThread(Thread::RunCallback& callback,
                                       	   const TimeMs periodMs,
                                       	   const uint32_t nStackBytes,
                                           const bool isSimulated,
                                           const char* name)
{
    return *(MemoryAllocator::allocate<ThreadFreeRtos>(callback,
                                                       periodMs,
                                                       nStackBytes,
                                                       name));
}

//------------------------------------------------------------------------------
Mutex& SystemFreeRtos::driverCreateMutex(Thread& thread)
{
    return *(MemoryAllocator::allocate<MutexFreeRtos>());
}

//------------------------------------------------------------------------------
WaitCondition& SystemFreeRtos::driverCreateWaitCondition(Thread& thread)
{
    return *(MemoryAllocator::allocate<WaitConditionFreeRtos>(thread));
}

//------------------------------------------------------------------------------
QueueDriver& SystemFreeRtos::driverCreateQueueDriver(
												  const uint32_t nValues,
												  const uint32_t valueSizeBytes,
												  Thread& thread)
{
	return *(MemoryAllocator::allocate<QueueDriverFreeRtos<0>>(nValues,
                                                               valueSizeBytes));
}

//------------------------------------------------------------------------------
Semaphore& SystemFreeRtos::driverCreateSemaphore(const uint32_t maxValue,
                                                 const uint32_t initialValue)
{
    return *(MemoryAllocator::allocate<SemaphoreFreeRtos>(maxValue,
                                                          initialValue));
}

//------------------------------------------------------------------------------
void SystemFreeRtos::driverRun()
{
    vTaskStartScheduler();
}

//------------------------------------------------------------------------------
Plat4m::TimeMs SystemFreeRtos::driverGetTimeMs()
{
    TimeMs timeMs = ((TimeMs) xTaskGetTickCount());

    if (timeMs < myLastTimeMs)
    {
        // Millisecond timer count has overflowed, approximately every 49.7 days
        myTimeMsRollOverCounter++;
    }

    myLastTimeMs = timeMs;

    return myLastTimeMs;
}

//------------------------------------------------------------------------------
void SystemFreeRtos::driverDelayTimeMs(const TimeMs timeMs)
{
    vTaskDelay((TickType_t) timeMs);
}

//------------------------------------------------------------------------------
void SystemFreeRtos::driverExit()
{
    vTaskEndScheduler();
}

//------------------------------------------------------------------------------
void SystemFreeRtos::driverEnterCriticalSection()
{
    if (Processor::isInterruptActive())
    {
        mySavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();
    }
    else
    {
        taskENTER_CRITICAL();
    }
}

//------------------------------------------------------------------------------
void SystemFreeRtos::driverExitCriticalSection()
{
    if (Processor::isInterruptActive())
    {
        taskEXIT_CRITICAL_FROM_ISR(mySavedInterruptStatus);
    }
    else
    {
        taskEXIT_CRITICAL();
    }
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
std::uint32_t SystemFreeRtos::getTimeMsRollOverCounter()
{
    return myTimeMsRollOverCounter;
}

//------------------------------------------------------------------------------
// Protected constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
SystemFreeRtos::SystemFreeRtos() :
    System(),
    myLastTimeMs(0),
    myTimeMsRollOverCounter(0),
    mySavedInterruptStatus(0)
{
}

//------------------------------------------------------------------------------
// Protected virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
SystemFreeRtos::~SystemFreeRtos()
{
}

//------------------------------------------------------------------------------
// Private virtual methods implemented from System
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Plat4m::TimeUs SystemFreeRtos::driverGetTimeUs()
{
	// Default if not implemented by subclass

    return (driverGetTimeMs() * 1000);
}
