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
/// @file I2cDevice.cpp
/// @author Ben Minerd
/// @date 3/26/13
/// @brief I2cDevice class.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <I2cDevice.h>

using Plat4m::I2cDevice;
using Plat4m::SlaveDevice;
using Plat4m::Module;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
I2cDevice::I2cDevice(const uint8_t address, I2c& i2c) :
    SlaveDevice(),
    myAddress(address),
    myI2c(i2c)
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
I2cDevice::~I2cDevice()
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
uint8_t I2cDevice::getAddress() const
{
    return myAddress;
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error I2cDevice::driverSetEnabled(const bool enabled)
{
    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods implemented from SlaveDevice
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
SlaveDevice::Error I2cDevice::driverTransmit(const ByteArray& byteArray,
                                             const bool waitUntilDone,
                                             const int32_t mailboxIndex)
{
    ByteArray emptyByteArray;
    I2c::Error error;

    if (mailboxIndex == -1)
    {
        if (waitUntilDone)
        {
            error = myI2c.masterTransfer(I2c::TRANSFER_MODE_TRANSMIT_WAIT,
                                         myAddress,
                                         byteArray,
                                         emptyByteArray);
        }
        else
        {
            error = myI2c.masterTransfer(I2c::TRANSFER_MODE_TRANSMIT,
                                         myAddress,
                                         byteArray,
                                         emptyByteArray);
        }
    }
    else
    {
        error = myI2c.masterTransfer(
                                   I2c::TRANSFER_MODE_TRANSMIT_MAILBOX,
                                   myAddress,
                                   byteArray,
                                   emptyByteArray,
                                   &(getMailboxArray()->getItem(mailboxIndex)));
    }

    if (error.getCode() != I2c::ERROR_CODE_NONE)
    {
       return Error(ERROR_CODE_COMMUNICATION);
    }

   return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
SlaveDevice::Error I2cDevice::driverReceive(ByteArray& byteArray,
                                            const int32_t mailboxIndex)
{
    ByteArray emptyByteArray;

    I2c::Error error;

    if (mailboxIndex == -1)
    {
       error = myI2c.masterTransfer(I2c::TRANSFER_MODE_RECEIVE_WAIT,
                                    myAddress,
                                    emptyByteArray,
                                    byteArray);
    }
    else
    {
       error = myI2c.masterTransfer(
                                   I2c::TRANSFER_MODE_RECEIVE_MAILBOX,
                                   myAddress,
                                   byteArray,
                                   emptyByteArray,
                                   &(getMailboxArray()->getItem(mailboxIndex)));
    }

    if (error.getCode() != I2c::ERROR_CODE_NONE)
    {
       return Error(ERROR_CODE_COMMUNICATION);
    }

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
SlaveDevice::Error I2cDevice::driverTransmitReceive(
                                             const ByteArray& transmitByteArray,
                                             ByteArray& receiveByteArray,
                                             const int32_t mailboxIndex)
{
    I2c::Error error;

    if (mailboxIndex == -1)
    {
        error = myI2c.masterTransfer(I2c::TRANSFER_MODE_TRANSMIT_RECEIVE_WAIT,
                                     myAddress,
                                     transmitByteArray,
                                     receiveByteArray);
    }
    else
    {
        error = myI2c.masterTransfer(
                                   I2c::TRANSFER_MODE_TRANSMIT_RECEIVE_MAILBOX,
                                   myAddress,
                                   transmitByteArray,
                                   receiveByteArray,
                                   &(getMailboxArray()->getItem(mailboxIndex)));
    }

    if (error.getCode() != I2c::ERROR_CODE_NONE)
    {
        return Error(ERROR_CODE_COMMUNICATION);
    }

    return Error(ERROR_CODE_NONE);
}
