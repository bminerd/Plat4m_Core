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
// Copyright (c) 2018 Benjamin Minerd
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

#include <Plat4m_Core/SystemFreeRtos/SystemFreeRtosCortexM.h>

using Plat4m::SystemFreeRtosCortexM;
using Plat4m::SystemFreeRtos;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
SystemFreeRtosCortexM::SystemFreeRtosCortexM() :
    SystemFreeRtos()
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
// Private methods implemented from SystemFreeRtos
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Plat4m::TimeUs SystemFreeRtosCortexM::driverGetTimeUs()
{
    volatile uint32_t sysTickLoad  =
    						   *((uint32_t*) (0xE000E000UL + 0x0010UL + 0x4UL));
    volatile uint32_t sysTickValue =
    						   *((uint32_t*) (0xE000E000UL + 0x0010UL + 0x8UL));

    return ((getTimeMs() * 1000) + ((sysTickValue * 1000) / sysTickLoad));
}
