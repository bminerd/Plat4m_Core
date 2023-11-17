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
// Copyright (c) 2018-2023 Benjamin Minerd
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
/// @file WaitConditionWindows.cpp
/// @author Ben Minerd
/// @date 2/19/2018
/// @brief WaitConditionWindows class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/SystemWindows/WaitConditionWindows.h>

using Plat4m::WaitConditionWindows;
using Plat4m::WaitCondition;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
WaitConditionWindows::WaitConditionWindows() :
    WaitCondition(),
    myThreadHandle(0)
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
WaitConditionWindows::~WaitConditionWindows()
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void WaitConditionWindows::notifyFast()
{
}

//------------------------------------------------------------------------------
// Private virtual methods overridden for WaitCondition
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
WaitCondition::Error WaitConditionWindows::driverWait(const TimeMs waitTimeMs)
{
//	myTaskHandle = xTaskGetCurrentTaskHandle();
//
//	ulTaskNotifyTake(pdFALSE, (TickType_t) waitTimeMs);

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
WaitCondition::Error WaitConditionWindows::driverNotify()
{
//	xTaskNotifyGive(myTaskHandle);
//
//	myTaskHandle = 0;

    return Error(ERROR_CODE_NONE);
}
