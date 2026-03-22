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
// Copyright (c) 2019-2024 Benjamin Minerd
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
/// @file ThreadPosix.cpp
/// @author Ben Minerd
/// @date 5/26/2019
/// @brief ThreadPosix class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <iostream>
#include <unistd.h>
#include <cstring>

#include <Plat4m_Core/Posix/ThreadPosix.h>
#include <Plat4m_Core/System.h>

using namespace Plat4m;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ThreadPosix::ThreadPosix(RunCallback& callback,
                         const TimeMs periodMs,
                         const char* name) :
    Thread(callback, periodMs, name),
    myThreadHandle(0),
    myMutexHandle(),
    myConditionHandle(),
    myNextCallTimeMs(0),
    myIsEnabled(false),
    myShouldExit(false)
{
    int returnValue = 0;

    returnValue = pthread_mutex_init(&myMutexHandle, 0);

    returnValue = pthread_cond_init(&myConditionHandle, 0);

    returnValue = pthread_create(&myThreadHandle,
                                 0,
                                 &threadCallback,
                                 this);

    if (returnValue != 0)
    {
        PLAT4M_REPORT_ERROR(Thread::Error,
                            Thread::ERROR_CODE_INSTANTIATION_FAILED,
                            ErrorBase::SEVERITY_CRITICAL,
                            this);
    }

    char newName[20];
    memset(newName, 0, sizeof(newName));

    if (isNullPointer(name))
    {
        std::strncpy(newName, "(Noname Thread)", 16);
    }
    else
    {
        std::strncpy(newName, name, 16);
    }

    returnValue = pthread_setname_np(myThreadHandle, newName);
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ThreadPosix::~ThreadPosix()
{
    pthread_mutex_lock(&myMutexHandle);
    myShouldExit = true;
    pthread_cond_broadcast(&myConditionHandle);
    pthread_mutex_unlock(&myMutexHandle);

    pthread_cancel(myThreadHandle);
    pthread_join(myThreadHandle, NULL);

    pthread_mutex_destroy(&myMutexHandle);
    pthread_cond_destroy(&myConditionHandle);
}

//------------------------------------------------------------------------------
// Private static methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void* ThreadPosix::threadCallback(void* arg)
{
    ThreadPosix* thread = static_cast<ThreadPosix*>(arg);
    thread->myNextCallTimeMs = thread->getPeriodMs();

    while (true) // Loop forever
    {
        int returnValue = 0;
        returnValue = pthread_mutex_lock(&(thread->myMutexHandle));

        if (!(thread->myIsEnabled))
        {
            returnValue = pthread_cond_wait(&(thread->myConditionHandle),
                                            &(thread->myMutexHandle));
        }

        if (thread->myShouldExit)
        {
            returnValue = pthread_mutex_unlock(&(thread->myMutexHandle));
            pthread_exit(NULL);

            break;
        }

        returnValue = pthread_mutex_unlock(&(thread->myMutexHandle));

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
// Private virtual methods overridden for Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error ThreadPosix::driverSetEnabled(const bool enabled)
{
    pthread_mutex_lock(&myMutexHandle);

    myIsEnabled = enabled;

    if (enabled)
    {
        pthread_cond_broadcast(&myConditionHandle);
    }

    pthread_mutex_unlock(&myMutexHandle);

    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private virtual methods overridden for Thread
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ThreadPosix::driverSetPeriodMs(const TimeMs periodMs)
{
    // Do nothing
}

//------------------------------------------------------------------------------
std::uint32_t ThreadPosix::driverSetPriority(const std::uint32_t priority)
{
    return 0;
}
