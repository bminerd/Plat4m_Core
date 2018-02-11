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
/// @file ComInterfaceDevice.cpp
/// @author Ben Minerd
/// @date 4/12/2017
/// @brief ComInterfaceDevice class.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/ComInterfaceDevice.h>
#include <Plat4m_Core/System.h>

using Plat4m::ComInterface;
using Plat4m::ComInterfaceDevice;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ComInterfaceDevice::ComInterfaceDevice(Buffer<uint8_t>& transmitBuffer,
                                       Buffer<uint8_t>& receiveBuffer) :
    Module(),
    myComInterface(0),
    myTransmitBuffer(transmitBuffer),
    myReceiveBuffer(receiveBuffer)
{
}

//------------------------------------------------------------------------------
ComInterfaceDevice::ComInterfaceDevice(Buffer<uint8_t>& transmitBuffer,
                                       Buffer<uint8_t>& receiveBuffer,
                                       ComInterface& comInterface) :
    Module(),
    myComInterface(&comInterface),
    myTransmitBuffer(transmitBuffer),
    myReceiveBuffer(receiveBuffer)
{
    myComInterface->setTransmitBuffer(myTransmitBuffer);
    myComInterface->setReceiveBuffer(myReceiveBuffer);
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ComInterfaceDevice::~ComInterfaceDevice()
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ComInterfaceDevice::setComInterface(ComInterface& comInterface)
{
    myComInterface = &comInterface;
    myComInterface->setTransmitBuffer(myTransmitBuffer);
    myComInterface->setReceiveBuffer(myReceiveBuffer);
}

//------------------------------------------------------------------------------
ComInterfaceDevice::Error ComInterfaceDevice::transmitBytes(
                                                     const ByteArray& byteArray,
                                                     const bool waitUntilDone)
{
    Error error = driverTransmitBytes(byteArray, waitUntilDone);

    return error;
}

//------------------------------------------------------------------------------
ComInterfaceDevice::Error ComInterfaceDevice::receiveBytes(
                                                         ByteArray& byteArray,
                                                         const TimeMs timeoutMs)
{
    Error error = driverReceiveBytes(byteArray, timeoutMs);

    return error;
}

//------------------------------------------------------------------------------
uint32_t ComInterfaceDevice::getReceivedBytesCount()
{
    return (myComInterface->getReceivedBytesCount());
}

//------------------------------------------------------------------------------
ComInterfaceDevice::Error ComInterfaceDevice::getReceivedBytes(
                                                          ByteArray& byteArray,
                                                          const uint32_t nBytes)
{
    myComInterface->getReceivedBytes(byteArray, nBytes);

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
ComInterfaceDevice::Error ComInterfaceDevice::transmitReceiveBytes(
                                             const ByteArray& transmitByteArray,
                                             ByteArray& receiveByteArray,
                                             const uint32_t timeoutMs)
{
    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Protected methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ComInterface* ComInterfaceDevice::getComInterface()
{
    return myComInterface;
}

//------------------------------------------------------------------------------
// Private virtual methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ComInterfaceDevice::Error ComInterfaceDevice::driverTransmitBytes(
                                                     const ByteArray& byteArray,
                                                     const bool waitUntilDone)
{
    myComInterface->transmitBytes(byteArray, waitUntilDone);

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
ComInterfaceDevice::Error ComInterfaceDevice::driverReceiveBytes(
                                                         ByteArray& byteArray,
                                                         const TimeMs timeoutMs)
{
    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
ComInterfaceDevice::Error ComInterfaceDevice::driverTransmitReceiveBytes(
                                             const ByteArray& transmitByteArray,
                                             ByteArray& receiveByteArray,
                                             const TimeMs timeoutMs)
{
    return Error(ERROR_CODE_NONE);
}
