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
// Copyright (c) 2021 Benjamin Minerd
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
/// @file SemaphoreWindows.cpp
/// @author Ben Minerd
/// @date 8/27/2021
/// @brief SemaphoreWindows class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/SystemWindows/SemaphoreWindows.h>
#include <Plat4m_Core/Plat4m.h>

using namespace std;
using namespace Plat4m;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
SemaphoreWindows::SemaphoreWindows(const uint32_t maxValue,
                                   const uint32_t initialValue) :
    Semaphore(),
    mySemaphoreHandle(CreateSemaphore(NULL, initialValue, maxValue, NULL))
{
    if (isNullPointer(mySemaphoreHandle))
    {
        while (true)
        {
            // Lock up, unable to create semaphore
        }
    }
}

//------------------------------------------------------------------------------
// Public destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
SemaphoreWindows::~SemaphoreWindows()
{
}

//------------------------------------------------------------------------------
// Private virtual methods implemented overridden for Semaphore
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Semaphore::Error SemaphoreWindows::driverWait()
{
	DWORD dwWaitResult;

    dwWaitResult = WaitForSingleObject(mySemaphoreHandle, INFINITE);

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
Semaphore::Error SemaphoreWindows::driverPost()
{
	ReleaseSemaphore(mySemaphoreHandle, 1, NULL);

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
uint32_t SemaphoreWindows::driverGetValue()
{
	uint32_t value;

    DWORD dwWaitResult;

    dwWaitResult = WaitForSingleObject(mySemaphoreHandle, 0);

    ReleaseSemaphore(mySemaphoreHandle, 1, &value);

    return value;
}
