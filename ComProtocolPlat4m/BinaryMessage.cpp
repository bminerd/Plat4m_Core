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
/// @file BinaryMessage.cpp
/// @author Ben Minerd
/// @date 4/25/2016
/// @brief BinaryMessage class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/ComProtocolPlat4m/BinaryMessage.h>
#include <Plat4m_Core/ByteArrayParser.h>

using Plat4m::BinaryMessage;
using Plat4m::ByteArray;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const uint8_t BinaryMessage::myFrameIdentifier = 0xA1;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
BinaryMessage::BinaryMessage(const uint16_t groupId, const uint16_t messageId) :
    Frame(myFrameIdentifier),
    myGroupId(groupId),
    myMessageId(messageId),
    myData()
{
}

//------------------------------------------------------------------------------
BinaryMessage::BinaryMessage(const uint16_t groupId,
                             const uint16_t messageId,
                             ByteArray& data) :
    Frame(myFrameIdentifier),
    myGroupId(groupId),
    myMessageId(messageId),
    myData(data)
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
BinaryMessage::~BinaryMessage()
{
}


//------------------------------------------------------------------------------
// Public virtual methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool BinaryMessage::parseMessageData(const ByteArray& data)
{
    // Not implemented by subclass, default behavior (message contains no data)

    return true;
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
uint16_t BinaryMessage::getGroupId() const
{
    return myGroupId;
}

//------------------------------------------------------------------------------
uint16_t BinaryMessage::getMessageId() const
{
    return myMessageId;
}

//------------------------------------------------------------------------------
const ByteArray& BinaryMessage::getData() const
{
    return myData;
}

//------------------------------------------------------------------------------
// Private methods implemented from Frame
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool BinaryMessage::frameToByteArray(ByteArray& byteArray) const
{
    return (byteArray.append(myGroupId,   ENDIAN_BIG) &&
            byteArray.append(myMessageId, ENDIAN_BIG) &&
            binaryMessageToByteArray(byteArray));
}

//------------------------------------------------------------------------------
bool BinaryMessage::frameParseByteArray(const ByteArray& byteArray)
{
    if (byteArray.getSize() < 4)
    {
        return false;
    }

    ByteArrayParser byteArrayParser(byteArray,
                                    ENDIAN_BIG,
                                    ByteArrayParser::PARSE_DIRECTION_FORWARD);

    return (byteArrayParser.parse(myGroupId)   &&
            byteArrayParser.parse(myMessageId) &&
            binaryMessageParseByteArray(byteArray.subArray(4)));
}

//------------------------------------------------------------------------------
// Private virtual methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool BinaryMessage::binaryMessageToByteArray(ByteArray& byteArray) const
{
    // Not implemented by subclass, default behavior

    return true;
}

//------------------------------------------------------------------------------
bool BinaryMessage::binaryMessageParseByteArray(const ByteArray& byteArray)
{
    // Not implemented by subclass, default behavior

    return true;
}
