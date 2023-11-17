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
/// @file ProcessorLinux.cpp
/// @author Ben Minerd
/// @date 5/3/2019
/// @brief ProcessorLinux class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Linux/ProcessorLinux.h>

using Plat4m::ProcessorLinux;
using Plat4m::Processor;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ProcessorLinux::ProcessorLinux() :
    Processor(ENDIAN_BIG, 0.0, 0)
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ProcessorLinux::~ProcessorLinux()
{
}

//------------------------------------------------------------------------------
// Private virtual methods overridden for Processor
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Processor::Error ProcessorLinux::driverReset()
{
    return Processor::Error(Processor::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
Processor::Error ProcessorLinux::driverConfigure(
                                                const Processor::Config& config)
{
    return Processor::Error(Processor::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
Processor::Error ProcessorLinux::driverSetPowerMode(
                                           const Processor::PowerMode powerMode)
{
    return Processor::Error(Processor::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
uint32_t ProcessorLinux::driverGetCoreClockFrequencyHz()
{
    return 0;
}
