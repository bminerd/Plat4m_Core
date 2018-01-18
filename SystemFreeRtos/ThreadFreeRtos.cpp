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
/// @file ThreadFreeRTOS.cpp
/// @author Ben Minerd
/// @date 12/23/2017
/// @brief ThreadFreeRTOS class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/SystemFreeRTOS/ThreadFreeRTOS.h>
#include <Plat4m_Core/System.h>

using Plat4m::ThreadFreeRTOS;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ThreadFreeRTOS::ThreadFreeRTOS(RunCallback& callback, const TimeMs periodMs) :
    Thread(callback, periodMs),
	myTaskHandle(0)
{
	BaseType_t returnValue = xTaskCreate(&ThreadFreeRTOS::taskCallback,
										 "",
										 256,
										 (void*) this,
										 tskIDLE_PRIORITY,
										 &myTaskHadle);

	if (returnValue != pdPASS)
	{
		while (true)
		{
			// Unable to create task, loop forever
		}
	}
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ThreadFreeRTOS::~ThreadFreeRTOS()
{
}

//------------------------------------------------------------------------------
// Private methods implemented from Thread
//------------------------------------------------------------------------------
void ThreadFreeRTOS::taskCallback(void* parameters)
{
	ThreadFreeRTOS* thread = static_cast<ThreadFreeRTOS*>(parameters);
	TickType_t lastWakeTime = xTaskGetTickCount();

	while (true) // Loop forever
	{
		if (thread->getPeriodMs() != 0)
		{
			vTaskDelayUntil(&lastWakeTime, thread->getPeriodMs());
		}

		thread->run();
	}
}

//------------------------------------------------------------------------------
// Private methods implemented from Thread
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ThreadFreeRTOS::driverSetPeriodMs(const TimeMs periodMs)
{
    // Do nothing
}

//------------------------------------------------------------------------------
uint32_t ThreadFreeRTOS::driverSetPriority(const uint32_t priority)
{
	vTaskPrioritySet(myTaskHandle, priority);

	return 0;
}
