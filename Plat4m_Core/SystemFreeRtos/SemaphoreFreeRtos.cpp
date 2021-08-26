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
/// @file SemaphoreFreeRtos.cpp
/// @author Ben Minerd
/// @date 8/19/2021
/// @brief SemaphoreFreeRtos class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/SystemFreeRtos/SemaphoreFreeRtos.h>

using namespace std;
using namespace Plat4m;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
SemaphoreFreeRtos::SemaphoreFreeRtos(const uint32_t maxValue,
                                     const uint32_t initialValue) :
    Semaphore(maxValue, initialValue),
	mySemaphoreHandle()
{
    uint32_t newMaxValue = 0;

    if (maxValue == 0)
    {
        newMaxValue = 256;
    }
    else
    {
        newMaxValue = maxValue;
    }

    mySemaphoreHandle = xSemaphoreCreateCounting(newMaxValue, initialValue);
}

//------------------------------------------------------------------------------
// Public destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
SemaphoreFreeRtos::~SemaphoreFreeRtos()
{
}

//------------------------------------------------------------------------------
// Private methods implemented from Semaphore
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Semaphore::Error SemaphoreFreeRtos::driverWait()
{
    xSemaphoreTake(mySemaphoreHandle, portMAX_DELAY);

    return Semaphore::Error(Semaphore::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
Semaphore::Error SemaphoreFreeRtos::driverPost()
{
    xSemaphoreGive(mySemaphoreHandle);

    return Semaphore::Error(Semaphore::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
uint32_t SemaphoreFreeRtos::driverGetValue()
{
    return uxSemaphoreGetCount(mySemaphoreHandle);
}
