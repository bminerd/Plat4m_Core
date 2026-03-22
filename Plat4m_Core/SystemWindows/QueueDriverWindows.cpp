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
// Copyright (c) 2018-2024 Benjamin Minerd
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
#include <Plat4m_Core/MemoryAllocator.h>
#include <Plat4m_Core/System.h>

using namespace Plat4m;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QueueDriverWindows::QueueDriverWindows(const std::uint32_t nValues,
                                       const std::uint32_t valueSizeBytes,
                                       Thread& thread) :
    QueueDriver(),
    myThreadId(0),
    myBytes(static_cast<std::uint8_t*>(MemoryAllocator::allocateArray(
                                                    nValues * valueSizeBytes))),
    myByteBuffer(myBytes, nValues, valueSizeBytes),
    myBufferMutex(System::createMutex(thread))
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
std::uint32_t QueueDriverWindows::driverGetSize()
{
    return (myByteBuffer.count());
}

//------------------------------------------------------------------------------
std::uint32_t QueueDriverWindows::driverGetSizeFast()
{
    return (driverGetSize());
}

//------------------------------------------------------------------------------
bool QueueDriverWindows::driverEnqueue(const void* value)
{
    myBufferMutex.lock();

    myByteBuffer.write(value);

    myBufferMutex.unlock();

    return (PostThreadMessage(myThreadId, 0x400, 0, 0));
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
    bool getMessageSucceeded = GetMessage(&message, NULL, 0, 0);

    bool readBufferSucceeded = false;

    if (getMessageSucceeded)
    {
        myBufferMutex.lock();

        readBufferSucceeded = myByteBuffer.read(value);

        myBufferMutex.unlock();
    }

    return (getMessageSucceeded && readBufferSucceeded);
}

//--------------------------------------------------------------------------
bool QueueDriverWindows::driverDequeueFast(void* value)
{
    return (driverDequeue(value));
}

//--------------------------------------------------------------------------
void QueueDriverWindows::driverClear()
{
    MSG message;

    while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
    {
        myByteBuffer.emptyRead();
    }
}
