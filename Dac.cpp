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
/// @file Dac.cpp
/// @author Ben Minerd
/// @date 4/3/2013
/// @brief Dac class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Dac.h>

#include <math.h>

using Plat4m::Dac;

//------------------------------------------------------------------------------
// Public virtual methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Dac::Error Dac::configure(const Config& config)
{
    if (!isEnabled())
    {
        return ERROR_NOT_ENABLED;
    }

    Error error = driverConfigure(config);
    
    if (error == ERROR_NONE)
    {
        myConfig = config;
    }

    return error;
}

//------------------------------------------------------------------------------
Dac::Error Dac::setVoltage(const float voltage)
{
    if (!isEnabled())
    {
        return ERROR_NOT_ENABLED;
    }
    
    if (voltage > myMaxVoltage)
    {
        // Error
    }
    
    Error error = driverSetVoltage(voltage);

    return error;
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
float Dac::getMaxVoltage() const
{
    return myMaxVoltage;
}

//------------------------------------------------------------------------------
float Dac::getMaxValue() const
{
    return myMaxValue;
}

//------------------------------------------------------------------------------
float Dac::getLevelsPerVolt() const
{
    return myLevelsPerVolt;
}

//------------------------------------------------------------------------------
float Dac::getVoltsPerLevel() const
{
    return (1.0f / myLevelsPerVolt);
}

//------------------------------------------------------------------------------
// Protected constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Dac::Dac(const float maxVoltage, const uint32_t maxValue) :
    Module(),
    myMaxVoltage(maxVoltage),
    myMaxValue(maxValue),
    myLevelsPerVolt(myMaxValue / myMaxVoltage)
{
}

//------------------------------------------------------------------------------
// Protected virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Dac::~Dac()
{
}

//------------------------------------------------------------------------------
// Protected methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
uint32_t Dac::computeDacValue(const float voltage)
{
    return (uint32_t) round(voltage * myLevelsPerVolt);
}
