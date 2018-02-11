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
/// @file Packet.cpp
/// @author Ben Minerd
/// @date 5/8/2017
/// @brief Packet class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/ComProtocolPlat4m/Packet.h>
#include <Plat4m_Core/ByteArrayParser.h>
#include <Plat4m_Core/Crc.h>

using Plat4m::Packet;
using Plat4m::Frame;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const uint8_t Packet::myFrameIdentifier = 0xA2;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Packet::Packet() :
    Frame(myFrameIdentifier),
    myNumber(0),
    myDataByteCount(0),
    myCrc(0),
    myFrame(0)
{
}

//------------------------------------------------------------------------------
Packet::Packet(const uint8_t number,
               const uint16_t dataByteCount,
               const uint16_t crc,
               Frame& frame) :
    Frame(myFrameIdentifier),
    myNumber(number),
    myDataByteCount(dataByteCount),
    myCrc(crc),
    myFrame(&frame)
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Packet::~Packet()
{
}

//------------------------------------------------------------------------------
// Public virtual methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool Packet::parseMessageData(const ByteArray& data)
{
    // Not implemented by subclass, default behavior (message contains no data)

    return true;
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
uint8_t Packet::getNumber() const
{
    return myNumber;
}

//------------------------------------------------------------------------------
void Packet::setNumber(const uint8_t number)
{
    myNumber = number;
}

//------------------------------------------------------------------------------
uint16_t Packet::getDataByteCount() const
{
    return myDataByteCount;
}

//------------------------------------------------------------------------------
void Packet::setDataByteCount(const uint16_t dataByteCount)
{
    myDataByteCount = dataByteCount;
}

//------------------------------------------------------------------------------
uint16_t Packet::getCrc() const
{
    return myCrc;
}

//------------------------------------------------------------------------------
void Packet::setCrc(const uint16_t crc)
{
    myCrc = crc;
}

//------------------------------------------------------------------------------
const Frame& Packet::getFrame() const
{
    return *myFrame;
}

//------------------------------------------------------------------------------
void Packet::setFrame(Frame& frame)
{
    myFrame = &frame;
}

//------------------------------------------------------------------------------
// Private methods implemented from Frame
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool Packet::frameToByteArray(ByteArray& byteArray) const
{
    uint16_t dataByteCount = 0;
    uint16_t crc = 0;

    bool toByteArraySucceeded =
                               (byteArray.append(myNumber,      ENDIAN_BIG) &&
                                byteArray.append(dataByteCount, ENDIAN_BIG) &&
                                byteArray.append(crc,           ENDIAN_BIG) &&
                                myFrame->toByteArray(byteArray));

    if (!toByteArraySucceeded)
    {
        return false;
    }

    dataByteCount = byteArray.getSize() - 5;

    byteArray[2] = (dataByteCount >> 8) & 0xFF;
    byteArray[3] = dataByteCount & 0xFF;

    crc = Crc::calculateCrc16Ccitt(byteArray.subArray(5));

    byteArray[4] = (crc >> 8) & 0xFF;
    byteArray[5] = crc & 0xFF;

    return true;
}

//------------------------------------------------------------------------------
bool Packet::frameParseByteArray(const ByteArray& byteArray)
{
    if (byteArray.getSize() < 5)
    {
        return false;
    }

    ByteArrayParser byteArrayParser(byteArray,
                                    ENDIAN_BIG,
                                    ByteArrayParser::PARSE_DIRECTION_FORWARD);

    return (byteArrayParser.parse(myNumber)                &&
            byteArrayParser.parse(myDataByteCount)         &&
            byteArrayParser.parse(myCrc)                   &&
            (myDataByteCount == (byteArray.getSize() - 5)) &&
            myFrame->parseByteArray(byteArray.subArray(5)));
}
