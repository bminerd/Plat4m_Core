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
/// @file Queue.h
/// @author Ben Minerd
/// @date 8/11/2017
/// @brief Queue class header file.
///

#ifndef PLAT4M_QUEUE_H
#define PLAT4M_QUEUE_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>

#include <Plat4m_Core/QueueDriver.h>
#include <Plat4m_Core/Thread.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

template <typename T>
class Queue
{
public:

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    Queue(QueueDriver& driver, Thread& thread) :
        myDriver(driver),
        myThread(thread)
    {
    }

    //--------------------------------------------------------------------------
    // Public virtual constructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual ~Queue()
    {
        myDriver.~QueueDriver();
    }

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    std::uint32_t getSize()
    {
        return (myDriver.driverGetSize());
    }

    //--------------------------------------------------------------------------
    std::uint32_t getSizeFast()
    {
        return (myDriver.driverGetSizeFast());
    }

    //--------------------------------------------------------------------------
    bool enqueue(const T& value)
    {
        return (myDriver.driverEnqueue(static_cast<const void*>(&value)));
    }

    //--------------------------------------------------------------------------
    bool enqueueFast(const T& value)
    {
        return (myDriver.driverEnqueueFast(static_cast<const void*>(&value)));
    }

    //--------------------------------------------------------------------------
    bool dequeue(T& value)
    {
        myThread.policyNotifyBlocked(true);

        bool returnValue = myDriver.driverDequeue(static_cast<void*>(&value));

        myThread.policyNotifyBlocked(false);

        return returnValue;
    }

    //--------------------------------------------------------------------------
    bool dequeueFast(T& value)
    {
        myThread.policyNotifyBlocked(true);

        bool returnValue = myDriver.driverDequeueFast(
                                                    static_cast<void*>(&value));

        myThread.policyNotifyBlocked(false);

        return returnValue;
    }

    //--------------------------------------------------------------------------
    void clear()
    {
        myDriver.driverClear();
    }

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    QueueDriver& myDriver;

    Thread& myThread;
};

}; // namespace Plat4m

#endif // PLAT4M_QUEUE_H
