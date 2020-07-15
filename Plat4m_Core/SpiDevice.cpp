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
/// @brief SpiDevice class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/SpiDevice.h>
#include <Plat4m_Core/Plat4m.h>

using Plat4m::SpiDevice;
using Plat4m::SlaveDevice;
using Plat4m::Module;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
SpiDevice::SpiDevice(Spi& spi) :
    SlaveDevice(),
    mySpi(spi),
    myChipSelectGpioPin(0)
{
}

//------------------------------------------------------------------------------
SpiDevice::SpiDevice(Spi& spi, GpioPin& chipSelectGpioPin) :
    SlaveDevice(),
    mySpi(spi),
    myChipSelectGpioPin(&chipSelectGpioPin)
{
}

//------------------------------------------------------------------------------
// Public destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
SpiDevice::~SpiDevice()
{
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error SpiDevice::driverSetEnabled(const bool enabled)
{
    if (isValidPointer(myChipSelectGpioPin))
    {
        myChipSelectGpioPin->setEnabled(enabled);
    
        if (enabled)
        {
            GpioPin::Config gpioConfig;
            gpioConfig.mode     = GpioPin::MODE_DIGITAL_OUTPUT_PUSH_PULL;
            gpioConfig.resistor = GpioPin::RESISTOR_PULL_UP;
            
            myChipSelectGpioPin->configure(gpioConfig);
            myChipSelectGpioPin->setLevel(GpioPin::LEVEL_HIGH);
        }
    }
    
    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods implemented from SlaveDevice
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
SlaveDevice::Error SpiDevice::driverTransmit(const ByteArray& byteArray,
                                             const bool waitUntilDone,
                                             const int32_t mailboxIndex)
{
    ByteArray emptyByteArray;

    Spi::TransferMode transferMode;

    if (waitUntilDone)
    {
        transferMode = Spi::TRANSFER_MODE_TRANSMIT_WAIT;
    }
    else
    {
        transferMode = Spi::TRANSFER_MODE_TRANSMIT;
    }

    Spi::Error error = mySpi.masterTransfer(transferMode,
                                            myChipSelectGpioPin,
                                            byteArray,
                                            emptyByteArray);

    if (error.getCode() != Spi::ERROR_CODE_NONE)
    {
        return SlaveDevice::Error(SlaveDevice::ERROR_CODE_COMMUNICATION);
    }
    
    return SlaveDevice::Error(SlaveDevice::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
SlaveDevice::Error SpiDevice::driverReceive(ByteArray& byteArray,
                                            const int32_t mailboxIndex)
{
    ByteArray emptyByteArray;

    Spi::Error error = mySpi.masterTransfer(Spi::TRANSFER_MODE_RECEIVE_WAIT,
                                            myChipSelectGpioPin,
                                            emptyByteArray,
                                            byteArray);

    if (error.getCode() != Spi::ERROR_CODE_NONE)
    {
        return Error(SlaveDevice::ERROR_CODE_COMMUNICATION);
    }

    return Error(SlaveDevice::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
SlaveDevice::Error SpiDevice::driverTransmitReceive(
                                             const ByteArray& transmitByteArray,
                                             ByteArray& receiveByteArray,
                                             const int32_t mailboxIndex)
{
    Spi::Error error = mySpi.masterTransfer(
                                 Spi::TRANSFER_MODE_TRANSMIT_RECEIVE_SEQUENTIAL,
                                 myChipSelectGpioPin,
                                 transmitByteArray,
                                 receiveByteArray);

    if (error.getCode() != Spi::ERROR_CODE_NONE)
    {
        return SlaveDevice::Error(SlaveDevice::ERROR_CODE_COMMUNICATION);
    }
    
    return Error(ERROR_CODE_NONE);
}
