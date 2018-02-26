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
/// @file InsClient.cpp
/// @author Ben Minerd
/// @date 4/20/2017
/// @brief InsClient class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/InsServer/InsClient.h>
#include <Plat4m_Core/ComProtocolPlat4m/BinaryMessageHandlerTemplate.h>
#include <Plat4m_Core/InsServer/InsServerBinaryMessages.h>
#include <Plat4m_Core/InsServer/InsMeasurementBinaryMessage.h>
#include <Plat4m_Core/ByteArrayN.h>

using Plat4m::InsClient;
using Plat4m::Ins;
using namespace Plat4m::InsServerBinaryMessages;
using Plat4m::Module;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
InsClient::InsClient(ComProtocolPlat4mBinary& comProtocolPlat4mBinary,
					 BinaryMessageFrameHandler& binaryMessageFrameHandler) :
    Ins(),
    myComProtocolPlat4mBinary(comProtocolPlat4mBinary),
    myBinaryMessageFrameHandler(binaryMessageFrameHandler),
    myBinaryMessageHandlerGroup(groupId),
    myMeasurement()
{
	initialize();

	binaryMessageFrameHandler.addMessageHandlerGroup(
	                                               myBinaryMessageHandlerGroup);
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
InsClient::~InsClient()
{
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error InsClient::driverSetEnabled(const bool enabled)
{
    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods implemented from Ins
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Ins::Error InsClient::driverSetConfig(const Config& config)
{
    return Ins::Error(Ins::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
Ins::Error InsClient::driverGetMeasurement(Measurement& measurement)
{
    measurement = myMeasurement;

    return Ins::Error(Ins::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
Ins::Error InsClient::driverGetEulerAnglesDegrees(
                                                AngleDegrees& yawAngleDegrees,
                                                AngleDegrees& pitchAngleDegrees,
                                                AngleDegrees& rollAngleDegrees)
{
    yawAngleDegrees   = myMeasurement.rotationZAngleDegrees;
    pitchAngleDegrees = myMeasurement.rotationYAngleDegrees;
    rollAngleDegrees  = myMeasurement.rotationXAngleDegrees;

    return Ins::Error(Ins::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void InsClient::initialize()
{
    myBinaryMessageHandlerGroup.addMessageHandler(
		  createMessageHandler<
			  InsClient,
			  InsMeasurementMessage,
			  InsMeasurementBinaryMessage>(
				  this, &InsClient::insMeasurementMessageCallback));
}

//------------------------------------------------------------------------------
void InsClient::insMeasurementMessageCallback(
                                           const InsMeasurementMessage& message)
{
    myMeasurement.rotationXAngleDegrees = message.rotationX;
    myMeasurement.rotationYAngleDegrees = message.rotationY;
    myMeasurement.rotationZAngleDegrees = message.rotationZ;
    myMeasurement.rotationRateXAngularVelocityDps = message.rotationRateX;
    myMeasurement.rotationRateYAngularVelocityDps = message.rotationRateY;
    myMeasurement.rotationRateZAngularVelocityDps = message.rotationRateZ;

    measurementReady();
}
