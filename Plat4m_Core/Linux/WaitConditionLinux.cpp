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
// Copyright (c) 2019-2023 Benjamin Minerd
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
/// @file WaitConditionLinux.cpp
/// @author Ben Minerd
/// @date 5/26/2019
/// @brief WaitConditionLinux class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Linux/WaitConditionLinux.h>

using Plat4m::WaitConditionLinux;
using Plat4m::WaitCondition;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
WaitConditionLinux::WaitConditionLinux() :
    WaitCondition(),
    myConditionHandle(PTHREAD_COND_INITIALIZER),
    myMutexHandle(PTHREAD_MUTEX_INITIALIZER),
    myThreadHandle(0)
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
WaitConditionLinux::~WaitConditionLinux()
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void WaitConditionLinux::notifyFast()
{
    pthread_mutex_lock(&myMutexHandle);
    pthread_cond_broadcast(&myConditionHandle);
    pthread_mutex_unlock(&myMutexHandle);
}

//------------------------------------------------------------------------------
// Private virtual methods overridden for WaitCondition
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
WaitCondition::Error WaitConditionLinux::driverWait(const TimeMs waitTimeMs)
{
    myThreadHandle = pthread_self();

    pthread_mutex_lock(&myMutexHandle);
    pthread_cond_wait(&myConditionHandle, &myMutexHandle);
    pthread_mutex_unlock(&myMutexHandle);

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
WaitCondition::Error WaitConditionLinux::driverNotify()
{
    pthread_mutex_lock(&myMutexHandle);
    pthread_cond_broadcast(&myConditionHandle);
    pthread_mutex_unlock(&myMutexHandle);

    return Error(ERROR_CODE_NONE);
}
