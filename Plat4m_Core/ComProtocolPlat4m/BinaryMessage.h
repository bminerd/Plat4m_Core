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
/// @file BinaryMessage.h
/// @author Ben Minerd
/// @date 4/25/2016
/// @brief BinaryMessage class header file.
///

#ifndef PLAT4M_BINARY_MESSAGE_H
#define PLAT4M_BINARY_MESSAGE_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <stdint.h>

#include <Plat4m_Core/ComProtocolPlat4m/Frame.h>
#include <Plat4m_Core/ByteArray.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class BinaryMessage : public Frame
{
public:

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    BinaryMessage(const uint16_t groupId, const uint16_t messageId);

    BinaryMessage(const uint16_t groupId,
                  const uint16_t messageId,
                  ByteArray& data);

    //--------------------------------------------------------------------------
    // Public destructors
    //--------------------------------------------------------------------------

    virtual ~BinaryMessage();

    //--------------------------------------------------------------------------
    // Public virtual methods
    //--------------------------------------------------------------------------

    virtual bool parseMessageData(const ByteArray& data);

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

	// TODO: This is a temporary fix to allow tying a ByteArray to a
	// BinaryMessage for dedicated memory reasons
    using Frame::setData;

    uint16_t getGroupId() const;

    uint16_t getMessageId() const;

    const ByteArray& getData() const;

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    static const uint8_t myFrameIdentifier;
    
    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    uint16_t myGroupId;

    uint16_t myMessageId;

    ByteArray myData;

    //--------------------------------------------------------------------------
    // Private methods implemented from Frame
    //--------------------------------------------------------------------------

    bool frameToByteArray(ByteArray& byteArray) const;

    bool frameParseByteArray(const ByteArray& data);

    //--------------------------------------------------------------------------
    // Private virtual methods
    //-------------------------------------------------------------------------

    virtual bool binaryMessageToByteArray(ByteArray& byteArray) const;

    virtual bool binaryMessageParseByteArray(const ByteArray& byteArray);
};

}; // namespace Plat4m

#endif // PLAT4M_BINARY_MESSAGE_H
