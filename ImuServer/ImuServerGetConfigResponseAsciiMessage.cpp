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
/// @file ImuServerGetConfigResponseAsciiMessage.cpp
/// @author Ben Minerd
/// @date 6/30/16
/// @brief ImuServerGetConfigResponseAsciiMessage class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/ImuServer/ImuServerGetConfigResponseAsciiMessage.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using Plat4m::ImuServerGetConfigResponseAsciiMessage;
using Plat4m::ByteArray;
using Plat4m::Array;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const ByteArray ImuServerGetConfigResponseAsciiMessage::myName(
                                              "IMU_SERVER_GET_CONFIG_RESPONSE");

ByteArray ImuServerGetConfigResponseAsciiMessage::myParameterNameStrings[] =
{
    ByteArray("OUTPUT_RATE_HZ")
};

const Array<ByteArray> ImuServerGetConfigResponseAsciiMessage::myParameterNames(
                                            myParameterNameStrings,
                                            ARRAY_SIZE(myParameterNameStrings));

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ImuServerGetConfigResponseAsciiMessage::ImuServerGetConfigResponseAsciiMessage(
                                         ImuServerGetConfigResponseMessage& message) :
    AsciiMessage(myName, &myParameterNames),
    myMessage(message)
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ImuServerGetConfigResponseAsciiMessage::~ImuServerGetConfigResponseAsciiMessage()
{
}

//------------------------------------------------------------------------------
// Public virtual methods implemented from AsciiMessage
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ImuServerGetConfigResponseAsciiMessage::stringParametersUpdated()
{
	myMessage.index =
		  getUint8Parameter(ImuServerGetConfigResponseMessage::PARAMETER_INDEX);
    myMessage.outputRateDivisionFactor =
        getUint32Parameter(
			ImuServerGetConfigResponseMessage::
										 PARAMETER_OUTPUT_RATE_DIVISION_FACTOR);
}

//------------------------------------------------------------------------------
void ImuServerGetConfigResponseAsciiMessage::messageParametersUpdated()
{
    setUint8Parameter(ImuServerGetConfigResponseMessage::PARAMETER_INDEX,
    				  myMessage.index);
    setUint32Parameter(
		ImuServerGetConfigResponseMessage::
										  PARAMETER_OUTPUT_RATE_DIVISION_FACTOR,
		myMessage.outputRateDivisionFactor);
}
