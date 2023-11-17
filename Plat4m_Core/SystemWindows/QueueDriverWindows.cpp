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
// Copyright (c) 2018-2023 Benjamin Minerd
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
/// @file QueueDriverWindows.cpp
/// @author Ben Minerd
/// @date 2/23/2018
/// @brief QueueDriverWindows class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/SystemWindows/QueueDriverWindows.h>
#include <Plat4m_Core/SystemWindows/ThreadWindows.h>

using Plat4m::QueueDriverWindows;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QueueDriverWindows::QueueDriverWindows(Thread& thread) :
    QueueDriver(),
    myThreadId(0)
{
    ThreadWindows& threadWindows = static_cast<ThreadWindows&>(thread);
    myThreadId = threadWindows.getThreadId();
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QueueDriverWindows::~QueueDriverWindows()
{
}

//------------------------------------------------------------------------------
// Public virtual methods overridden for QueueDriver
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
uint32_t QueueDriverWindows::driverGetSize()
{
//      return ((uint32_t) uxQueueMessagesWaiting(myQueueHandle));
    return 0;
}

//------------------------------------------------------------------------------
uint32_t QueueDriverWindows::driverGetSizeFast()
{
//      return ((uint32_t) uxQueueMessagesWaitingFromISR(myQueueHandle));
    return 0;
}

//------------------------------------------------------------------------------
bool QueueDriverWindows::driverEnqueue(const void* value)
{
    uint8_t convertedValue = *(static_cast<const uint8_t*>(value));

    return (PostThreadMessage(myThreadId,
                              0x400,
                              *(static_cast<const uint8_t*>(value)),
                              0));
}

//------------------------------------------------------------------------------
bool QueueDriverWindows::driverEnqueueFast(const void* value)
{
    return (driverEnqueue(value));
}

//--------------------------------------------------------------------------
bool QueueDriverWindows::driverDequeue(void* value)
{
    MSG message;
    bool returnValue = GetMessage(&message, NULL, 0, 0);

    *(static_cast<uint8_t*>(value)) = message.wParam;

    return returnValue;
}

//--------------------------------------------------------------------------
bool QueueDriverWindows::driverDequeueFast(void* value)
{
    return (driverDequeue(value));
}

//--------------------------------------------------------------------------
void QueueDriverWindows::driverClear()
{
//      xQueueReset(myQueueHandle);
}
