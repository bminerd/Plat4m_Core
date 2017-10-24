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
/// @file Can.cpp
/// @author Ben Minerd
/// @date 1/4/2016
/// @brief Can class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Can.h>
#include <System.h>

using Plat4m::Can;

//------------------------------------------------------------------------------
// Public virtual methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Can::Error Can::setConfig(const Config& config)
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
Can::Error Can::addReceivedMessageHandler(
							   const IdType idType,
				          	   const uint32_t id,
							   const uint32_t mask,
		                  	   ReceivedMessageCallback& receivedMessageCallback)
{
	ReceivedMessageHandler receivedMessageHandler;
	receivedMessageHandler.idType 			       = idType;
	receivedMessageHandler.id		 			   = id;
	receivedMessageHandler.mask		 			   = mask;
	receivedMessageHandler.receivedMessageCallback = &receivedMessageCallback;

	return addReceivedMessageHandler(receivedMessageHandler);
}

//------------------------------------------------------------------------------
Can::Error Can::addReceivedMessageHandler(
					  	         ReceivedMessageHandler& receivedMessageHandler)
{
	Error error = driverAddAcceptanceFilter(receivedMessageHandler.idType,
										    receivedMessageHandler.id,
											receivedMessageHandler.mask);

	if (error.getCode() == ERROR_CODE_NONE)
	{
		myReceivedMessageHandlerList.append(receivedMessageHandler);
	}

	return error;
}

//------------------------------------------------------------------------------
Can::Error Can::sendMessage(const IdType idType,
							const uint32_t id,
							const FrameType frameType,
							ByteArrayN<8>& data)
{
    Message message;
    message.idType    = idType;
    message.id 		  = id;
    message.frameType = frameType;
    message.data	  = data;

    return sendMessage(message);
}

//------------------------------------------------------------------------------
Can::Error Can::sendMessage(const Message& message)
{
    if (!isEnabled())
    {
        return Error(ERROR_CODE_NOT_ENABLED);
    }

    Error error = driverSendMessage(message);

    return error;
}

//------------------------------------------------------------------------------
void Can::handleReceivedMessages()
{
	while (!(myReceivedMessageBuffer.isEmpty()))
	{
		Message message;
		myReceivedMessageBuffer.read(message);

		bool wasHandlerFound = false;
		List<ReceivedMessageHandler>::Iterator iterator =
										myReceivedMessageHandlerList.iterator();

		while (iterator.hasCurrent() && !wasHandlerFound)
		{
			ReceivedMessageHandler& handler = iterator.current();

			if ((message.id & handler.mask) == (handler.id))
			{
				handler.receivedMessageCallback->call(message);
				wasHandlerFound = true;
			}

			iterator.next();
		}

		if (!wasHandlerFound)
		{
			// Shouldn't have received this message

			break;
		}
	}
}

//------------------------------------------------------------------------------
// Protected constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Can::Can() :
	Module(),
	myReceivedMessageHandlerList(),
	myTransmitMessageBuffer(),
	myReceivedMessageBuffer()
{
}

//------------------------------------------------------------------------------
// Protected virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Can::~Can()
{
}

//------------------------------------------------------------------------------
// Protected methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void Can::interfaceMessageReceived(Message& message)
{
	myReceivedMessageBuffer.write(message);
}
