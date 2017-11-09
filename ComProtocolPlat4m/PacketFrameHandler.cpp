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
/// @file PacketFrameHandler.cpp
/// @author Ben Minerd
/// @date 4/28/16
/// @brief PacketFrameHandler class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <PacketFrameHandler.h>
#include <Crc.h>

using Plat4m::PacketFrameHandler;
using Plat4m::ByteArray;
using Plat4m::ComProtocol;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const uint8_t PacketFrameHandler::myPacketFrameIdentifier = 0xA2;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
PacketFrameHandler::PacketFrameHandler(
                             ComProtocolPlat4mBinary& comProtocolPlat4mBinary,
                             FrameHandler& embeddedFrameHandler) :
    FrameHandler(myPacketFrameIdentifier),
    myComProtocolPlat4mBinary(comProtocolPlat4mBinary),
    myEmbeddedFrameHandler(embeddedFrameHandler),
    myResponsePacket(),
    myPacketNumber(0)
{
    myComProtocolPlat4mBinary.addFrameHandler(*this);
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
PacketFrameHandler::~PacketFrameHandler()
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void PacketFrameHandler::transmitFrame(Frame& frame)
{
    myResponsePacket.setNumber(myPacketNumber);
    myResponsePacket.setFrame(frame);
    myComProtocolPlat4mBinary.transmitFrame(myResponsePacket);

    if (myPacketNumber == 255)
    {
        myPacketNumber = 0;
    }
    else
    {
        myPacketNumber++;
    }
}

//------------------------------------------------------------------------------
// Private methods implemented from FrameHandler
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ComProtocol::ParseStatus PacketFrameHandler::driverHandleFrame(
                                                      const Frame& requestFrame,
                                                      Frame*& responseFrame)
{
    ComProtocol::ParseStatus parseStatus =
                                    ComProtocol::PARSE_STATUS_UNSUPPORTED_FRAME;

    ByteArray frameData = requestFrame.getData();

    if (frameData.getSize() < 5)
    {
        parseStatus = ComProtocol::PARSE_STATUS_MID_FRAME;

        return parseStatus;
    }

    uint8_t packetNumber   = frameData[0];
    uint16_t dataByteCount = (((uint16_t) frameData[1]) << 8) |
                              ((uint16_t) frameData[2]);
    uint16_t crc           = (((uint16_t) frameData[3]) << 8) |
                              ((uint16_t) frameData[4]);

    if (frameData.getSize() < (5 + dataByteCount))
    {
        parseStatus = ComProtocol::PARSE_STATUS_MID_FRAME;

        return parseStatus;
    }

    ByteArray packetData = frameData.subArray(5, dataByteCount);

    if (!Crc::checkCrc16Ccitt(packetData, crc))
    {
        parseStatus = ComProtocol::PARSE_STATUS_INVALID_FRAME;

        return parseStatus;
    }

//    Packet packet(packetNumber, dataByteCount, crc, packetData);

    // TODO: Find a cleaner way to do this
    const uint8_t embeddedFrameIdentifier = packetData[0];
    ByteArray embeddedFrameData(packetData.subArray(1));
    Frame embeddedRequestFrame(embeddedFrameIdentifier, embeddedFrameData);

    parseStatus =
        myEmbeddedFrameHandler.handleFrame(embeddedRequestFrame, responseFrame);

    if (parseStatus == ComProtocol::PARSE_STATUS_FOUND_FRAME)
    {
        if (isValidPointer(responseFrame))
        {
            myResponsePacket.setNumber(myPacketNumber);
            myResponsePacket.setFrame(*responseFrame);
            responseFrame = &myResponsePacket;

            if (myPacketNumber == 255)
            {
                myPacketNumber = 0;
            }
            else
            {
                myPacketNumber++;
            }
        }
    }

    return parseStatus;
}
