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
// Copyright (c) 2013-2023 Benjamin Minerd
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
/// @file Spi.cpp
/// @author Ben Minerd
/// @date 4/4/13
/// @brief Spi class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Spi.h>
#include <Plat4m_Core/System.h>

using Plat4m::Spi;
using Plat4m::Module;

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Spi::Error Spi::setConfig(const Config& config)
{
    if (!isEnabled())
    {
        return Error(ERROR_CODE_NOT_ENABLED);
    }

    Error error = driverSetConfig(config);
    
    if (error.getCode() == ERROR_CODE_NONE)
    {
        myConfig = config;
    }

    return error;
}

//------------------------------------------------------------------------------
Spi::Config Spi::getConfig() const
{
    return myConfig;
}

//------------------------------------------------------------------------------
Spi::Error Spi::masterTransfer(const TransferMode transferMode,
                               GpioPin* chipSelectGpioPin,
                               const ByteArray& transmitByteArray,
                               ByteArray& receiveByteArray,
                               MasterSlaveBus::Mailbox* mailbox)
{
    if (!isEnabled())
    {
        return Error(ERROR_CODE_NOT_ENABLED);
    }

    Transfer transfer;
    transfer.transferMode      = transferMode;
    transfer.chipSelectGpioPin = chipSelectGpioPin;
    transfer.transmitCount     = transmitByteArray.getSize();
    transfer.receiveCount      = receiveByteArray.getMaxSize();
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
Spi::Spi(const TransmissionMode transmissionMode) :
    Module(),
    myTransmissionMode(transmissionMode),
    myConfig(),
    myTransferBuffer()
{
}

//------------------------------------------------------------------------------
// Protected virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Spi::~Spi()
{
}
