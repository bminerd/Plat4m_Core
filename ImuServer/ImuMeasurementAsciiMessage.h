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
/// @file ImuMeasurementAsciiMessage.h
/// @author Ben Minerd
/// @date 4/25/16
/// @brief ImuMeasurementAsciiMessage class header file.
///

#ifndef IMU_MEASUREMENT_ASCII_MESSAGE_H
#define IMU_MEASUREMENT_ASCII_MESSAGE_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <AsciiMessage.h>
#include <ImuMeasurementMessage.h>
#include <ByteArray.h>
#include <Array.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class ImuMeasurementAsciiMessage : public AsciiMessage
{
public:

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------
    
    ImuMeasurementAsciiMessage(ImuMeasurementMessage& message);

    //--------------------------------------------------------------------------
    // Public destructors
    //--------------------------------------------------------------------------

    virtual ~ImuMeasurementAsciiMessage();

    //--------------------------------------------------------------------------
    // Public virtual methods implemented from AsciiMessage
    //--------------------------------------------------------------------------

    virtual void stringParametersUpdated();

    virtual void messageParametersUpdated();

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    static const ByteArray myName;
    static ByteArray myParameterNameStrings[];
    static const Array<ByteArray> myParameterNames;
    
    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    ImuMeasurementMessage& myMessage;
};

}; // namespace Plat4m

#endif // IMU_MEASUREMENT_ASCII_MESSAGE_H
