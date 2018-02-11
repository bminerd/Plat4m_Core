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
// Copyright (c) 2013 Benjamin Minerd
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
/// @file DacIc.cpp
/// @author Ben Minerd
/// @date 4/3/2013
/// @brief DacIc class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/DacIc.h>

using Plat4m::DacIc;

//------------------------------------------------------------------------------
// Public virtual methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
DacIc::Error DacIc::registerRead(const Id registerId, uint16_t& value)
{
    Error error = driverRegisterRead(registerId, value);
    
    return error;
}
    
//------------------------------------------------------------------------------
DacIc::Error DacIc::registerWrite(const Id registerId, const uint16_t value)
{
    Error error = driverRegisterWrite(registerId, value);
    
    return error;
}

//------------------------------------------------------------------------------
DacIc::Error DacIc::registerReadBit(const Id registerId,
                                    const unsigned int bit,
                                    Plat4m::BitValue& bitValue)
{
    Error error = driverRegisterReadBit(registerId, bit, bitValue);
    
    return error;
}

//------------------------------------------------------------------------------
DacIc::Error DacIc::registerWriteBit(const Id registerId,
                                     const unsigned int bit,
                                     const Plat4m::BitValue bitValue)
{
    Error error = driverRegisterWriteBit(registerId, bit, bitValue);
    
    return error;
}

//------------------------------------------------------------------------------
// Protected constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
DacIc::DacIc() :
    Module()
{
}

//------------------------------------------------------------------------------
// Protected virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
DacIc::~DacIc()
{
}
