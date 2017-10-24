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
// Copyright (c) 2017 Benjamin Minerd
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
/// @file PwmOutput.cpp
/// @author Ben Minerd
/// @date 7/12/2016
/// @brief PwmOutput class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <PwmOutput.h>

using Plat4m::PwmOutput;

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
PwmOutput::Error PwmOutput::setConfig(const Config& config)
{
    if (!isEnabled())
    {
        return Error(ERROR_CODE_NOT_ENABLED);
    }
    
    Error error = driverSetConfig(config);
    
    if (error.getCode() == ERROR_CODE_NONE)
    {
        myConfig = config;

        myDutyCycleRangePercent =
                    myConfig.maxDutyCyclePercent - myConfig.minDutyCyclePercent;
    }
    
    return error;
}

//------------------------------------------------------------------------------
PwmOutput::Config PwmOutput::getConfig() const
{
    return myConfig;
}

//------------------------------------------------------------------------------
PwmOutput::Error PwmOutput::setDutyCyclePercent(const float dutyCyclePercent)
{
    if (!isEnabled())
    {
        return Error(ERROR_CODE_NOT_ENABLED);
    }

    float scaledDutyCyclePercent =
                        (dutyCyclePercent * (myDutyCycleRangePercent / 100.0)) +
                        myConfig.minDutyCyclePercent;

    Error error = driverSetDutyCyclePercent(scaledDutyCyclePercent);

    if (error.getCode() == ERROR_CODE_NONE)
    {
        myConfig.dutyCyclePercent = dutyCyclePercent;
    }

    return error;
}

//------------------------------------------------------------------------------
PwmOutput::Error PwmOutput::setActive(const bool active)
{
    if (!isEnabled())
    {
        return Error(ERROR_CODE_NOT_ENABLED);
    }

    Error error = driverSetActive(active);

    if (error.getCode() == ERROR_CODE_NONE)
    {
        myIsActive = active;
    }

    return error;
}

//------------------------------------------------------------------------------
PwmOutput::Error PwmOutput::setComplementaryActive(const bool active)
{
    if (!isEnabled())
    {
        return Error(ERROR_CODE_NOT_ENABLED);
    }

    Error error = driverSetComplementaryActive(active);

    if (error.getCode() == ERROR_CODE_NONE)
    {
        myIsActive = active;
    }

    return error;
}

//------------------------------------------------------------------------------
// Protected constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
PwmOutput::PwmOutput() :
    Module(),
    myConfig(),
    myDutyCycleRangePercent(0.0),
    myIsActive(false)
{
}

//------------------------------------------------------------------------------
// Protected virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
PwmOutput::~PwmOutput()
{
}
