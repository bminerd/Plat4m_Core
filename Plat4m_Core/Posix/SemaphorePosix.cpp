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
// Copyright (c) 2021-2024 Benjamin Minerd
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
/// @file SemaphorePosix.cpp
/// @author Ben Minerd
/// @date 8/19/2021
/// @brief SemaphorePosix class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Posix/SemaphorePosix.h>
#include <Plat4m_Core/Plat4m.h>

using namespace Plat4m;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
SemaphorePosix::SemaphorePosix(const std::uint32_t maxValue,
                               const std::uint32_t initialValue) :
    Semaphore(maxValue, initialValue),
    mySemaphoreHandle()
{
    int returnValue = sem_init(&mySemaphoreHandle, 0, initialValue);

    if (returnValue != 0)
    {
        PLAT4M_REPORT_ERROR(Semaphore::Error,
                            Semaphore::ERROR_CODE_INSTANTIATION_FAILED,
                            ErrorBase::SEVERITY_CRITICAL,
                            this);
    }
}

//------------------------------------------------------------------------------
// Public destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
SemaphorePosix::~SemaphorePosix()
{
    sem_destroy(&mySemaphoreHandle);
}

//------------------------------------------------------------------------------
// Private virtual methods implemented overridden for Semaphore
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Semaphore::Error SemaphorePosix::driverWait()
{
    sem_wait(&mySemaphoreHandle);

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
Semaphore::Error SemaphorePosix::driverPost()
{
    sem_post(&mySemaphoreHandle);

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
std::uint32_t SemaphorePosix::driverGetValue()
{
    std::uint32_t value;

    sem_getvalue(&mySemaphoreHandle, (int*) &value);

    return value;
}
