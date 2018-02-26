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
// Copyright (c) 2018 Benjamin Minerd
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
/// @file ImuClient.cpp
/// @author Ben Minerd
/// @date 2/23/2018
/// @brief ImuClient class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/ImuServer/ImuClient.h>
#include <Plat4m_Core/ComProtocolPlat4m/BinaryMessageHandlerTemplate.h>
#include <Plat4m_Core/ImuServer/ImuServerBinaryMessages.h>
#include <Plat4m_Core/ImuServer/ImuMeasurementBinaryMessage.h>
#include <Plat4m_Core/ByteArrayN.h>

using Plat4m::ImuClient;
using Plat4m::Imu;
using namespace Plat4m::ImuServerBinaryMessages;
using Plat4m::Module;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ImuClient::ImuClient(ComProtocolPlat4mBinary& comProtocolPlat4mBinary,
                     BinaryMessageFrameHandler& binaryMessageFrameHandler) :
    Imu(),
    myComProtocolPlat4mBinary(comProtocolPlat4mBinary),
    myBinaryMessageFrameHandler(binaryMessageFrameHandler),
    myBinaryMessageHandlerGroup(groupId),
    myMeasurement()
{
    initialize();

    myBinaryMessageFrameHandler.addMessageHandlerGroup(
                                                   myBinaryMessageHandlerGroup);
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ImuClient::~ImuClient()
{
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error ImuClient::driverSetEnabled(const bool enabled)
{
    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods implemented from Imu
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Imu::Error ImuClient::driverSetConfig(const Imu::Config& config)
{
    // TODO: Put config message here
    return Imu::Error(Imu::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
Imu::Error ImuClient::driverGetMeasurement(Measurement& measurement)
{
    measurement = myMeasurement;

    return Imu::Error(Imu::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ImuClient::initialize()
{
    myBinaryMessageHandlerGroup.addMessageHandler(
    createMessageHandler<ImuClient,
                         ImuMeasurementMessage,
                         ImuMeasurementBinaryMessage>(
                              this, &ImuClient::imuMeasurementMessageCallback));
}

//------------------------------------------------------------------------------
void ImuClient::imuMeasurementMessageCallback(
                                           const ImuMeasurementMessage& message)
{
    myMeasurement.xAccelerationG      = message.accelX;
    myMeasurement.yAccelerationG      = message.accelY;
    myMeasurement.zAccelerationG      = message.accelZ;
    myMeasurement.xAngularVelocityDps = message.gyroX;
    myMeasurement.yAngularVelocityDps = message.gyroY;
    myMeasurement.zAngularVelocityDps = message.gyroZ;
    myMeasurement.magX                = 0.0;
    myMeasurement.magY                = 0.0;
    myMeasurement.magZ                = 0.0;

    measurementReady();
}
