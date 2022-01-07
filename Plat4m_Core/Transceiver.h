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
// Copyright (c) 2022 Benjamin Minerd
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
/// @file Transceiver.h
/// @author Ben Minerd
/// @date 1/6/2022
/// @brief Transceiver class header file.
///

#ifndef PLAT4M_TRANSCEIVER_H
#define PLAT4M_TRANSCEIVER_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>

#include <Plat4m_Core/Module.h>
#include <Plat4m_Core/ErrorTemplate.h>
#include <Plat4m_Core/Callback.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class Transceiver : public Module
{
public:

    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------

    enum ErrorCode
    {
        ERROR_CODE_NONE
    };

    typedef ErrorTemplate<ErrorCode> Error;

    // typedef Callback<void, std::uint8_t> PacketReceivedCallback;

    //--------------------------------------------------------------------------
    // Public pure virtual methods
    //--------------------------------------------------------------------------

    // virtual Error transmitPacket(const Packet& packet,
    //                              const bool waitUntilDone = true) = 0;

    // virtual std::uint32_t getReceivedPacketsCount() = 0;

    // virtual Error getReceivedPackets(Plat4m::Array<Packet>& packetArray,
    //                                  const std::uint32_t nPackets = 0) = 0;

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    // void setPacketReceivedCallback(PacketReceivedCallback& callback);

protected:

    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------

    Transceiver();

    //--------------------------------------------------------------------------
    // Protected virtual destructors
    //--------------------------------------------------------------------------

    virtual ~Transceiver();

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    PacketReceivedCallback* myPacketReceivedCallback;
};

}; // namespace Plat4m

#endif // PLAT4M_TRANSCEIVER_H
