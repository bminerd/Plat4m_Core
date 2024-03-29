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
/// @file ImuMeasurementBinaryMessage.h
/// @author Ben Minerd
/// @date 5/24/16
/// @brief ImuMeasurementBinaryMessage class header file.
///

#ifndef PLAT4M_IMU_MEASUREMENT_BINARY_MESSAGE_H
#define PLAT4M_IMU_MEASUREMENT_BINARY_MESSAGE_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <stdint.h>

#include <Plat4m_Core/ComProtocolPlat4m/BinaryMessage.h>
#include <Plat4m_Core/ImuServer/ImuMeasurementMessage.h>
#include <Plat4m_Core/ByteArray.h>
#include <Plat4m_Core/Array.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class ImuMeasurementBinaryMessage : public BinaryMessage
{
public:

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------
    
    ImuMeasurementBinaryMessage(ImuMeasurementMessage& message);

    //--------------------------------------------------------------------------
    // Public destructors
    //--------------------------------------------------------------------------

    virtual ~ImuMeasurementBinaryMessage();

    //--------------------------------------------------------------------------
    // Public methods implemented from BinaryMessage
    //--------------------------------------------------------------------------

    bool parseMessageData(const ByteArray& data);

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    ImuMeasurementMessage& myMessage;
    
    //--------------------------------------------------------------------------
    // Private methods implemented from BinaryMessage
    //--------------------------------------------------------------------------

    bool binaryMessageToByteArray(ByteArray& byteArray) const;
};

}; // namespace Plat4m

#endif // PLAT4M_IMU_MEASUREMENT_BINARY_MESSAGE_H
