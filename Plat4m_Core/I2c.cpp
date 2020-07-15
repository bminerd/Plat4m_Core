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
// Copyright (c) 2012 Benjamin Minerd
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
/// @file I2c.cpp
/// @author Ben Minerd
/// @date 2/25/2012
/// @brief I2C class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/I2c.h>
#include <Plat4m_Core/System.h>

using Plat4m::I2c;
using Plat4m::Buffer;

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
I2c::Error I2c::setConfig(const Config& config)
{
    Error error = driverSetConfig(config);

    if (error.getCode() == ERROR_CODE_NONE)
    {
        myConfig = config;
    }

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
I2c::Error I2c::masterTransfer(const TransferMode transferMode,
                               const uint8_t address,
                               const ByteArray& transmitByteArray,
                               ByteArray& receiveByteArray,
                               MasterSlaveBus::Mailbox* mailbox)
{
    if (!isEnabled())
    {
        return Error(ERROR_CODE_NOT_ENABLED);
    }

    Transfer transfer;
    transfer.transferMode  = transferMode;
    transfer.address       = address;
    transfer.transmitCount = transmitByteArray.getSize();
    transfer.receiveCount  = receiveByteArray.getMaxSize();
    transfer.transmitBuffer.setItems(transmitByteArray.getItems(),
                                     transmitByteArray.getSize(),
                                     true);
    transfer.receiveBuffer.setItems(receiveByteArray.getItems(),
                                    receiveByteArray.getMaxSize());
    transfer.mailbox = mailbox;
    transfer.error.setCode(ERROR_CODE_NONE);

    if (isValidPointer(mailbox))
    {
        mailbox->receiveByteArray = &receiveByteArray;
    }

    Error error = driverMasterTransfer(transfer);

    if (isValidPointer(mailbox) && (error.getCode() == ERROR_CODE_NONE))
    {
        receiveByteArray.setSize(transfer.receiveCount);
    }

    return (transfer.error);
}

//------------------------------------------------------------------------------
// Protected constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
I2c::I2c() :
    Module(),
    myConfig(),
    myTransferBuffer()
{
}

//------------------------------------------------------------------------------
// Protected virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
I2c::~I2c()
{
}

//------------------------------------------------------------------------------
// Protected methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
I2c::Config I2c::getConfig()
{
	return myConfig;
}

//------------------------------------------------------------------------------
Buffer<I2c::Transfer>& I2c::getTransferBuffer()
{
    return myTransferBuffer;
}
