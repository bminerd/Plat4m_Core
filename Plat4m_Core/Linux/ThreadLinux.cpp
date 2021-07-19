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
/// @file ThreadLinux.cpp
/// @author Ben Minerd
/// @date 5/26/2019
/// @brief ThreadLinux class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <iostream>
#include <unistd.h>

#include <Plat4m_Core/Linux/ThreadLinux.h>
#include <Plat4m_Core/System.h>

using Plat4m::ThreadLinux;
using Plat4m::Module;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ThreadLinux::ThreadLinux(RunCallback& callback, const TimeMs periodMs) :
    Thread(callback, periodMs),
	myThreadHandle(0),
	myMutexHandle(PTHREAD_MUTEX_INITIALIZER),
	myConditionHandle(PTHREAD_COND_INITIALIZER),
	myNextCallTimeMs(0),
    myShouldExit(false)
{
    int returnValue = pthread_create(&myThreadHandle,
					 				 NULL,
									 &threadCallback,
									 this);

    if (returnValue != 0)
    {
        while (true)
        {
            // Lock up, unable to create thread
        }
    }
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ThreadLinux::~ThreadLinux()
{
    myShouldExit = true;
    pthread_cond_signal(&myConditionHandle);
    int returnValue = pthread_join(myThreadHandle, NULL);
}

//------------------------------------------------------------------------------
// Private static methods implemented from Thread
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void* ThreadLinux::threadCallback(void* arg)
{
	ThreadLinux* thread = static_cast<ThreadLinux*>(arg);
	thread->myNextCallTimeMs = thread->getPeriodMs();

	while (!(thread->myShouldExit)) // Loop forever
	{
		if (!(thread->isEnabled()))
		{
			pthread_mutex_lock(&(thread->myMutexHandle));
			pthread_cond_wait(&(thread->myConditionHandle),
								&(thread->myMutexHandle));
			pthread_mutex_unlock(&(thread->myMutexHandle));
		}

        if (thread->myShouldExit)
        {
            pthread_exit(NULL);

            break;
        }

		TimeMs periodMs = thread->getPeriodMs();

		if (periodMs != 0)
		{
			TimeMs sleepTimeMs = thread->getPeriodMs();

			struct timespec timeSpec;
			timeSpec.tv_sec = sleepTimeMs / 1000;
			timeSpec.tv_nsec = (sleepTimeMs % 1000) * 1000000;
			nanosleep(&timeSpec, NULL);
		}

		thread->run();

		thread->myNextCallTimeMs += periodMs;
	}

	return 0;
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error ThreadLinux::driverSetEnabled(const bool enabled)
{
	if (enabled)
	{
		pthread_cond_signal(&myConditionHandle);
	}

	return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods implemented from Thread
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ThreadLinux::driverSetPeriodMs(const TimeMs periodMs)
{
    // Do nothing
}

//------------------------------------------------------------------------------
uint32_t ThreadLinux::driverSetPriority(const uint32_t priority)
{
	return 0;
}
