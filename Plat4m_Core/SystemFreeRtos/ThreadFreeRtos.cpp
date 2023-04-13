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
/// @file ThreadFreeRtos.cpp
/// @author Ben Minerd
/// @date 12/23/2017
/// @brief ThreadFreeRtos class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/SystemFreeRtos/ThreadFreeRtos.h>
#include <Plat4m_Core/System.h>

using Plat4m::ThreadFreeRtos;
using Plat4m::Module;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ThreadFreeRtos::ThreadFreeRtos(RunCallback& callback,
                               const TimeMs periodMs,
                               const uint32_t nStackBytes,
                               const char* name) :
    Thread(callback, periodMs, name),
    myTaskHandle(0)
{
    uint32_t nTotalStackBytes;

    if (nStackBytes == 0)
    {
        nTotalStackBytes = 256;
    }
    else
    {
        nTotalStackBytes = nStackBytes;
    }

    const char* newName = name;

    if (isNullPointer(newName))
    {
        newName = "(Unnamed Thread)";
    }

    BaseType_t returnValue = xTaskCreate(&ThreadFreeRtos::taskCallback,
                                         newName,
                                         nTotalStackBytes / 4,
                                         (void*) this,
                                         tskIDLE_PRIORITY,
                                         &myTaskHandle);

    if (returnValue != pdPASS)
    {
        while (true)
        {
            // Unable to create task, loop forever
        }
    }

    vTaskSuspend(myTaskHandle);
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ThreadFreeRtos::~ThreadFreeRtos()
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
TaskHandle_t ThreadFreeRtos::getTaskHandle() const
{
    return myTaskHandle;
}

//------------------------------------------------------------------------------
// Private static methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ThreadFreeRtos::taskCallback(void* parameter)
{
    ThreadFreeRtos* thread = static_cast<ThreadFreeRtos*>(parameter);
    TickType_t lastWakeTime = xTaskGetTickCount();

    while (true) // Loop forever
    {
        if (thread->getPeriodMs() != 0)
        {
            vTaskDelayUntil(&lastWakeTime, thread->getPeriodMs());
        }

        thread->run();
    }
}

//------------------------------------------------------------------------------
// Private virtual methods overridden for Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error ThreadFreeRtos::driverSetEnabled(const bool enabled)
{
    if (enabled)
    {
        vTaskResume(myTaskHandle);
    }
    else
    {
        vTaskSuspend(myTaskHandle);
    }

    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private virtual methods overridden for Thread
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ThreadFreeRtos::driverSetPeriodMs(const TimeMs periodMs)
{
    // Do nothing
}

//------------------------------------------------------------------------------
uint32_t ThreadFreeRtos::driverSetPriority(const uint32_t priority)
{
    vTaskPrioritySet(myTaskHandle, priority);

    return 0;
}
