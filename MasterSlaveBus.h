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
/// @file MasterSlaveBus.h
/// @author Ben Minerd
/// @date 7/5/2016
/// @brief MasterSlaveBus class header file.
///

#ifndef PLAT4M_MASTER_SLAVE_BUS_H
#define PLAT4M_MASTER_SLAVE_BUS_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <stdint.h>

#include <Plat4m_Core/Module.h>
#include <Plat4m_Core/ErrorTemplate.h>
#include <Plat4m_Core/ByteArray.h>
#include <Plat4m_Core/Buffer.h>
#include <Plat4m_Core/Callback.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class MasterSlaveBus : public Module
{
public:

    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------

    enum ErrorCode
    {
        ERROR_CODE_NONE,
        ERROR_CODE_PARAMETER_INVALID,
        ERROR_CODE_ADDRESS_INVALID,
        ERROR_CODE_NOT_ENABLED,
        ERROR_CODE_TX_BUFFER_FULL,
        ERROR_CODE_TIMEOUT,
        ERROR_CODE_BUS,
        ERROR_CODE_BUS_BUSY,
        ERROR_CODE_TRANSFER_BUFFER_FULL
    };

    enum TransferMode
    {
        TRANSFER_MODE_TRANSMIT_WAIT,
        TRANSFER_MODE_TRANSMIT_MAILBOX,
        TRANSFER_MODE_RECEIVE_WAIT,
        TRANSFER_MODE_RECEIVE_MAILBOX,
        TRANSFER_MODE_TRANSMIT_RECEIVE_WAIT,
        TRANSFER_MODE_TRANSMIT_RECEIVE_MAILBOX
    };

    typedef ErrorTemplate<ErrorCode> Error;

    typedef Callback<> MailboxFullCallback;

    struct Mailbox
    {
        ByteArray* receiveByteArray;
        Error error;
        MailboxFullCallback* mailboxFullCallback;
    };

	struct Transfer
	{
		TransferMode transferMode;
		uint32_t txCount;
		uint32_t rxCount;
		Buffer<uint8_t>* txBuffer;
		Buffer<uint8_t>* rxBuffer;
		Mailbox* mailbox;
		Error error;
	};

    //--------------------------------------------------------------------------
    // Public virtual methods
    //--------------------------------------------------------------------------

    virtual Error masterTransfer(Transfer& transfer);

protected:

    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------

    MasterSlaveBus();

    //--------------------------------------------------------------------------
    // Protected virtual destructors
    //--------------------------------------------------------------------------

    virtual ~MasterSlaveBus();

    //--------------------------------------------------------------------------
    // Protected methods
    //--------------------------------------------------------------------------

    Buffer<Transfer>* getTransferBuffer();

private:

    //--------------------------------------------------------------------------
    // Private pure virtual methods
    //--------------------------------------------------------------------------

    virtual Error driverMasterTransfer(Transfer& transfer) = 0;
};

}; // namespace Plat4m

#endif // PLAT4M_MASTER_SLAVE_BUS_H
