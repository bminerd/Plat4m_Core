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
/// @file Packet.h
/// @author Ben Minerd
/// @date 5/8/2017
/// @brief Packet class header file.
///

#ifndef PLAT4M_PACKET_H
#define PLAT4M_PACKET_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Frame.h>
#include <ByteArray.h>

#include <stdint.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class Packet : public Frame
{
public:
    
    //--------------------------------------------------------------------------
    // Public enumerations
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    Packet();

    Packet(const uint8_t number,
           const uint16_t dataByteCount,
           const uint16_t crc,
           Frame& frame);

    //--------------------------------------------------------------------------
    // Public destructors
    //--------------------------------------------------------------------------

    virtual ~Packet();

    //--------------------------------------------------------------------------
    // Public virtual methods
    //--------------------------------------------------------------------------

    virtual bool parseMessageData(const ByteArray& data);

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    uint8_t getNumber() const;

    void setNumber(const uint8_t number);

    uint16_t getDataByteCount() const;

    void setDataByteCount(const uint16_t dataByteCount);

    uint16_t getCrc() const;

    void setCrc(const uint16_t crc);

    const Frame& getFrame() const;

    void setFrame(Frame& data);

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    static const uint8_t myFrameIdentifier;
    
    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    uint8_t myNumber;

    uint16_t myDataByteCount;

    uint16_t myCrc;

    Frame* myFrame;

    //--------------------------------------------------------------------------
    // Private methods implemented from Frame
    //--------------------------------------------------------------------------

    bool frameToByteArray(ByteArray& byteArray) const;

    bool frameParseByteArray(const ByteArray& data);
};

}; // namespace Plat4m

#endif // PLAT4M_PACKET_H
