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
/// @file ImuServerMeasurementAsciiMessage.cpp
/// @author Ben Minerd
/// @date 6/30/16
/// @brief ImuServerMeasurementAsciiMessage class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>

#include <Plat4m_Core/ImuServer/ImuServerMeasurementAsciiMessage.h>

using Plat4m::ImuServerMeasurementAsciiMessage;
using Plat4m::ImuServerMeasurementMessage;
using Plat4m::ByteArray;
using Plat4m::Array;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const ByteArray ImuServerMeasurementAsciiMessage::myName(
                                                      "IMU_SERVER_MEASUREMENT");

ByteArray ImuServerMeasurementAsciiMessage::myParameterNameStrings[] =
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
//    ByteArray("MAG_Z"),
    ByteArray("TIME_MS")
};

const Array<ByteArray> ImuServerMeasurementAsciiMessage::myParameterNames(
                                            myParameterNameStrings,
                                            ARRAY_SIZE(myParameterNameStrings));

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ImuServerMeasurementAsciiMessage::ImuServerMeasurementAsciiMessage(
                                               ImuServerMeasurementMessage& message) :
    AsciiMessage(myName, &myParameterNames),
    myMessage(message)
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ImuServerMeasurementAsciiMessage::~ImuServerMeasurementAsciiMessage()
{
}

//------------------------------------------------------------------------------
// Public virtual methods implemented from AsciiMessage
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ImuServerMeasurementAsciiMessage::stringParametersUpdated()
{
    myMessage.tempC =
               getFloatParameter(ImuServerMeasurementMessage::PARAMETER_TEMP_C);
    myMessage.accelX =
              getFloatParameter(ImuServerMeasurementMessage::PARAMETER_ACCEL_X);
    myMessage.accelY =
              getFloatParameter(ImuServerMeasurementMessage::PARAMETER_ACCEL_Y);
    myMessage.accelX =
              getFloatParameter(ImuServerMeasurementMessage::PARAMETER_ACCEL_Z);
    myMessage.gyroX =
               getFloatParameter(ImuServerMeasurementMessage::PARAMETER_GYRO_X);
    myMessage.gyroX =
               getFloatParameter(ImuServerMeasurementMessage::PARAMETER_GYRO_Y);
    myMessage.gyroZ =
               getFloatParameter(ImuServerMeasurementMessage::PARAMETER_GYRO_Z);
//    myMessage.magX = getFloatParameter(
//                                    ImuServerMeasurementMessage::PARAMETER_MAG_X));
//    myMessage.magY = getFloatParameter(
//                                    ImuServerMeasurementMessage::PARAMETER_MAG_Y));
//    myMessage.magZ = getFloatParameter(
//                                    ImuServerMeasurementMessage::PARAMETER_MAG_Z));
    myMessage.timeMs =
             getUint32Parameter(ImuServerMeasurementMessage::PARAMETER_TIME_MS);
}

//------------------------------------------------------------------------------
void ImuServerMeasurementAsciiMessage::messageParametersUpdated()
{
    setFloatParameter(ImuServerMeasurementMessage::PARAMETER_TEMP_C,
                      myMessage.tempC,
                      1);
    setFloatParameter(ImuServerMeasurementMessage::PARAMETER_ACCEL_X,
                      myMessage.accelX);
    setFloatParameter(ImuServerMeasurementMessage::PARAMETER_ACCEL_Y,
                      myMessage.accelY);
    setFloatParameter(ImuServerMeasurementMessage::PARAMETER_ACCEL_Z,
                      myMessage.accelZ);
    setFloatParameter(ImuServerMeasurementMessage::PARAMETER_GYRO_X,
                      myMessage.gyroX);
    setFloatParameter(ImuServerMeasurementMessage::PARAMETER_GYRO_Y,
                      myMessage.gyroY);
    setFloatParameter(ImuServerMeasurementMessage::PARAMETER_GYRO_Z,
                      myMessage.gyroZ);
//    setFloatParameter(ImuServerMeasurementMessage::PARAMETER_MAG_X,
//                      myMessage.magX);
//    setFloatParameter(ImuServerMeasurementMessage::PARAMETER_MAG_Y,
//                      myMessage.magY);
//    setFloatParameter(ImuServerMeasurementMessage::PARAMETER_MAG_Z,
//                      myMessage.magZ);
    setUint32Parameter(ImuServerMeasurementMessage::PARAMETER_TIME_MS,
                       myMessage.timeMs);
}
