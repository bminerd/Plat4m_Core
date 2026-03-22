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
/// @file WaitConditionPosix.cpp
/// @author Ben Minerd
/// @date 5/26/2019
/// @brief WaitConditionPosix class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Posix/WaitConditionPosix.h>

using namespace Plat4m;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
WaitConditionPosix::WaitConditionPosix(Thread& thread) :
    WaitCondition(thread),
    myConditionHandle(),
    myMutexHandle(),
    myThreadHandle(0)
{
    int returnValue = 0;

    returnValue = pthread_mutex_init(&myMutexHandle, 0);

    if (returnValue != 0)
    {
        PLAT4M_REPORT_ERROR(WaitCondition::Error,
                            WaitCondition::ERROR_CODE_INSTANTIATION_FAILED,
                            ErrorBase::SEVERITY_CRITICAL,
                            this);
    }

    returnValue = pthread_cond_init(&myConditionHandle, 0);

    if (returnValue != 0)
    {
        PLAT4M_REPORT_ERROR(WaitCondition::Error,
                            WaitCondition::ERROR_CODE_INSTANTIATION_FAILED,
                            ErrorBase::SEVERITY_CRITICAL,
                            this);
    }
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
WaitConditionPosix::~WaitConditionPosix()
{
    pthread_cond_destroy(&myConditionHandle);
    pthread_mutex_destroy(&myMutexHandle);
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void WaitConditionPosix::notifyFast()
{
    pthread_mutex_lock(&myMutexHandle);
    pthread_cond_broadcast(&myConditionHandle);
    pthread_mutex_unlock(&myMutexHandle);
}

//------------------------------------------------------------------------------
// Private virtual methods overridden for WaitCondition
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
WaitCondition::Error WaitConditionPosix::driverWait(const TimeMs waitTimeMs)
{
    myThreadHandle = pthread_self();

    pthread_mutex_lock(&myMutexHandle);
    pthread_cond_wait(&myConditionHandle, &myMutexHandle);
    pthread_mutex_unlock(&myMutexHandle);

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
WaitCondition::Error WaitConditionPosix::driverNotify()
{
    pthread_mutex_lock(&myMutexHandle);
    pthread_cond_broadcast(&myConditionHandle);
    pthread_mutex_unlock(&myMutexHandle);

    return Error(ERROR_CODE_NONE);
}
