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
/// @file SpiDeviceSt.h
/// @author Ben Minerd
/// @date 4/4/13
/// @brief SpiDeviceSt class header file.
///

#ifndef SPI_DEVICE_ST_H
#define SPI_DEVICE_ST_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <SlaveDevice.h>
#include <SpiDevice.h>
#include <Spi.h>
#include <GpioPin.h>
#include <ByteArray.h>

#include <stdint.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class SpiDeviceSt : public SlaveDevice
{
public:
    
    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------
    
    SpiDeviceSt(Spi& spi);

    SpiDeviceSt(Spi& spi, GpioPin& chipSelectGpioPin);
    
    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~SpiDeviceSt();

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    SpiDevice mySpiDevice;

    //--------------------------------------------------------------------------
    // Private methods implemented from Module
    //--------------------------------------------------------------------------
    
    Module::Error driverSetEnabled(const bool enabled);

    //--------------------------------------------------------------------------
    // Private methods implemented from SlaveDevice
    //--------------------------------------------------------------------------

    SlaveDevice::Error driverTransmit(const ByteArray& byteArray,
                                      const bool waitUntilDone,
                                      const int32_t mailboxIndex);
    
    SlaveDevice::Error driverReceive(ByteArray& byteArray,
                                     const int32_t mailboxIndex);
    
    SlaveDevice::Error driverTransmitReceive(const ByteArray& transmitByteArray,
                                             ByteArray& receiveByteArray,
                                             const int32_t mailboxIndex);
};

}; // namespace Plat4m

#endif // SPI_DEVICE_ST_H
