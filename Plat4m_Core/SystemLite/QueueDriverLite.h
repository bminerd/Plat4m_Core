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
/// @file QueueDriverLite.h
/// @author Ben Minerd
/// @date 11/20/2017
/// @brief QueueDriverLite class header file.
///

#ifndef PLAT4M_QUEUE_DRIVER_LITE_H
#define PLAT4M_QUEUE_DRIVER_LITE_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <stdint.h>

#include <Plat4m_Core/QueueDriver.h>
#include <Plat4m_Core/ArrayN.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

template <uint32_t nValues>
class QueueDriverLite : public QueueDriver
{
public:

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
    QueueDriverLite() :
    	QueueDriver(),
		myValues()
	{
	}

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual ~QueueDriverLite()
    {
    }

    //--------------------------------------------------------------------------
    // Public methods implemented from QueueDriver
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
	uint32_t driverGetSize()
	{
		return 0;
	}

	//--------------------------------------------------------------------------
	uint32_t driverGetSizeFast()
	{
		return 0;
	}

	//--------------------------------------------------------------------------
	bool driverEnqueue(const void* value)
	{
		return true;
	}

	//--------------------------------------------------------------------------
	bool driverEnqueueFast(const void* value)
	{
		return true;
	}

	//--------------------------------------------------------------------------
	bool driverDequeue(void* value)
	{
		return true;
	}

	//--------------------------------------------------------------------------
	bool driverDequeueFast(void* value)
	{
		return true;
	}

	//--------------------------------------------------------------------------
	void driverClear()
	{

	}

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

	ArrayN<uint8_t, nValues> myValues;
};

}; // namespace Plat4m

#endif // PLAT4M_QUEUE_DRIVER_LITE_H
