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
/// @file CurrentSensor.cpp
/// @author Ben Minerd
/// @date 1/12/2017
/// @brief CurrentSensor class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <CurrentSensor.h>

using Plat4m::CurrentSensor;
using Plat4m::Module;

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
CurrentSensor::Config CurrentSensor::getConfig() const
{
    return myConfig;
}

//------------------------------------------------------------------------------
CurrentSensor::Error CurrentSensor::setConfig(const Config& config)
{
    if (!isEnabled())
    {
        return Error(ERROR_CODE_NOT_ENABLED);
    }

    Error error = driverSetConfig(config);

    if (error.getCode() == ERROR_CODE_NONE)
    {
        myConfig = config;
    }

    return error;
}

//------------------------------------------------------------------------------
void CurrentSensor::setCurrentReadyCallback(
                                     CurrentReadyCallback& currentReadyCallback)
{
    myCurrentReadyCallback = &currentReadyCallback;
}

//------------------------------------------------------------------------------
Plat4m::CurrentA CurrentSensor::getOffsetCurrentA()
{
    return myOffsetCurrentA;
}

//------------------------------------------------------------------------------
CurrentSensor::Error CurrentSensor::setOffsetCurrentA(
                                                  const CurrentA offsetCurrentA)
{
    if (!isEnabled())
    {
        return Error(ERROR_CODE_NOT_ENABLED);
    }

    myOffsetCurrentA = offsetCurrentA;

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
CurrentSensor::Error CurrentSensor::getCurrentA(CurrentA& currentA)
{
    if (!isEnabled())
    {
        return Error(ERROR_CODE_NOT_ENABLED);
    }

    CurrentA tempCurrentA;

    Error error = driverGetCurrentA(tempCurrentA);

    if (error.getCode() == ERROR_CODE_NONE)
    {
        currentA = tempCurrentA + myOffsetCurrentA;
    }

    return error;
}

//------------------------------------------------------------------------------
CurrentSensor::Error CurrentSensor::startOffsetCalibration(
                                                        const uint32_t nSamples)
{
    if (!isEnabled())
    {
        return Error(ERROR_CODE_NOT_ENABLED);
    }

    Error error = driverStartOffsetCalibration(nSamples);

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
bool CurrentSensor::isOffsetCalibrationComplete()
{
    return (driverIsOffsetCalibrationComplete());
}

//------------------------------------------------------------------------------
// Protected constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
CurrentSensor::CurrentSensor() :
	Module(),
	myCurrentReadyCallback(0),
	myOffsetCurrentA(0.0)
{
}

//------------------------------------------------------------------------------
// Protected virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
CurrentSensor::~CurrentSensor()
{
}

//------------------------------------------------------------------------------
// Private virtual methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
CurrentSensor::Error CurrentSensor::driverStartOffsetCalibration(
                                                        const uint32_t nSamples)
{
    // Not implemented by subclass, generic implementation

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
bool CurrentSensor::driverIsOffsetCalibrationComplete()
{
    // Not implemented by subclass, generic implementation

    return true;
}
