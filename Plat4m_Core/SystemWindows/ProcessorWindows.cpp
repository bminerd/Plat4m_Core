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
/// @file ProcessorWindows.cpp
/// @author Ben Minerd
/// @date 2/23/2018
/// @brief ProcessorWindows class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/SystemWindows/ProcessorWindows.h>

using Plat4m::ProcessorWindows;
using Plat4m::Processor;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ProcessorWindows::ProcessorWindows() :
    Processor(ENDIAN_BIG, 0.0, 0)
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ProcessorWindows::~ProcessorWindows()
{
}

//------------------------------------------------------------------------------
// Private virtual methods overridden for Processor
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Processor::Error ProcessorWindows::driverReset()
{
    return Processor::Error(Processor::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
Processor::Error ProcessorWindows::driverConfigure(const Processor::Config& config)
{
    return Processor::Error(Processor::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
Processor::Error ProcessorWindows::driverSetPowerMode(
                                               const Processor::PowerMode powerMode)
{
    return Processor::Error(Processor::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
uint32_t ProcessorWindows::driverGetCoreClockFrequencyHz()
{
    return 0;
}
