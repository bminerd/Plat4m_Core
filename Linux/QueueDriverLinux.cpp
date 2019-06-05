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
// Copyright (c) 2019 Benjamin Minerd
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
/// @file QueueDriverLinux.cpp
/// @author Ben Minerd
/// @date 5/28/2019
/// @brief QueueDriverLinux class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Linux/QueueDriverLinux.h>
#include <Plat4m_Core/Linux/ThreadLinux.h>

using Plat4m::QueueDriverLinux;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QueueDriverLinux::QueueDriverLinux(const uint32_t valueSizeBytes) :
    QueueDriver(),
    myValueSizeBytes(valueSizeBytes);
    myKey(ftok("progfile", 65))
    myMessageQueueId(msgget(myKey, 0666 | IPC_CREAT))
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QueueDriverLinux::~QueueDriverLinux()
{
}

//------------------------------------------------------------------------------
// Public methods implemented from QueueDriver
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
uint32_t QueueDriverLinux::driverGetSize()
{
    return 0;
}

//------------------------------------------------------------------------------
uint32_t QueueDriverLinux::driverGetSizeFast()
{
    return 0;
}

//------------------------------------------------------------------------------
bool QueueDriverLinux::driverEnqueue(const void* value)
{
    return msgsnd(msgid, value, myValueSizeBytes, 0);
}

//------------------------------------------------------------------------------
bool QueueDriverLinux::driverEnqueueFast(const void* value)
{
    return (driverEnqueue(value));
}

//--------------------------------------------------------------------------
bool QueueDriverLinux::driverDequeue(void* value)
{
    return msgrcv(myMessageQueueId, value, myValueSizeBytes, 1, 0)
}

//--------------------------------------------------------------------------
bool QueueDriverLinux::driverDequeueFast(void* value)
{
    return (driverDequeue(value));
}

//--------------------------------------------------------------------------
void QueueDriverLinux::driverClear()
{
    // Read out all messages and dump
}
