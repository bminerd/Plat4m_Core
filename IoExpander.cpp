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
/// @file IoExpander.cpp
/// @author Ben Minerd
/// @date 3/25/2013
/// @brief IoExpander class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <IoExpander.h>

using Plat4m::IoExpander;

//------------------------------------------------------------------------------
// Public virtual methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
IoExpander::Error IoExpander::registerRead(const Id registerId,
                                           uint8_t& value)
{
    return driverRegisterRead(registerId, value);
}
    
//------------------------------------------------------------------------------
IoExpander::Error IoExpander::registerWrite(const Id registerId,
                                            const uint8_t value)
{
    return driverRegisterWrite(registerId, value);
}

//------------------------------------------------------------------------------
IoExpander::Error IoExpander::registerReadBit(const Id registerId,
                                              const unsigned int bit,
                                              Plat4m::BitValue& bitValue)
{
    return driverRegisterReadBit(registerId, bit, bitValue);
}

//------------------------------------------------------------------------------
IoExpander::Error IoExpander::registerWriteBit(const Id registerId,
                                               const unsigned int bit,
                                               const Plat4m::BitValue bitValue)
{
    return driverRegisterWriteBit(registerId, bit, bitValue);
}

//------------------------------------------------------------------------------
IoExpander::Error IoExpander::pinConfigure(const Id pinId,
                                           const GpioPin::Config& config)
{
    return driverPinConfigure(pinId, config);
}

//------------------------------------------------------------------------------
IoExpander::Error IoExpander::pinSetLevel(const Id pinId,
                                          const GpioPin::Level level)
{
    return driverPinSetLevel(pinId, level);
}

//------------------------------------------------------------------------------
IoExpander::Error IoExpander::pinGetLevel(const Id pinId,
                                          GpioPin::Level& level)
{
    return driverPinGetLevel(pinId, level);
}

//------------------------------------------------------------------------------
// Protected constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
IoExpander::IoExpander()
{
}

//------------------------------------------------------------------------------
// Protected virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
IoExpander::~IoExpander()
{
}
