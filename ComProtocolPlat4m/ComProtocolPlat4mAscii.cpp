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
/// @file ComProtocolPlat4mAscii.cpp
/// @author Ben Minerd
/// @date 3/1/16
/// @brief ComProtocolPlat4mAscii class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/ComProtocolPlat4m/ComProtocolPlat4mAscii.h>

using Plat4m::ComProtocolPlat4mAscii;
using Plat4m::ComProtocol;
using Plat4m::ByteArray;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ComProtocolPlat4mAscii::ComProtocolPlat4mAscii(ComLink& comLink,
                                               const bool addNewLine) :
    ComProtocol(100, comLink),
    myAddNewLine(addNewLine),
    myRawMessageByteArray(),
    myMessageNameStorage(),
    myParameterNameStorage(),
    myParameterNameStorageArray(),
    myParameterValueStorage(),
    myParameterValueStorageArray(),
    myAsciiMessageTemplate(),
    myMessageHandlerList()
{
    for (int i = 0; i < ARRAY_SIZE(myParameterNameStorage); i++)
    {
        myParameterNameStorageArray[i].setItems(
                                          myParameterNameStorage[i],
                                          ARRAY_SIZE(myParameterNameStorage[i]),
                                          0);
    }

    for (int i = 0; i < ARRAY_SIZE(myParameterValueStorage); i++)
    {
        myParameterValueStorageArray[i].setItems(
                                         myParameterValueStorage[i],
                                         ARRAY_SIZE(myParameterValueStorage[i]),
                                         0);
    }
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ComProtocolPlat4mAscii::~ComProtocolPlat4mAscii()
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ComProtocolPlat4mAscii::addMessageHandler(
                                            AsciiMessageHandler& messageHandler)
{
    AsciiMessageHandler* pointer = &messageHandler;
    myMessageHandlerList.append(pointer);
}

//------------------------------------------------------------------------------
void ComProtocolPlat4mAscii::getMessageStorage(AsciiMessage& message)
{
    message.setParameterValues(&myParameterValueStorageArray);
    message.messageParametersUpdated();
}

//------------------------------------------------------------------------------
ByteArray& ComProtocolPlat4mAscii::messageToByteArray(AsciiMessage& message)
{
    getMessageStorage(message);
    myRawMessageByteArray.clear(true);
    message.toByteArray(myRawMessageByteArray);

    if (myAddNewLine)
    {
        myRawMessageByteArray.append('\n');
    }

    return myRawMessageByteArray;
}

//------------------------------------------------------------------------------
// Private methods implemented from ComProtocol
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ComProtocol::ParseStatus ComProtocolPlat4mAscii::driverParseData(
                                                  const ByteArray& rxByteArray,
                                                  ByteArray& txByteArray,
                                                  Callback<>*& followUpCallback)
{
	ParseStatus parseStatus =
	                    myAsciiMessageTemplate.parseByteArray(
                                                  rxByteArray,
                                                  myMessageNameStorage,
                                                  myParameterNameStorageArray,
                                                  myParameterValueStorageArray);

	if (parseStatus == PARSE_STATUS_FOUND_FRAME)
	{
	    List<AsciiMessageHandler*>::Iterator iterator =
                                                myMessageHandlerList.iterator();

        while (iterator.hasCurrent())
        {
            AsciiMessageHandler* messageHandler = iterator.current();

            AsciiMessage* response = 0;

            if (messageHandler->handleMessage(myAsciiMessageTemplate, response))
            {
    //            Callback<>* handlerFollowUpCallback =
    //                                          messageHandler->getFollowUpCallback();
    //
    //            if (isValidPointer(handlerFollowUpCallback))
    //            {
    //                followUpCallback = handlerFollowUpCallback;
    //            }

                response->toByteArray(txByteArray);

                if (myAddNewLine)
                {
                    txByteArray.append('\n');
                }

                break;
            }

            iterator.next();
        }
	}

	return parseStatus;
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

