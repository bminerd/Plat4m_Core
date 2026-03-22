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
// Copyright (c) 2024 Benjamin Minerd
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
/// @file QueueDriverSimulation.cpp
/// @author Ben Minerd
/// @date 2/7/2024
/// @brief QueueDriverSimulation class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Simulation/QueueDriverSimulation.h>
#include <Plat4m_Core/System.h>

using namespace Plat4m;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QueueDriverSimulation::QueueDriverSimulation(
                                          const std::uint32_t nValues,
                                          const std::uint32_t valueSizeBytes,
                                          Thread& thread,
                                          Semaphore& threadsNotifiedSemaphore) :
    QueueDriver(),
    myQueueDriver(System::createQueueDriver(nValues,
                                            valueSizeBytes,
                                            thread,
                                            false)),
    myThreadsNotifiedSemaphore(threadsNotifiedSemaphore)
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QueueDriverSimulation::~QueueDriverSimulation()
{
    myQueueDriver.~QueueDriver();
}

//------------------------------------------------------------------------------
// Public virtual methods overridden for QueueDriver
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
std::uint32_t QueueDriverSimulation::driverGetSize()
{
    return (myQueueDriver.driverGetSize());
}

//------------------------------------------------------------------------------
std::uint32_t QueueDriverSimulation::driverGetSizeFast()
{
    return (myQueueDriver.driverGetSizeFast());
}

//------------------------------------------------------------------------------
bool QueueDriverSimulation::driverEnqueue(const void* value)
{
    bool returnValue = myQueueDriver.driverEnqueue(value);

    myThreadsNotifiedSemaphore.post();

    return returnValue;
}

//------------------------------------------------------------------------------
bool QueueDriverSimulation::driverEnqueueFast(const void* value)
{
    bool returnValue = myQueueDriver.driverEnqueueFast(value);

    myThreadsNotifiedSemaphore.post();

    return returnValue;
}

//------------------------------------------------------------------------------
bool QueueDriverSimulation::driverDequeue(void* value)
{
    return (myQueueDriver.driverDequeue(value));
}

//------------------------------------------------------------------------------
bool QueueDriverSimulation::driverDequeueFast(void* value)
{
    return (myQueueDriver.driverDequeueFast(value));
}

//------------------------------------------------------------------------------
void QueueDriverSimulation::driverClear()
{
    myQueueDriver.driverClear();
}
