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
/// @file ImuSetConfigResponseBinaryMessage.cpp
/// @author Ben Minerd
/// @date 4/21/2017
/// @brief ImuSetConfigResponseBinaryMessage class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/ImuServer/ImuSetConfigResponseBinaryMessage.h>
#include <Plat4m_Core/ImuServer/ImuServerBinaryMessages.h>
#include <Plat4m_Core/ByteArrayParser.h>

using Plat4m::ImuSetConfigResponseBinaryMessage;
using namespace Plat4m::ImuServerBinaryMessages;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ImuSetConfigResponseBinaryMessage::ImuSetConfigResponseBinaryMessage(
                                         ImuSetConfigResponseMessage& message) :
    BinaryMessage(groupId, imuSetConfigResponseBinaryMessageId),
    myMessage(message)
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ImuSetConfigResponseBinaryMessage::~ImuSetConfigResponseBinaryMessage()
{
}

//------------------------------------------------------------------------------
// Public methods implemented from BinaryMessage
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool ImuSetConfigResponseBinaryMessage::parseMessageData(const ByteArray& data)
{
    if (data.getSize() < 1)
    {
        return false;
    }

    ByteArrayParser byteArrayParser(data,
                                    ENDIAN_BIG,
                                    ByteArrayParser::PARSE_DIRECTION_FORWARD);

    return (byteArrayParser.parse(myMessage.error));
}

//------------------------------------------------------------------------------
// Private methods implemented from BinaryMessage
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool ImuSetConfigResponseBinaryMessage::binaryMessageToByteArray(
                                                     ByteArray& byteArray) const
{
    return (byteArray.append(myMessage.error, ENDIAN_BIG));
}
