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
/// @file ComInterface.cpp
/// @author Ben Minerd
/// @date 4/22/13
/// @brief ComInterface class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <ComInterface.h>
#include <System.h>

using Plat4m::ComInterface;
using Plat4m::Buffer;

//------------------------------------------------------------------------------
// Public virtual methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ComInterface::setTransmitBuffer(Buffer<uint8_t>& transmitBuffer)
{
    myTransmitBuffer = &transmitBuffer;
}

//------------------------------------------------------------------------------
void ComInterface::setReceiveBuffer(Buffer<uint8_t>& receiveBuffer)
{
    myReceiveBuffer = &receiveBuffer;
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ComInterface::setByteReceivedCallback(
                                   	 ByteReceivedCallback& byteReceivedCallback)
{
    myByteReceivedCallback = &byteReceivedCallback;
}

//------------------------------------------------------------------------------
// Protected constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ComInterface::ComInterface() :
    Module(),
    myTransmitBuffer(0),
    myReceiveBuffer(0),
    myByteReceivedCallback(0)
{
}

//------------------------------------------------------------------------------
ComInterface::ComInterface(Buffer<uint8_t>& transmitBuffer,
                           Buffer<uint8_t>& receiveBuffer) :
    Module(),
    myTransmitBuffer(&transmitBuffer),
    myReceiveBuffer(&receiveBuffer),
    myByteReceivedCallback(0)
{
}

//------------------------------------------------------------------------------
// Protected virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ComInterface::~ComInterface()
{
}

//------------------------------------------------------------------------------
// Protected methods
//------------------------------------------------------------------------------

////------------------------------------------------------------------------------
//Buffer<uint8_t>* ComInterface::getTransmitBuffer()
//{
//    return myTransmitBuffer;
//}
//
////------------------------------------------------------------------------------
//Buffer<uint8_t>* ComInterface::getReceiveBuffer()
//{
//    return myReceiveBuffer;
//}
//
////------------------------------------------------------------------------------
//void ComInterface::bytesReceived()
//{
//    if (isValidPointer(myBytesReceivedCallback))
//    {
//        myBytesReceivedCallback->call();
//    }
//}
