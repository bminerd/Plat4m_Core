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
/// @file WaitConditionFreeRtos.cpp
/// @author Ben Minerd
/// @date 12/28/2017
/// @brief WaitConditionFreeRtos class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/SystemFreeRtos/WaitConditionFreeRtos.h>
#include <Plat4m_Core/SystemFreeRtos/ThreadFreeRtos.h>

using Plat4m::WaitConditionFreeRtos;
using Plat4m::WaitCondition;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
WaitConditionFreeRtos::WaitConditionFreeRtos(Thread& thread) :
    WaitCondition(),
    myTaskHandle(0)
{
    ThreadFreeRtos& threadFreeRtos = static_cast<ThreadFreeRtos&>(thread);
    myTaskHandle = threadFreeRtos.getTaskHandle();
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
WaitConditionFreeRtos::~WaitConditionFreeRtos()
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void WaitConditionFreeRtos::notifyFast()
{
    BaseType_t higherPriorityTaskWasWoken = pdFALSE;
    vTaskNotifyGiveFromISR(myTaskHandle, &higherPriorityTaskWasWoken);

    portYIELD_FROM_ISR(higherPriorityTaskWasWoken);
}

//------------------------------------------------------------------------------
// Private virtual methods overridden for WaitCondition
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
WaitCondition::Error WaitConditionFreeRtos::driverWait(const TimeMs waitTimeMs)
{
    ulTaskNotifyTake(pdFALSE, (TickType_t) waitTimeMs);

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
WaitCondition::Error WaitConditionFreeRtos::driverNotify()
{
    xTaskNotifyGive(myTaskHandle);

    return Error(ERROR_CODE_NONE);
}
