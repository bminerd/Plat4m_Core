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
// Copyright (c) 2017 Benjamin Minerd
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

#include <stdint.h>

#include <QueueDriver.h>

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
    Queue(QueueDriver& driver) :
		myDriver(driver)
    {
    }

    //--------------------------------------------------------------------------
    // Public virtual constructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual ~Queue()
    {
    }

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    uint32_t getSize()
    {
    	return (myDriver.driverGetSize());
    }

    //--------------------------------------------------------------------------
    uint32_t getSizeFast()
    {
    	return (myDriver.driverGetSizeFast());
    }

    //--------------------------------------------------------------------------
	bool enqueue(const T& value)
	{
		return (myDriver.driverEnqueue((void*) &value));
	}

    //--------------------------------------------------------------------------
	bool enqueueFast(const T& value)
	{
		return (myDriver.driverEnqueueFast((void*) &value));
	}

	//--------------------------------------------------------------------------
	bool dequeue(T& value)
	{
		return (myDriver.driverDequeue((void*) &value));
	}

	//--------------------------------------------------------------------------
	bool dequeueFast(T& value)
	{
		return (myDriver.driverDequeueFast((void*) &value));
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
};

}; // namespace Plat4m

#endif // PLAT4M_QUEUE_H
