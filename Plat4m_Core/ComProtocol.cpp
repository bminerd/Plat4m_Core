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
// Copyright (c) 2013 Benjamin Minerd
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
/// @file ComProtocol.cpp
/// @author Ben Minerd
/// @date 4/22/2013
/// @brief ComProtocol class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/ComProtocol.h>
#include <Plat4m_Core/System.h>

using Plat4m::ComProtocol;
using Plat4m::ComLink;

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
uint32_t ComProtocol::getParseTimeoutMs()
{
    return myParseTimeoutMs;
}

//------------------------------------------------------------------------------
ComProtocol::ParseStatus ComProtocol::parseData(
                                              const ByteArray& receiveByteArray,
								   	   	   	  ByteArray& transmitByteArray,
						                      Callback<>*& followUpCallback)
{
	return driverParseData(receiveByteArray,
	                       transmitByteArray,
	                       followUpCallback);
}

//------------------------------------------------------------------------------
// Protected constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ComProtocol::ComProtocol(const uint32_t parseTimeoutMs, ComLink& comLink) :
    myParseTimeoutMs(parseTimeoutMs),
    myComLink(comLink)
{
    myComLink.addComProtocol(*this);
}

//------------------------------------------------------------------------------
// Protected virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ComProtocol::~ComProtocol()
{
}

//------------------------------------------------------------------------------
// Protected methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ComLink& ComProtocol::getComLink()
{
    return myComLink;
}
