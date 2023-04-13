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
/// @file SystemFreeRtosCortexM.cpp
/// @author Ben Minerd
/// @date 1/17/2018
/// @brief SystemFreeRtosCortexM class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <FreeRTOS-Kernel/include/FreeRTOS.h>
#include <FreeRTOSConfig.h>

#include <Plat4m_Core/SystemFreeRtos/SystemFreeRtosCortexM.h>

using namespace Plat4m;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
SystemFreeRtosCortexM::SystemFreeRtosCortexM() :
    SystemFreeRtos(),
    myLastTimeMs(0),
    myLastNsPortion(0)
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
SystemFreeRtosCortexM::~SystemFreeRtosCortexM()
{
}

//------------------------------------------------------------------------------
// Private virtual methods overridden for SystemFreeRtos
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
TimeUs SystemFreeRtosCortexM::driverGetTimeUs()
{
    volatile std::uint32_t sysTickLoad  =
                          *((std::uint32_t*) (0xE000E000UL + 0x0010UL + 0x4UL));
    volatile std::uint32_t sysTickValue =
                          *((std::uint32_t*) (0xE000E000UL + 0x0010UL + 0x8UL));

    Plat4m::TimeUs timeUs =
        ((getTimeMs() * 1000) +
                         (((sysTickLoad - sysTickValue) * 1000) / sysTickLoad));

    return timeUs;
}

//------------------------------------------------------------------------------
TimeStamp SystemFreeRtosCortexM::driverGetTimeStamp()
{
    volatile std::uint32_t sysTickLoad  =
                          *((std::uint32_t*) (0xE000E000UL + 0x0010UL + 0x4UL));
    volatile std::uint32_t sysTickValue =
                          *((std::uint32_t*) (0xE000E000UL + 0x0010UL + 0x8UL));

    // SysTick is configured to increment by 1 millisecond, so the fractional
    // part of the timer will always be <= 1ms which means we can safely add to
    // the nanosecond portion of the TimeStamp only
    std::uint64_t nsPortion =
        (((std::uint64_t) sysTickLoad - sysTickValue) * 1000000) / sysTickLoad;

    TimeMs timeMs = SystemFreeRtos::driverGetTimeMs();
    std::uint32_t timeMsRollOverCount = getTimeMsRollOverCounter();

    // If the SysTick timer has rolled over but timeMs has not been updated,
    // increment timeMs
    if ((timeMs == myLastTimeMs) && (nsPortion < myLastNsPortion))
    {
        timeMs++;
    }

    TimeStamp timeStamp;
    timeStamp.fromTimeMs(timeMs, timeMsRollOverCount);

    timeStamp.timeNs += (TimeNs) nsPortion;

    myLastTimeMs = timeMs;
    myLastNsPortion = nsPortion;

    return timeStamp;
}

//------------------------------------------------------------------------------
TimeStamp SystemFreeRtosCortexM::driverGetWallTimeStamp()
{
    return (driverGetTimeStamp());
}
