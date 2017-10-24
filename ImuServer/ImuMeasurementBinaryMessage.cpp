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
/// @file ImuMeasurementBinaryMessage.cpp
/// @author Ben Minerd
/// @date 4/25/16
/// @brief ImuMeasurementBinaryMessage class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <ImuMeasurementBinaryMessage.h>
#include <ImuServerBinaryMessages.h>
#include <ByteArrayParser.h>

using Plat4m::ImuMeasurementBinaryMessage;
using namespace Plat4m::ImuServerBinaryMessages;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ImuMeasurementBinaryMessage::ImuMeasurementBinaryMessage(
                                               ImuMeasurementMessage& message) :
    BinaryMessage(groupId, measurementBinaryMessageId),
    myMessage(message)
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ImuMeasurementBinaryMessage::~ImuMeasurementBinaryMessage()
{
}

//------------------------------------------------------------------------------
// Public methods implemented from AsciiMessage
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool ImuMeasurementBinaryMessage::parseMessageData(const ByteArray& data)
{
    if (data.getSize() < 45)
    {
        return false;
    }

    ByteArrayParser byteArrayParser(data,
                                    ENDIAN_BIG,
                                    ByteArrayParser::PARSE_DIRECTION_FORWARD);

    return (byteArrayParser.parse(myMessage.index)  &&
            byteArrayParser.parse(myMessage.timeUs) &&
            byteArrayParser.parse(myMessage.accelX) &&
            byteArrayParser.parse(myMessage.accelY) &&
            byteArrayParser.parse(myMessage.accelZ) &&
            byteArrayParser.parse(myMessage.gyroX)  &&
            byteArrayParser.parse(myMessage.gyroY)  &&
            byteArrayParser.parse(myMessage.gyroZ)  &&
            byteArrayParser.parse(myMessage.magX)   &&
            byteArrayParser.parse(myMessage.magY)   &&
            byteArrayParser.parse(myMessage.magZ)   &&
			byteArrayParser.parse(myMessage.tempC));
}

//------------------------------------------------------------------------------
// Private methods implemented from BinaryMessage
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool ImuMeasurementBinaryMessage::binaryMessageToByteArray(
                                                     ByteArray& byteArray) const
{
    return (byteArray.append(myMessage.index,  ENDIAN_BIG) &&
            byteArray.append(myMessage.timeUs, ENDIAN_BIG) &&
            byteArray.append(myMessage.accelX, ENDIAN_BIG) &&
            byteArray.append(myMessage.accelY, ENDIAN_BIG) &&
            byteArray.append(myMessage.accelZ, ENDIAN_BIG) &&
            byteArray.append(myMessage.gyroX,  ENDIAN_BIG) &&
            byteArray.append(myMessage.gyroY,  ENDIAN_BIG) &&
            byteArray.append(myMessage.gyroZ,  ENDIAN_BIG) &&
            byteArray.append(myMessage.magX,   ENDIAN_BIG) &&
            byteArray.append(myMessage.magY,   ENDIAN_BIG) &&
            byteArray.append(myMessage.magZ,   ENDIAN_BIG) &&
			byteArray.append(myMessage.tempC,  ENDIAN_BIG));
}
