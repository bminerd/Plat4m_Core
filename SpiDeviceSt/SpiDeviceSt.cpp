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
/// @file SpiDeviceSt.cpp
/// @author Ben Minerd
/// @date 4/4/2016
/// @brief SpiDeviceSt class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/SpiDeviceSt/SpiDeviceSt.h>
#include <Plat4m_Core/Plat4m.h>

using Plat4m::SpiDeviceSt;
using Plat4m::SpiDevice;
using Plat4m::SlaveDevice;
using Plat4m::Module;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
SpiDeviceSt::SpiDeviceSt(Plat4m::Spi& spi) :
    SlaveDevice(),
	mySpiDevice(spi)
{
}

//------------------------------------------------------------------------------
SpiDeviceSt::SpiDeviceSt(Plat4m::Spi& spi, Plat4m::GpioPin& chipSelectGpioPin) :
    SlaveDevice(),
    mySpiDevice(spi, chipSelectGpioPin)
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
SpiDeviceSt::~SpiDeviceSt()
{
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error SpiDeviceSt::driverSetEnabled(const bool enabled)
{
    mySpiDevice.setEnabled(enabled);

    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods implemented from SlaveDevice
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
SlaveDevice::Error SpiDeviceSt::driverTransmit(const ByteArray& byteArray,
                                               const bool waitUntilDone,
                                               const int32_t mailboxIndex)
{
    return (mySpiDevice.transmit(byteArray, waitUntilDone, mailboxIndex));
}

//------------------------------------------------------------------------------
SlaveDevice::Error SpiDeviceSt::driverReceive(ByteArray& byteArray,
                                              const int32_t mailboxIndex)
{
	setBits(byteArray[0], (uint8_t) 0x80);

	return mySpiDevice.receive(byteArray, mailboxIndex);
}

//------------------------------------------------------------------------------
SlaveDevice::Error SpiDeviceSt::driverTransmitReceive(
                                             const ByteArray& transmitByteArray,
                                             ByteArray& receiveByteArray,
                                             const int32_t mailboxIndex)
{
	setBits(transmitByteArray[0], (uint8_t) 0x80);

	return mySpiDevice.transmitReceive(transmitByteArray,
	                                   receiveByteArray,
	                                   mailboxIndex);
}
