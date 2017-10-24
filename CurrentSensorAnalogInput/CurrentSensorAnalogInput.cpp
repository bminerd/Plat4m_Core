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
/// @file CurrentSensorAnalogInput.cpp
/// @author Ben Minerd
/// @date 1/12/2017
/// @brief CurrentSensorAnalogInput class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <CurrentSensorAnalogInput.h>

using Plat4m::CurrentSensorAnalogInput;
using Plat4m::CurrentSensor;
using Plat4m::Module;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
CurrentSensorAnalogInput::CurrentSensorAnalogInput(const RealNumber scale,
												   const RealNumber offset,
												   AnalogInput& analogInput) :
	CurrentSensor(),
	myScale(scale),
	myOffset(offset),
	myAnalogInput(analogInput)
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
CurrentSensorAnalogInput::~CurrentSensorAnalogInput()
{
}

//------------------------------------------------------------------------------
// Public methods implemented from CurrentSensor
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Plat4m::CurrentA CurrentSensorAnalogInput::getCurrentAFast()
{
    VoltageV voltageV = myAnalogInput.readVoltageVFast();

    return (voltageV * myScale + myOffset);
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error CurrentSensorAnalogInput::driverSetEnabled(const bool enabled)
{
    myAnalogInput.setEnabled(enabled);

    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods implemented from CurrentSensor
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
CurrentSensor::Error CurrentSensorAnalogInput::driverSetConfig(
                                                           const Config& config)
{


    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
CurrentSensor::Error CurrentSensorAnalogInput::driverGetCurrentA(
                                                             CurrentA& currentA)
{
    VoltageV voltageV;

    AnalogInput::Error analogInputError = myAnalogInput.readVoltageV(voltageV);

    currentA = voltageV * myScale + myOffset;

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
CurrentSensor::Error CurrentSensorAnalogInput::driverStartOffsetCalibration(
                                                        const uint32_t nSamples)
{
    myAnalogInput.startOffsetCalibration(nSamples);

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
bool CurrentSensorAnalogInput::driverIsOffsetCalibrationComplete()
{
    return (myAnalogInput.isOffsetCalibrationComplete());
}
