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
/// @file PacketFrameHandler.h
/// @author Ben Minerd
/// @date 5/9/2017
/// @brief PacketFrameHandler class header file.
///

#ifndef PLAT4M_PACKET_FRAME_HANDLER_H
#define PLAT4M_PACKET_FRAME_HANDLER_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <FrameHandler.h>
#include <ByteArrayN.h>
#include <List.h>
#include <ComProtocolPlat4mBinary.h>
#include <ComProtocol.h>
#include <Packet.h>

#include <stdint.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class PacketFrameHandler : public FrameHandler
{
public:

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------
    
    PacketFrameHandler(ComProtocolPlat4mBinary& comProtocolPlat4mBinary,
                       FrameHandler& embeddedFrameHandler);

    //--------------------------------------------------------------------------
    // Public destructors
    //--------------------------------------------------------------------------

    virtual ~PacketFrameHandler();

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    void transmitFrame(Frame& frame);

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------
    
    static const uint8_t myPacketFrameIdentifier;

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    ComProtocolPlat4mBinary& myComProtocolPlat4mBinary;

    FrameHandler& myEmbeddedFrameHandler;

    Packet myResponsePacket;

    uint8_t myPacketNumber;

    //--------------------------------------------------------------------------
    // Private methods implemented from FrameHandler
    //--------------------------------------------------------------------------

    ComProtocol::ParseStatus driverHandleFrame(const Frame& requestFrame,
                                               Frame*& responseFrame);
};

}; // namespace Plat4m

#endif // PLAT4M_PACKET_FRAME_HANDLER_H
