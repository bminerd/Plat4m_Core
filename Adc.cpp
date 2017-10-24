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
/// @file Adc.cpp
/// @author Ben Minerd
/// @date 4/10/13
/// @brief Adc class.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Adc.h>

using Plat4m::Adc;

//------------------------------------------------------------------------------
// Public virtual methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Adc::Error Adc::configure(const Config& config)
{
    if (!isEnabled())
    {
        return Error(ERROR_CODE_NOT_ENABLED);
    }
    
    Error error = driverConfigure(config);
    
    if (error.getCode() == ERROR_CODE_NONE)
    {
        myConfig = config;
    }

    return error;
}

//------------------------------------------------------------------------------
Adc::Error Adc::readVoltage(float& voltage, const unsigned int nReadings)
{
    if (!isEnabled())
    {
        return Error(ERROR_CODE_NOT_ENABLED);
    }

    float tempVoltage;

    Error error = driverReadVoltage(tempVoltage);

    if (error.getCode() == ERROR_CODE_NONE)
    {
        voltage = tempVoltage;
    }

    return error;
}

//------------------------------------------------------------------------------
// Protected constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Adc::Adc() :
    Module(),
    myConfig()
{
}

//------------------------------------------------------------------------------
// Protected virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Adc::~Adc()
{
}
