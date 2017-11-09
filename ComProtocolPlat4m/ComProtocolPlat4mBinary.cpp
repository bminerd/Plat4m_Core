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
/// @file ComProtocolPlat4mBinary.cpp
/// @author Ben Minerd
/// @date 3/1/16
/// @brief ComProtocolPlat4mBinary class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <ComProtocolPlat4mBinary.h>

using Plat4m::ComProtocolPlat4mBinary;
using Plat4m::ComProtocol;
using Plat4m::ByteArray;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ComProtocolPlat4mBinary::ComProtocolPlat4mBinary(ComLink& comLink) :
    ComProtocol(100, comLink),
	myFrameHandlerList(),
	myReceiveMessageByteArray()
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ComProtocolPlat4mBinary::~ComProtocolPlat4mBinary()
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ComProtocolPlat4mBinary::addFrameHandler(FrameHandler& messageHandler)
{
    FrameHandler* pointer = &messageHandler;
    myFrameHandlerList.append(pointer);
}

//------------------------------------------------------------------------------
void ComProtocolPlat4mBinary::transmitFrame(Frame& frame,
                                            const bool waitUntilDone)
{
    frame.toByteArray(frame.getData());

    getComLink().transmitBytes(frame.getData(), waitUntilDone);
}

//------------------------------------------------------------------------------
void ComProtocolPlat4mBinary::transmitReceiveFrame(Frame& transmitFrame,
                                                   Frame& receiveFrame)
{
    myReceiveMessageByteArray.clear();

    transmitFrame.toByteArray(transmitFrame.getData());

    getComLink().transmitBytes(transmitFrame.getData(), true);
}

//------------------------------------------------------------------------------
// Private methods implemented from ComProtocol
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ComProtocol::ParseStatus ComProtocolPlat4mBinary::driverParseData(
                                              const ByteArray& receiveByteArray,
                                              ByteArray& transmitByteArray,
                                              Callback<>*& followUpCallback)
{
    ParseStatus parseStatus = PARSE_STATUS_NOT_A_FRAME;

    if (receiveByteArray.getSize() == 0)
    {
        return parseStatus;
    }

    const uint8_t frameIdentifier = receiveByteArray[0];
    ByteArray frameData(receiveByteArray.subArray(1));
    Frame requestFrame(frameIdentifier, frameData);

    List<FrameHandler*>::Iterator iterator = myFrameHandlerList.iterator();

    while (iterator.hasCurrent())
    {
        FrameHandler* frameHandler = iterator.current();

        Frame* responseFrame = 0;

        parseStatus = frameHandler->handleFrame(requestFrame, responseFrame);

        if (parseStatus == PARSE_STATUS_FOUND_FRAME)
        {
            if (isValidPointer(responseFrame))
            {
                // TODO: Move this further up the parsing chain so things like
                // CRCs can be calculated before now
                responseFrame->toByteArray(transmitByteArray);
            }

//            Callback<>* handlerFollowUpCallback =
//                                            frameHandler->getFollowUpCallback();
//
//            if (isValidPointer(handlerFollowUpCallback))
//            {
//                followUpCallback = handlerFollowUpCallback;
//            }

            break;
        }
        else if (parseStatus == PARSE_STATUS_MID_FRAME)
        {
            int a = 0;

            break;
        }

        iterator.next();
    }

	return parseStatus;
}
