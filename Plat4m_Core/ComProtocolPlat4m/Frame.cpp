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
/// @file Frame.cpp
/// @author Ben Minerd
/// @date 3/23/2016
/// @brief Frame class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/ComProtocolPlat4m/Frame.h>

using Plat4m::Frame;
using Plat4m::ByteArray;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Frame::Frame() :
    myIdentifier(0),
    myData()
{
}


//------------------------------------------------------------------------------
Frame::Frame(const uint8_t identifier) :
    myIdentifier(identifier),
    myData()
{
}

//------------------------------------------------------------------------------
Frame::Frame(const uint8_t identifier, ByteArray& data) :
    myIdentifier(identifier),
    myData(data)
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Frame::~Frame()
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
uint8_t Frame::getIdentifier() const
{
    return myIdentifier;
}

//------------------------------------------------------------------------------
ByteArray& Frame::getData()
{
    return myData;
}

//------------------------------------------------------------------------------
const ByteArray& Frame::getData() const
{
    return myData;
}

//------------------------------------------------------------------------------
bool Frame::toByteArray(ByteArray& byteArray) const
{
    return (byteArray.append(myIdentifier) & frameToByteArray(byteArray));
}

//------------------------------------------------------------------------------
bool Frame::parseByteArray(const ByteArray& data)
{
    if (data.getSize() < 1)
    {
        return false;
    }

    return frameParseByteArray(data.subArray(1));
}

//------------------------------------------------------------------------------
// Protected methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void Frame::setIdentifier(const uint8_t identifier)
{
    myIdentifier = identifier;
}

//------------------------------------------------------------------------------
void Frame::setData(ByteArray& byteArray)
{
    myData = byteArray;
}

//------------------------------------------------------------------------------
// Private virtual methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool Frame::frameToByteArray(ByteArray& byteArray) const
{
    // Intentionally blank, not implemented by subclass
    return true;
}

//------------------------------------------------------------------------------
bool Frame::frameParseByteArray(const ByteArray& data)
{
    // Intentionally blank, not implemented by subclass
    return true;
}
