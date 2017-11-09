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
/// @file BinaryMessageServer.cpp
/// @author Ben Minerd
/// @date 4/13/2017
/// @brief BinaryMessageServer class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <BinaryMessageServer.h>

using Plat4m::BinaryMessageServer;
using Plat4m::Module;

//------------------------------------------------------------------------------
// Protected constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
BinaryMessageServer::BinaryMessageServer(
                         const uint16_t groupId,
                         ComProtocolPlat4mBinary& comProtocolPlat4mBinary,
                         BinaryMessageFrameHandler& binaryMessageFrameHandler) :
    Module(),
    myComProtocolPlat4mBinary(comProtocolPlat4mBinary),
    myBinaryMessageFrameHandler(binaryMessageFrameHandler),
    myBinaryMessageHandlerGroup(groupId),
	myMessageByteArray()
{
    myBinaryMessageFrameHandler.addMessageHandlerGroup(
                                                   myBinaryMessageHandlerGroup);
}


//------------------------------------------------------------------------------
// Protected virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
BinaryMessageServer::~BinaryMessageServer()
{
}

//------------------------------------------------------------------------------
// Protected methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void BinaryMessageServer::addMessageHandler(
                                           BinaryMessageHandler& messageHandler)
{
    myBinaryMessageHandlerGroup.addMessageHandler(messageHandler);
}

//------------------------------------------------------------------------------
void BinaryMessageServer::transmitMessage(BinaryMessage& binaryMessage)
{
	myMessageByteArray.clear();

	binaryMessage.setData(myMessageByteArray);
    myBinaryMessageFrameHandler.transmitMessage(binaryMessage);
}

//------------------------------------------------------------------------------
void BinaryMessageServer::transmitFrame(Frame& frame)
{
	myMessageByteArray.clear();

	frame.setData(myMessageByteArray);
    myComProtocolPlat4mBinary.transmitFrame(frame);
}

//------------------------------------------------------------------------------
// Private virtual methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error BinaryMessageServer::interfaceSetEnabled(const bool enabled)
{
    return Module::Error(Module::ERROR_CODE_NONE);
}
