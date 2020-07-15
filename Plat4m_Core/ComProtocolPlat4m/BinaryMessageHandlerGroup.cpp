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
/// @file BinaryMessageHandlerGroup.cpp
/// @author Ben Minerd
/// @date 4/13/17
/// @brief BinaryMessageHandlerGroup class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/ComProtocolPlat4m/BinaryMessageHandlerGroup.h>

using Plat4m::BinaryMessageHandlerGroup;
using Plat4m::ByteArray;
using Plat4m::ComProtocol;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
BinaryMessageHandlerGroup::BinaryMessageHandlerGroup(
                                                const uint16_t messageGroupId) :
    myId(messageGroupId)
{
}

//------------------------------------------------------------------------------
// Protected virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
BinaryMessageHandlerGroup::~BinaryMessageHandlerGroup()
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
uint16_t BinaryMessageHandlerGroup::getId() const
{
    return myId;
}

//------------------------------------------------------------------------------
void BinaryMessageHandlerGroup::addMessageHandler(
                                           BinaryMessageHandler& messageHandler)
{
    BinaryMessageHandler* pointer = &messageHandler;

    myMessageHandlerList.append(pointer);
}

//------------------------------------------------------------------------------
ComProtocol::ParseStatus BinaryMessageHandlerGroup::handleMessage(
                                      const BinaryMessage& requestBinaryMessage,
                                      BinaryMessage*& responseBinaryMessage)
{
    ComProtocol::ParseStatus parseStatus =
                                    ComProtocol::PARSE_STATUS_UNSUPPORTED_FRAME;

    if (requestBinaryMessage.getGroupId() != myId)
    {
        return parseStatus;
    }

    List<BinaryMessageHandler*>::Iterator iterator =
                                                myMessageHandlerList.iterator();

    while (iterator.hasCurrent())
    {
        BinaryMessageHandler* messageHandler = iterator.current();

        parseStatus = messageHandler->handleMessage(requestBinaryMessage,
                                                    responseBinaryMessage);

        if (parseStatus == ComProtocol::PARSE_STATUS_FOUND_FRAME)
        {
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
