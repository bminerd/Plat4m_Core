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
/// @file ImuSetConfigResponseAsciiMessage.cpp
/// @author Ben Minerd
/// @date 6/7/2016
/// @brief ImuSetConfigResponseAsciiMessage class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <Plat4m_Core/ImuServer/ImuSetConfigResponseAsciiMessage.h>

using Plat4m::ImuSetConfigResponseAsciiMessage;
using Plat4m::ByteArray;
using Plat4m::Array;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const ByteArray ImuSetConfigResponseAsciiMessage::myName(
                                                     "IMU_SET_CONFIG_RESPONSE");

ByteArray ImuSetConfigResponseAsciiMessage::myParameterNameStrings[] =
{
    ByteArray("ERROR")
};

const Array<ByteArray> ImuSetConfigResponseAsciiMessage::myParameterNames(
                                            myParameterNameStrings,
                                            ARRAY_SIZE(myParameterNameStrings));

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ImuSetConfigResponseAsciiMessage::ImuSetConfigResponseAsciiMessage(
                                         ImuSetConfigResponseMessage& message) :
    AsciiMessage(myName, &myParameterNames),
    myMessage(message)
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ImuSetConfigResponseAsciiMessage::~ImuSetConfigResponseAsciiMessage()
{
}

//------------------------------------------------------------------------------
// Public virtual methods implemented from AsciiMessage
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ImuSetConfigResponseAsciiMessage::stringParametersUpdated()
{
    const Array<ByteArray>* parameterValues = getParameterValues();
    const char* string = 0;

    // Error
    string = (const char*) (parameterValues->getItem(0).getItems());
    myMessage.error = strtoul(string, 0, 0);
}

//------------------------------------------------------------------------------
void ImuSetConfigResponseAsciiMessage::messageParametersUpdated()
{
    setUint8Parameter(ImuSetConfigResponseMessage::PARAMETER_ERROR,
                      myMessage.error);
}
