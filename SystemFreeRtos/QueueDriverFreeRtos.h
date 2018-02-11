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
// Copyright (c) 2018 Benjamin Minerd
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
/// @file QueueDriverFreeRtos.h
/// @author Ben Minerd
/// @date 1/17/2018
/// @brief QueueDriverFreeRtos class header file.
///

#ifndef PLAT4M_QUEUE_DRIVER_FREE_RTOS_H
#define PLAT4M_QUEUE_DRIVER_FREE_RTOS_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <stdint.h>

#include <FreeRTOS/Source/include/FreeRTOS.h>
#include <FreeRTOS/Source/include/queue.h>

#include <Plat4m_Core/QueueDriver.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

template <uint32_t nBytes>
class QueueDriverFreeRtos : public QueueDriver
{
public:

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
    QueueDriverFreeRtos(const uint32_t nValues, const uint32_t valueSizeBytes) :
    	QueueDriver(),
		myQueueHandle(xQueueCreate(nValues, valueSizeBytes))
	{
	}

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual ~QueueDriverFreeRtos()
    {
    }

    //--------------------------------------------------------------------------
    // Public methods implemented from QueueDriver
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
	uint32_t driverGetSize()
	{
		return ((uint32_t) uxQueueMessagesWaiting(myQueueHandle));
	}

	//--------------------------------------------------------------------------
	uint32_t driverGetSizeFast()
	{
		return ((uint32_t) uxQueueMessagesWaitingFromISR(myQueueHandle));
	}

	//--------------------------------------------------------------------------
	bool driverEnqueue(const void* value)
	{
		return ((bool) xQueueSendToBack(myQueueHandle, value, 0));
	}

	//--------------------------------------------------------------------------
	bool driverEnqueueFast(const void* value)
	{
		return ((bool) xQueueSendToBackFromISR(myQueueHandle, value, 0));
	}

	//--------------------------------------------------------------------------
	bool driverDequeue(void* value)
	{
		return ((bool) xQueueReceive(myQueueHandle, value, 0xFFFFFFFF));
	}

	//--------------------------------------------------------------------------
	bool driverDequeueFast(void* value)
	{
		BaseType_t xTaskWokenByReceive = pdFALSE;

		return ((bool) xQueueReceiveFromISR(myQueueHandle,
											value,
											&xTaskWokenByReceive));
	}

	//--------------------------------------------------------------------------
	void driverClear()
	{
		xQueueReset(myQueueHandle);
	}

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

	QueueHandle_t myQueueHandle;
};

}; // namespace Plat4m

#endif // PLAT4M_QUEUE_DRIVER_FREE_RTOS_H
