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
/// @file ImuSetConfigAsciiMessage.cpp
/// @author Ben Minerd
/// @date 6/7/16
/// @brief ImuSetConfigAsciiMessage class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/ImuServer/ImuSetConfigAsciiMessage.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using Plat4m::ImuSetConfigAsciiMessage;
using Plat4m::ByteArray;
using Plat4m::Array;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const ByteArray ImuSetConfigAsciiMessage::myName("IMU_SET_CONFIG");

ByteArray ImuSetConfigAsciiMessage::myParameterNameStrings[] =
{
    ByteArray("MEASUREMENT_MODE"),
    ByteArray("ACCEL_MEASUREMENT_RANGE"),
    ByteArray("ACCEL_MEASUREMENT_RATE_HZ"),
    ByteArray("GYRO_MEASUREMENT_RANGE"),
    ByteArray("GYRO_MEASUREMENT_RATE_HZ"),
//    ByteArray("MAG_MEASUREMENT_RANGE"),
//    ByteArray("MAG_MEASUREMENT_RATE_HZ")
};

const Array<ByteArray> ImuSetConfigAsciiMessage::myParameterNames(
                                            myParameterNameStrings,
                                            ARRAY_SIZE(myParameterNameStrings));

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ImuSetConfigAsciiMessage::ImuSetConfigAsciiMessage(
                                               ImuSetConfigMessage& message) :
    AsciiMessage(myName, &myParameterNames),
    myMessage(message)
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ImuSetConfigAsciiMessage::~ImuSetConfigAsciiMessage()
{
}

//------------------------------------------------------------------------------
// Public virtual methods implemented from AsciiMessage
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ImuSetConfigAsciiMessage::stringParametersUpdated()
{
    const Array<ByteArray>* parameterValues = getParameterValues();
    const char* string = 0;

    // Measurement mode

    string = (const char*) (parameterValues->getItem(0).getItems());

    if (strcmp(string, "SINGLE") == 0)
    {
        myMessage.measurementMode = 0;
    }
    else if (strcmp(string, "CONTINUOUS") == 0)
    {
        myMessage.measurementMode = 1;
    }

    myMessage.accelMeasurementRange = getFloatParameter(
                        ImuSetConfigMessage::PARAMETER_ACCEL_MEASUREMENT_RANGE);
    myMessage.accelMeasurementRateHz = getUint32Parameter(
                      ImuSetConfigMessage::PARAMETER_ACCEL_MEASUREMENT_RATE_HZ);
    myMessage.gyroMeasurementRange = getFloatParameter(
                         ImuSetConfigMessage::PARAMETER_GYRO_MEASUREMENT_RANGE);
    myMessage.gyroMeasurementRateHz = getUint32Parameter(
                       ImuSetConfigMessage::PARAMETER_GYRO_MEASUREMENT_RATE_HZ);
//    myMessage.magMeasurementRange = getFloatParameter(
//            ImuSetConfigMessage::PARAMETER_MAG_MEASUREMENT_RANGE));
//    myMessage.magMeasurementRateHz = getFloatParameter(
//                         ImuSetConfigMessage::PARAMETER_MAG_MEASUREMENT_RANGE));
}

//------------------------------------------------------------------------------
void ImuSetConfigAsciiMessage::messageParametersUpdated()
{
    if (myMessage.measurementMode == 0)
    {
        setStringParameter(ImuSetConfigMessage::PARAMETER_MEASUREMENT_MODE,
                           "SINGLE");
    }
    else if (myMessage.measurementMode == 1)
    {
        setStringParameter(ImuSetConfigMessage::PARAMETER_MEASUREMENT_MODE,
                           "CONTINUOUS");
    }

    setFloatParameter(ImuSetConfigMessage::PARAMETER_ACCEL_MEASUREMENT_RANGE,
                      myMessage.accelMeasurementRange);
    setFloatParameter(ImuSetConfigMessage::PARAMETER_ACCEL_MEASUREMENT_RATE_HZ,
                      myMessage.accelMeasurementRateHz);
    setFloatParameter(ImuSetConfigMessage::PARAMETER_GYRO_MEASUREMENT_RANGE,
                      myMessage.gyroMeasurementRange);
    setFloatParameter(ImuSetConfigMessage::PARAMETER_GYRO_MEASUREMENT_RATE_HZ,
                      myMessage.gyroMeasurementRateHz);
//    setFloatParameter(ImuSetConfigMessage::PARAMETER_MAG_MEASUREMENT_RANGE,
//                      myMessage.getMagMeasurementRange());
//    setFloatParameter(ImuSetConfigMessage::PARAMETER_MAG_MEASUREMENT_RATE_HZ,
//                      myMessage.getMagMeasurementRateHz());
}
