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
/// @file BinaryMessageFrameHandler.cpp
/// @author Ben Minerd
/// @date 4/28/16
/// @brief BinaryMessageFrameHandler class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <BinaryMessageFrameHandler.h>

using Plat4m::BinaryMessageFrameHandler;
using Plat4m::ByteArray;
using Plat4m::ComProtocol;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const uint8_t BinaryMessageFrameHandler::myBinaryMessageFrameIdentifier = 0xA1;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
BinaryMessageFrameHandler::BinaryMessageFrameHandler(
                             ComProtocolPlat4mBinary& comProtocolPlat4mBinary) :
    FrameHandler(myBinaryMessageFrameIdentifier),
    myComProtocolPlat4mBinary(comProtocolPlat4mBinary),
    myMessageHandlerGroupList(),
    myExpectedResponseBinaryMessage(0),
    myIsResponseReceived(false)
{
    myComProtocolPlat4mBinary.addFrameHandler(*this);
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
BinaryMessageFrameHandler::~BinaryMessageFrameHandler()
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void BinaryMessageFrameHandler::addMessageHandlerGroup(
                                 BinaryMessageHandlerGroup& messageHandlerGroup)
{
    BinaryMessageHandlerGroup* pointer = &messageHandlerGroup;

    myMessageHandlerGroupList.append(pointer);
}

//------------------------------------------------------------------------------
void BinaryMessageFrameHandler::transmitMessage(BinaryMessage& binaryMessage)
{
    myComProtocolPlat4mBinary.transmitFrame(binaryMessage);
}

//------------------------------------------------------------------------------
void BinaryMessageFrameHandler::transmitReceiveMessage(
                                      	   BinaryMessage& requestBinaryMessage,
										   BinaryMessage& responseBinaryMessage)
{
    myIsResponseReceived = false;
    myExpectedResponseBinaryMessage = &responseBinaryMessage;

    myComProtocolPlat4mBinary.transmitFrame(requestBinaryMessage);

    while (!myIsResponseReceived)
    {
        // Do nothing
    }

    myExpectedResponseBinaryMessage = 0;
}

//------------------------------------------------------------------------------
void BinaryMessageFrameHandler::transmitFrame(Frame& frame)
{
    myComProtocolPlat4mBinary.transmitFrame(frame);
}

//------------------------------------------------------------------------------
// Private methods implemented from FrameHandler
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ComProtocol::ParseStatus BinaryMessageFrameHandler::driverHandleFrame(
                                                      const Frame& requestFrame,
                                                      Frame*& responseFrame)
{
    ComProtocol::ParseStatus parseStatus =
                                    ComProtocol::PARSE_STATUS_UNSUPPORTED_FRAME;

    ByteArray frameData = requestFrame.getData();

    if (frameData.getSize() < 4)
    {
        parseStatus = ComProtocol::PARSE_STATUS_MID_FRAME;

        return parseStatus;
    }

    uint16_t groupId = (((uint16_t) frameData[0]) << 8) |
                        ((uint16_t) frameData[1]);

    uint16_t messageId = (((uint16_t) frameData[2]) << 8) |
                          ((uint16_t) frameData[3]);

    ByteArray messageData = frameData.subArray(4);
    BinaryMessage requestBinaryMessage(groupId, messageId, messageData);
    BinaryMessage* responseBinaryMessage = 0;

    if (isValidPointer(myExpectedResponseBinaryMessage))
    {
        if ((requestBinaryMessage.getGroupId() ==
                               myExpectedResponseBinaryMessage->getGroupId()) &&
            (requestBinaryMessage.getMessageId() ==
                               myExpectedResponseBinaryMessage->getMessageId()))
        {
            myIsResponseReceived =
                 myExpectedResponseBinaryMessage->parseMessageData(messageData);

            if (myIsResponseReceived)
            {
                parseStatus = ComProtocol::PARSE_STATUS_FOUND_FRAME;
            }

            return parseStatus;
        }
    }

    List<BinaryMessageHandlerGroup*>::Iterator iterator =
                                           myMessageHandlerGroupList.iterator();

    while (iterator.hasCurrent())
    {
        BinaryMessageHandlerGroup* messageHandlerGroup = iterator.current();

        parseStatus = messageHandlerGroup->handleMessage(requestBinaryMessage,
                                                         responseBinaryMessage);

        if (parseStatus == ComProtocol::PARSE_STATUS_FOUND_FRAME)
        {
            if (isValidPointer(responseBinaryMessage))
            {
                responseFrame = responseBinaryMessage;
            }

            break;
        }
        else if (parseStatus == ComProtocol::PARSE_STATUS_MID_FRAME)
        {
            break;
        }

        iterator.next();
    }

    return parseStatus;
}
