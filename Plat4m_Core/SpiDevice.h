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
/// @file SpiDevice.h
/// @author Ben Minerd
/// @date 4/4/2013
/// @brief SpiDevice class header file.
///

#ifndef PLAT4M_SPI_DEVICE_H
#define PLAT4M_SPI_DEVICE_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <stdint.h>

#include <Plat4m_Core/SlaveDevice.h>
#include <Plat4m_Core/Spi.h>
#include <Plat4m_Core/GpioPin.h>
#include <Plat4m_Core/ByteArray.h>
#include <Plat4m_Core/Module.h>
#include <Plat4m_Core/MasterSlaveBus.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class SpiDevice : public SlaveDevice
{
public:
    
    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    SpiDevice(Spi& spi);

    SpiDevice(Spi& spi, GpioPin& chipSelectGpioPin);
    
    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~SpiDevice();

private:
    
    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    Spi& mySpi;
    
    GpioPin* myChipSelectGpioPin;

    //--------------------------------------------------------------------------
    // Private methods implemented from Module
    //--------------------------------------------------------------------------

    Module::Error driverSetEnabled(const bool enabled);
    
    //--------------------------------------------------------------------------
    // Private methods implemented from SlaveDevice
    //--------------------------------------------------------------------------
    
    Error driverTransmit(const ByteArray& byteArray,
                         const bool waitUntilDone,
                         const int32_t mailboxIndex);
    
    Error driverReceive(ByteArray& byteArray, const int32_t mailboxIndex);
    
    Error driverTransmitReceive(const ByteArray& transmitByteArray,
                                ByteArray& receiveByteArray,
                                const int32_t mailboxIndex);
};

}; // namespace Plat4m

#endif // PLAT4M_SPI_DEVICE_H
