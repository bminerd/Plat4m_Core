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
/// @file I2c.h
/// @author Ben Minerd
/// @date 3/25/2013
/// @brief I2c class header file.
///

#ifndef PLAT4M_I2C_H
#define PLAT4M_I2C_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Module.h>
#include <ErrorTemplate.h>
#include <ByteArray.h>
#include <Buffer.h>
#include <BufferN.h>
#include <Callback.h>
#include <MasterSlaveBus.h>

#include <stdint.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class I2c : public Module
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

    enum MasterMode
    {
        MASTER_MODE_TX = 0,
        MASTER_MODE_RX = 1
    };

    enum AddressBits
    {
        ADDRESS_BITS_7 = 0,
        ADDRESS_BITS_10
    };

    enum TransferMode
    {
        TRANSFER_MODE_TRANSMIT,
        TRANSFER_MODE_TRANSMIT_WAIT,
        TRANSFER_MODE_TRANSMIT_MAILBOX,
        TRANSFER_MODE_RECEIVE_WAIT,
        TRANSFER_MODE_RECEIVE_MAILBOX,
        TRANSFER_MODE_TRANSMIT_RECEIVE_WAIT,
        TRANSFER_MODE_TRANSMIT_RECEIVE_MAILBOX
    };

    typedef ErrorTemplate<ErrorCode> Error;

    struct Config
    {
        uint32_t clockSpeedHz;
        AddressBits addressBits;
    };

	struct Transfer
	{
		TransferMode transferMode;
		uint8_t address;
		uint32_t transmitCount;
		uint32_t receiveCount;
		Buffer<uint8_t> transmitBuffer;
		Buffer<uint8_t> receiveBuffer;
		MasterSlaveBus::Mailbox* mailbox;
		Error error;
	};

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    Error setConfig(const Config& config);

    Error masterTransfer(const TransferMode transferMode,
                         const uint8_t address,
                         const ByteArray& transmitByteArray,
                         ByteArray& receiveByteArray,
                         MasterSlaveBus::Mailbox* mailbox = 0);

protected:

    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------

    I2c();

    //--------------------------------------------------------------------------
    // Protected virtual destructors
    //--------------------------------------------------------------------------

    virtual ~I2c();

    //--------------------------------------------------------------------------
    // Protected methods
    //--------------------------------------------------------------------------

    Config getConfig();

    Buffer<Transfer>& getTransferBuffer();

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    Config myConfig;

    BufferN<Transfer, 10> myTransferBuffer;

    //--------------------------------------------------------------------------
    // Private pure virtual methods
    //--------------------------------------------------------------------------

    virtual Error driverSetConfig(const Config& config) = 0;

    virtual Error driverMasterTransfer(Transfer& transfer) = 0;
};

}; // namespace Plat4m

#endif // PLAT4M_I2C_H
