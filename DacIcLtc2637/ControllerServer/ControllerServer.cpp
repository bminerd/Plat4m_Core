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
/// @file ControllerServer.cpp
/// @author Ben Minerd
/// @date 6/8/2016
/// @brief ControllerServer class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <ControllerServer.h>
#include <BinaryMessageHandlerTemplate.h>
#include <ControllerServerGetCountBinaryMessage.h>
#include <ControllerServerGetCountResponseBinaryMessage.h>
#include <ControllerServerSetConfigBinaryMessage.h>
#include <ControllerServerSetConfigResponseBinaryMessage.h>
#include <ControllerUpdateMessage.h>
#include <ControllerUpdateBinaryMessage.h>
#include <ByteArrayN.h>

using Plat4m::Control::ControllerServer;
using Plat4m::Module;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const Plat4m::TimeMs ControllerServer::myOutputThreadPeriodMs = 0;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ControllerServer::ControllerServer(
                         ComProtocolPlat4mBinary& comProtocolPlat4mBinary,
                         BinaryMessageFrameHandler& binaryMessageFrameHandler) :
    BinaryMessageServer(0x0002,
                        comProtocolPlat4mBinary,
                        binaryMessageFrameHandler),
    myConfig(),
	myWaitCondition(System::createWaitCondition()),
    myControllerList(),
    myOutputThread(System::createThread(
                  createCallback(this, &ControllerServer::outputThreadCallback),
                  myOutputThreadPeriodMs)),
    myUpdateCount(0)
{
    initialize();
}


//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ControllerServer::~ControllerServer()
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool ControllerServer::addController(Controller& controller)
{
    if (myControllerList.size() >= 256)
    {
        return false;
    }

    Controller* pointer = &controller;
    myControllerList.append(pointer);

    controller.setUpdateCallback(createCallback(
								  this,
								  &ControllerServer::controllerUpdateCallback));

    return true;
}

//------------------------------------------------------------------------------
ControllerServer::Config ControllerServer::getConfig() const
{
    return myConfig;
}

//------------------------------------------------------------------------------
ControllerServer::Error ControllerServer::setConfig(const Config& config)
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
// Private virtual methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error ControllerServer::driverSetEnabled(const bool enabled)
{
	myOutputThread.setEnabled(enabled);

    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ControllerServer::initialize()
{
    myOutputThread.setPriority(2);

    addMessageHandler(
               createMessageHandler<
                   ControllerServer,
                   ControllerServerGetCountMessage,
                   ControllerServerGetCountResponseMessage,
                   ControllerServerGetCountBinaryMessage,
                   ControllerServerGetCountResponseBinaryMessage>(
                       this, &ControllerServer::serverGetCountMessageCallback));

    addMessageHandler(
              createMessageHandler<
                  ControllerServer,
                  ControllerServerSetConfigMessage,
                  ControllerServerSetConfigResponseMessage,
                  ControllerServerSetConfigBinaryMessage,
                  ControllerServerSetConfigResponseBinaryMessage>(
                      this, &ControllerServer::serverSetConfigMessageCallback));
}

//------------------------------------------------------------------------------
void ControllerServer::controllerUpdateCallback()
{
	if (myConfig.outputRateDivisionFactor != 0)
	{
		myUpdateCount++;

		if (myUpdateCount == myConfig.outputRateDivisionFactor)
		{
			myWaitCondition.notifyFast();

			myUpdateCount = 0;
		}
	}
}

//------------------------------------------------------------------------------
void ControllerServer::outputThreadCallback()
{
	myWaitCondition.waitFast();

    uint8_t i = 0;
    List<Controller*>::Iterator iterator = myControllerList.iterator();

    while (iterator.hasCurrent())
    {
        Controller* controller = iterator.current();

        ControllerUpdateMessage message;
        message.index    = i;
        message.timeUs   = System::getTimeUs();
        message.setPoint = controller->getSetPoint();
        message.feedback = controller->getFeedback();
        message.error    = controller->getError();
        message.output   = controller->getOutput();

        ControllerUpdateBinaryMessage binaryMessage(message);
        transmitMessage(binaryMessage);

        i++;
        iterator.next();
    }
}

//------------------------------------------------------------------------------
void ControllerServer::serverSetConfigMessageCallback(
                             const ControllerServerSetConfigMessage& request,
                             ControllerServerSetConfigResponseMessage& response)
{
    Config config;
    config.outputRateDivisionFactor = request.outputRateDivisionFactor;

    Error error = setConfig(config);

    response.error = (uint8_t) error.getCode();
}

//------------------------------------------------------------------------------
void ControllerServer::serverGetCountMessageCallback(
                              const ControllerServerGetCountMessage& request,
                              ControllerServerGetCountResponseMessage& response)
{
    response.count = myControllerList.size();
}
