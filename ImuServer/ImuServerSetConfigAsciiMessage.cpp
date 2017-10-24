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
/// @file ImuServerSetConfigAsciiMessage.cpp
/// @author Ben Minerd
/// @date 6/8/16
/// @brief ImuServerSetConfigAsciiMessage class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <ImuServerSetConfigAsciiMessage.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using Plat4m::ImuServerSetConfigAsciiMessage;
using Plat4m::ByteArray;
using Plat4m::Array;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const ByteArray ImuServerSetConfigAsciiMessage::myName("IMU_SERVER_SET_CONFIG");

ByteArray ImuServerSetConfigAsciiMessage::myParameterNameStrings[] =
{
    ByteArray("OUTPUT_RATE_HZ")
};

const Array<ByteArray> ImuServerSetConfigAsciiMessage::myParameterNames(
                                            myParameterNameStrings,
                                            ARRAY_SIZE(myParameterNameStrings));

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ImuServerSetConfigAsciiMessage::ImuServerSetConfigAsciiMessage(
                                           ImuServerSetConfigMessage& message) :
    AsciiMessage(myName, &myParameterNames),
    myMessage(message)
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ImuServerSetConfigAsciiMessage::~ImuServerSetConfigAsciiMessage()
{
}

//------------------------------------------------------------------------------
// Public virtual methods implemented from AsciiMessage
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ImuServerSetConfigAsciiMessage::stringParametersUpdated()
{
    myMessage.outputRateDivisionFactor =
        getUint32Parameter(
        	  ImuServerSetConfigMessage::PARAMETER_OUTPUT_RATE_DIVISION_FACTOR);
}

//------------------------------------------------------------------------------
void ImuServerSetConfigAsciiMessage::messageParametersUpdated()
{
    setUint32Parameter(
    		   ImuServerSetConfigMessage::PARAMETER_OUTPUT_RATE_DIVISION_FACTOR,
               myMessage.outputRateDivisionFactor);
}
