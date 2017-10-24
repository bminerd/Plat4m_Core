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
/// @file AnalogInput.cpp
/// @author Ben Minerd
/// @date 8/9/16
/// @brief AnalogInput class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <AnalogInput.h>

using Plat4m::AnalogInput;

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void AnalogInput::setOffsetVoltageV(const VoltageV offsetVoltageV)
{
    driverSetOffsetVoltageV(offsetVoltageV);
}

//------------------------------------------------------------------------------
AnalogInput::Error AnalogInput::readVoltageV(VoltageV& voltageV)
{
    if (!isEnabled())
    {
        return Error(ERROR_CODE_NOT_ENABLED);
    }

    VoltageV tempVoltageV;

    Error error = driverReadVoltageV(tempVoltageV);

    if (error.getCode() == ERROR_CODE_NONE)
    {
        voltageV = tempVoltageV + myOffsetVoltageV;
    }

    return error;
}

//------------------------------------------------------------------------------
AnalogInput::Error AnalogInput::startOffsetCalibration(const uint32_t nSamples)
{
    if (!isEnabled())
    {
        return Error(ERROR_CODE_NOT_ENABLED);
    }

    Error error = driverStartOffsetCalibration(nSamples);

    return error;
}

//------------------------------------------------------------------------------
bool AnalogInput::isOffsetCalibrationComplete()
{
    return (driverIsOffsetCalibrationComplete());
}

//------------------------------------------------------------------------------
// Protected constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
AnalogInput::AnalogInput() :
    Module(),
    myOffsetVoltageV(0.0),
    myIsOffsetCalibrationRunning(false)
{
}

//------------------------------------------------------------------------------
// Protected virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
AnalogInput::~AnalogInput()
{
}

//------------------------------------------------------------------------------
// Private virtual methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
AnalogInput::Error AnalogInput::driverSetOffsetVoltageV(
                                                  const VoltageV offsetVoltageV)
{
    // Not implemented by subclass, generic implementation

    myOffsetVoltageV = offsetVoltageV;

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
AnalogInput::Error AnalogInput::driverStartOffsetCalibration(
                                                        const uint32_t nSamples)
{
    // Not implemented by subclass, generic implementation

    myIsOffsetCalibrationRunning = true;

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
bool AnalogInput::driverIsOffsetCalibrationComplete()
{
    // Not implemented by subclass, generic implementation

    return (!myIsOffsetCalibrationRunning);
}
