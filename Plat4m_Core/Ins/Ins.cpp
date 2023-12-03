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
/// @file Ins.cpp
/// @author Ben Minerd
/// @date 4/20/2017
/// @brief Ins class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Ins.h>

using Plat4m::Ins;

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void Ins::setMeasurementReadyCallback(MeasurementCallback& callback)
{
    myMeasurementCallback = &callback;
}

//------------------------------------------------------------------------------
Ins::Error Ins::setConfig(const Config& config)
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
Ins::Config Ins::getConfig() const
{
    return myConfig;
}

//------------------------------------------------------------------------------
Ins::Error Ins::getMeasurement(Measurement& measurement)
{
    if (!isEnabled())
    {
        return Error(ERROR_CODE_NOT_ENABLED);
    }

    Error error = driverGetMeasurement(measurement);

    if (error.getCode() == ERROR_CODE_NONE)
    {
        myLastMeasurement = measurement;
    }

    return error;
}

//------------------------------------------------------------------------------
Ins::Error Ins::getEulerAnglesDegrees(AngleDegrees& yawAngleDegrees,
                                      AngleDegrees& pitchAngleDegrees,
                                      AngleDegrees& rollAngleDegrees)
{
    if (!isEnabled())
    {
        return Error(ERROR_CODE_NOT_ENABLED);
    }

    AngleDegrees tempYawAngleDegrees;
    AngleDegrees tempPitchAngleDegrees;
    AngleDegrees tempRollAngleDegrees;

    Error error = driverGetEulerAnglesDegrees(tempYawAngleDegrees,
                                              tempPitchAngleDegrees,
                                              tempRollAngleDegrees);

    if (error.getCode() == ERROR_CODE_NONE)
    {
        yawAngleDegrees   = tempYawAngleDegrees;
        pitchAngleDegrees = tempPitchAngleDegrees;
        rollAngleDegrees  = tempRollAngleDegrees;
    }

    return error;
}

//------------------------------------------------------------------------------
Ins::Error Ins::getLastMeasurement(Measurement& measurement)
{
    measurement = myLastMeasurement;

    return Error(ERROR_CODE_NONE);
}
//------------------------------------------------------------------------------
// Protected constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Ins::Ins() :
	Module(),
	myConfig(),
    myMeasurementCallback(0),
    myLastMeasurement()
{
}

//------------------------------------------------------------------------------
// Protected virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Ins::~Ins()
{
}

//------------------------------------------------------------------------------
// Protected methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void Ins::measurementReady()
{
    if (isValidPointer(myMeasurementCallback))
    {
        myMeasurementCallback->call();
    }
}
