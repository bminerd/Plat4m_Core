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
/// @file ImuGetMeasurementBinaryMessage.cpp
/// @author Ben Minerd
/// @date 5/4/16
/// @brief ImuGetMeasurementBinaryMessage class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/ImuServer/ImuGetMeasurementBinaryMessage.h>
#include <Plat4m_Core/ImuServer/ImuServerBinaryMessages.h>
#include <Plat4m_Core/ByteArrayParser.h>

using Plat4m::ImuGetMeasurementBinaryMessage;
using namespace Plat4m::ImuServerBinaryMessages;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ImuGetMeasurementBinaryMessage::ImuGetMeasurementBinaryMessage(
                                            ImuGetMeasurementMessage& message) :
    BinaryMessage(groupId, getMeasurementBinaryMessageId),
    myMessage(message)
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ImuGetMeasurementBinaryMessage::~ImuGetMeasurementBinaryMessage()
{
}

//------------------------------------------------------------------------------
// Public methods implemented from BinaryMessage
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool ImuGetMeasurementBinaryMessage::parseMessageData(const ByteArray& data)
{
    if (data.getSize() < 1)
    {
        return false;
    }

    ByteArrayParser byteArrayParser(data,
                                    ENDIAN_BIG,
                                    ByteArrayParser::PARSE_DIRECTION_FORWARD);

    return (byteArrayParser.parse(myMessage.index));
}

//------------------------------------------------------------------------------
// Private methods implemented from BinaryMessage
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool ImuGetMeasurementBinaryMessage::binaryMessageToByteArray(
                                                     ByteArray& byteArray) const
{
    return (byteArray.append(myMessage.index, ENDIAN_BIG));
}
