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
/// @file QueueDriverSimulation.h
/// @author Ben Minerd
/// @date 2/7/2024
/// @brief QueueDriverSimulation class header file.
///

#ifndef PLAT4M_QUEUE_DRIVER_SIMULATION_H
#define PLAT4M_QUEUE_DRIVER_SIMULATION_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>

#include <Plat4m_Core/QueueDriver.h>
#include <Plat4m_Core/Thread.h>
#include <Plat4m_Core/Semaphore.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class QueueDriverSimulation : public QueueDriver
{
public:

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    QueueDriverSimulation(const std::uint32_t nValues,
                          const std::uint32_t valueSizeBytes,
                          Thread& thread,
                          Semaphore& threadsNotifiedSemaphore);

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~QueueDriverSimulation();

    //--------------------------------------------------------------------------
    // Public virtual methods overridden for Queue
    //--------------------------------------------------------------------------

    virtual std::uint32_t driverGetSize() override;

    virtual std::uint32_t driverGetSizeFast() override;

    virtual bool driverEnqueue(const void* value) override;

    virtual bool driverEnqueueFast(const void* value) override;

    virtual bool driverDequeue(void* value) override;

    virtual bool driverDequeueFast(void* value) override;

    virtual void driverClear() override;

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    QueueDriver& myQueueDriver;

    Semaphore& myThreadsNotifiedSemaphore;
};

}; // namespace Plat4m

#endif // PLAT4M_QUEUE_DRIVER_SIMULATION_H
