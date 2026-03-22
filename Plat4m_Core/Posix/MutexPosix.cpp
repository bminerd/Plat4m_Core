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
/// @file MutexPosix.cpp
/// @author Ben Minerd
/// @date 5/28/2019
/// @brief MutexPosix class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Posix/MutexPosix.h>
#include <Plat4m_Core/Plat4m.h>

using namespace Plat4m;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
MutexPosix::MutexPosix() :
    Mutex(),
    myMutexHandle()
{
    int returnValue = pthread_mutex_init(&myMutexHandle, NULL);

    if (returnValue != 0)
    {
        PLAT4M_REPORT_ERROR(Mutex::Error,
                            Mutex::ERROR_CODE_INSTANTIATION_FAILED,
                            ErrorBase::SEVERITY_CRITICAL,
                            this);
    }
}

//------------------------------------------------------------------------------
// Public destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
MutexPosix::~MutexPosix()
{
    pthread_mutex_destroy(&myMutexHandle);
}

//------------------------------------------------------------------------------
// Private virtual methods overridden for Mutex
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Mutex::Error MutexPosix::driverSetLocked(const bool locked)
{
    if (locked)
    {
        pthread_mutex_lock(&myMutexHandle);
    }
    else
    {
        pthread_mutex_unlock(&myMutexHandle);
    }

    return Error(ERROR_CODE_NONE);
}
