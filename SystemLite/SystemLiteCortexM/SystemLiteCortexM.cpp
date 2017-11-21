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
// Copyright (c) 2016 Benjamin Minerd
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
/// @file SystemLiteCortexM.cpp
/// @author Ben Minerd
/// @date 11/28/2016
/// @brief SystemLiteCortexM class.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <SystemLiteCortexM.h>

// TODO: For now
#include <stm32f30x.h>

using Plat4m::SystemLiteCortexM;
using Plat4m::SystemLite;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
SystemLiteCortexM::SystemLiteCortexM() :
    SystemLite()
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
SystemLiteCortexM::~SystemLiteCortexM()
{
}

//------------------------------------------------------------------------------
// Private methods implemented from SystemLite
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void SystemLiteCortexM::driverEnableSystemClock(
                                            const uint32_t coreClockFrequencyHz)
{
    uint32_t value = SysTick_Config(coreClockFrequencyHz / 1000);
}

//------------------------------------------------------------------------------
// Interrupt service routines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
extern "C" void SysTick_Handler(void)
{
    SystemLite::timeMsHandler();
}
