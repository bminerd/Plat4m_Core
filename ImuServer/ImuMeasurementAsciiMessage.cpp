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
/// @file ImuMeasurementAsciiMessage.cpp
/// @author Ben Minerd
/// @date 5/20/16
/// @brief ImuMeasurementAsciiMessage class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <ImuMeasurementAsciiMessage.h>

#include <stdlib.h>
#include <stdio.h>

using Plat4m::ImuMeasurementAsciiMessage;
using Plat4m::ImuMeasurementMessage;
using Plat4m::ByteArray;
using Plat4m::Array;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const ByteArray ImuMeasurementAsciiMessage::myName("IMU_MEASUREMENT");

ByteArray ImuMeasurementAsciiMessage::myParameterNameStrings[] =
{
    ByteArray("TEMP"),
    ByteArray("ACCEL_X"),
    ByteArray("ACCEL_Y"),
    ByteArray("ACCEL_Z"),
    ByteArray("GYRO_X"),
    ByteArray("GYRO_Y"),
    ByteArray("GYRO_Z"),
//    ByteArray("MAG_X"),
//    ByteArray("MAG_Y"),
//    ByteArray("MAG_Z")
};

const Array<ByteArray> ImuMeasurementAsciiMessage::myParameterNames(
                                            myParameterNameStrings,
                                            ARRAY_SIZE(myParameterNameStrings));

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ImuMeasurementAsciiMessage::ImuMeasurementAsciiMessage(
                                               ImuMeasurementMessage& message) :
    AsciiMessage(myName, &myParameterNames),
    myMessage(message)
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ImuMeasurementAsciiMessage::~ImuMeasurementAsciiMessage()
{
}

//------------------------------------------------------------------------------
// Public virtual methods implemented from AsciiMessage
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ImuMeasurementAsciiMessage::stringParametersUpdated()
{
    myMessage.tempC =
                     getFloatParameter(ImuMeasurementMessage::PARAMETER_TEMP_C);
    myMessage.accelX =
                    getFloatParameter(ImuMeasurementMessage::PARAMETER_ACCEL_X);
    myMessage.accelY =
                    getFloatParameter(ImuMeasurementMessage::PARAMETER_ACCEL_Y);
    myMessage.accelX =
                    getFloatParameter(ImuMeasurementMessage::PARAMETER_ACCEL_Z);
    myMessage.gyroX =
                     getFloatParameter(ImuMeasurementMessage::PARAMETER_GYRO_X);
    myMessage.gyroX =
                     getFloatParameter(ImuMeasurementMessage::PARAMETER_GYRO_Y);
    myMessage.gyroZ =
                     getFloatParameter(ImuMeasurementMessage::PARAMETER_GYRO_Z);
//    myMessage.magX = getFloatParameter(
//                                    ImuMeasurementMessage::PARAMETER_MAG_X);
//    myMessage.magY = getFloatParameter(
//                                    ImuMeasurementMessage::PARAMETER_MAG_Y);
//    myMessage.magZ = getFloatParameter(
//                                    ImuMeasurementMessage::PARAMETER_MAG_Z);
}

//------------------------------------------------------------------------------
void ImuMeasurementAsciiMessage::messageParametersUpdated()
{
    setFloatParameter(ImuMeasurementMessage::PARAMETER_TEMP_C,
                      myMessage.tempC,
                      1);
    setFloatParameter(ImuMeasurementMessage::PARAMETER_ACCEL_X,
                      myMessage.accelX);
    setFloatParameter(ImuMeasurementMessage::PARAMETER_ACCEL_Y,
                      myMessage.accelY);
    setFloatParameter(ImuMeasurementMessage::PARAMETER_ACCEL_Z,
                      myMessage.accelZ);
    setFloatParameter(ImuMeasurementMessage::PARAMETER_GYRO_X,
                      myMessage.gyroX);
    setFloatParameter(ImuMeasurementMessage::PARAMETER_GYRO_Y,
                      myMessage.gyroY);
    setFloatParameter(ImuMeasurementMessage::PARAMETER_GYRO_Z,
                      myMessage.gyroZ);
//    setFloatParameter(ImuMeasurementMessage::PARAMETER_MAG_X,
//                      myMessage.magX);
//    setFloatParameter(ImuMeasurementMessage::PARAMETER_MAG_Y,
//                      myMessage.magY);
//    setFloatParameter(ImuMeasurementMessage::PARAMETER_MAG_Z,
//                      myMessage.magZ);
}
