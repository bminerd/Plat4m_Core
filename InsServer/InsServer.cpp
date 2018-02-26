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
/// @file InsServer.cpp
/// @author Ben Minerd
/// @date 4/20/2017
/// @brief InsServer class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <math.h>

#include <Plat4m_Core/InsServer/InsServer.h>
#include <Plat4m_Core/ComProtocolPlat4m/BinaryMessageHandlerTemplate.h>
#include <Plat4m_Core/InsServer/InsServerBinaryMessages.h>
#include <Plat4m_Core/InsServer/InsMeasurementBinaryMessage.h>
#include <Plat4m_Core/InsServer/InsServerSetConfigBinaryMessage.h>
#include <Plat4m_Core/InsServer/InsServerSetConfigResponseBinaryMessage.h>
#include <Plat4m_Core/InsServer/InsServerGetConfigBinaryMessage.h>
#include <Plat4m_Core/InsServer/InsServerGetConfigResponseBinaryMessage.h>
#include <Plat4m_Core/ByteArrayN.h>

using Plat4m::InsServer;
using namespace Plat4m::InsServerBinaryMessages;
using Plat4m::Module;
//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
InsServer::InsServer(ComProtocolPlat4mBinary& comProtocolPlat4mBinary,
					 BinaryMessageFrameHandler& binaryMessageFrameHandler) :
    BinaryMessageServer(groupId,
    					comProtocolPlat4mBinary,
						binaryMessageFrameHandler),
    myConfig(),
	myInsList(),
    myOutputThread(System::createThread(
                       createCallback(this, &InsServer::outputThreadCallback))),
	myWaitCondition(System::createWaitCondition(myOutputThread)),
	myUpdateCount(0)
{
	initialize();
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
InsServer::~InsServer()
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool InsServer::addIns(Ins& ins)
{
    if (myInsList.size() >= 256)
    {
        return false;
    }

    Ins* pointer = &ins;
    myInsList.append(pointer);

    return true;
}

//------------------------------------------------------------------------------
InsServer::Config InsServer::getConfig() const
{
    return myConfig;
}

//------------------------------------------------------------------------------
InsServer::Error InsServer::setConfig(const Config& config)
{
    if (!isEnabled())
    {
        return Error(ERROR_CODE_NOT_ENABLED);
    }

    myConfig = config;
    myUpdateCount = 0;

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
void InsServer::measurementReadyCallback()
{
	if (myConfig.outputRateDivisionFactor != 0)
	{
		myUpdateCount++;

		if (myUpdateCount == myConfig.outputRateDivisionFactor)
		{
//			myWaitCondition.notifyFast();
		    myWaitCondition.notify();

			myUpdateCount = 0;
		}
	}
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error InsServer::driverSetEnabled(const bool enabled)
{
	myOutputThread.setEnabled(enabled);

    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void InsServer::initialize()
{
    myOutputThread.setPriority(2);

    addMessageHandler(
		  createMessageHandler<
			  InsServer,
			  InsServerSetConfigMessage,
			  InsServerSetConfigResponseMessage,
			  InsServerSetConfigBinaryMessage,
			  InsServerSetConfigResponseBinaryMessage>(
				  this, &InsServer::insServerSetConfigMessageCallback));
}

//------------------------------------------------------------------------------
void InsServer::outputThreadCallback()
{
	myWaitCondition.wait();

    uint8_t i = 0;
    List<Ins*>::Iterator iterator = myInsList.iterator();

    while (iterator.hasCurrent())
    {
    	Ins* ins = iterator.current();

    	Ins::Measurement measurement;
    	Ins::Error error = ins->getLastMeasurement(measurement);

		InsMeasurementMessage message;
		message.index    	  = i;
        message.timeUs   	  = System::getTimeUs();
		message.rotationX 	  = measurement.rotationXAngleDegrees;
		message.rotationY 	  = measurement.rotationYAngleDegrees;
		message.rotationZ 	  = measurement.rotationZAngleDegrees;
		message.rotationRateX = measurement.rotationRateXAngularVelocityDps;
		message.rotationRateY = measurement.rotationRateYAngularVelocityDps;
		message.rotationRateZ = measurement.rotationRateZAngularVelocityDps;

        InsMeasurementBinaryMessage binaryMessage(message);
        transmitMessage(binaryMessage);

        i++;
        iterator.next();
    }
}

//------------------------------------------------------------------------------
void InsServer::insServerSetConfigMessageCallback(
                                    const InsServerSetConfigMessage& request,
                                    InsServerSetConfigResponseMessage& response)
{
    Config config;
    config.outputRateDivisionFactor = request.outputRateDivisionFactor;

    Error error = setConfig(config);

    response.error = (uint8_t) error.getCode();
}
