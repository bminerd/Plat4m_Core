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
/// @file Spi.h
/// @author Ben Minerd
/// @date 4/4/2013
/// @brief Spi class header file.
///

#ifndef PLAT4M_SPI_H
#define PLAT4M_SPI_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m.h>
#include <Module.h>
#include <ErrorTemplate.h>
#include <ByteArray.h>
#include <BufferN.h>
#include <GpioPin.h>
#include <MasterSlaveBus.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class Spi : public Module
{
public:
    
    //--------------------------------------------------------------------------
    // Public enumerations
    //--------------------------------------------------------------------------
    
    enum ErrorCode
    {
        ERROR_CODE_NONE,
        ERROR_CODE_PARAMETER_INVALID,
        ERROR_CODE_NOT_ENABLED,
        ERROR_CODE_TX_BUFFER_FULL,
        ERROR_CODE_TIMEOUT,
		ERROR_CODE_CLOCK_FREQUENCY_INVALID
    };

    enum Mode
    {
        MODE_MASTER = 0,
        MODE_SLAVE
    };

    enum DataBits
    {
        DATA_BITS_8 = 0,
        DATA_BITS_16
    };

    enum ClockPolarity
    {
        CLOCK_POLARITY_LOW = 0,
        CLOCK_POLARITY_HIGH
    };

    enum ClockPhase
    {
        CLOCK_PHASE_LEADING_EDGE = 0,
        CLOCK_PHASE_TRAILING_EDGE
    };

    enum BitOrder
    {
        BIT_ORDER_LSB_FIRST = 0,
        BIT_ORDER_MSB_FIRST
    };

    enum TransmissionMode
    {
        TRANSMISSION_MODE_RX_TX,
        TRANSMISSION_MODE_RX,
        TRANSMISSION_MODE_TX
    };
    
    enum TransferMode
    {
        TRANSFER_MODE_TRANSMIT,
        TRANSFER_MODE_TRANSMIT_WAIT,
        TRANSFER_MODE_TRANSMIT_MAILBOX,
        TRANSFER_MODE_RECEIVE_WAIT,
        TRANSFER_MODE_RECEIVE_MAILBOX,
        TRANSFER_MODE_TRANSMIT_RECEIVE_SEQUENTIAL,
        TRANSFER_MODE_TRANSMIT_RECEIVE_SEQUENTIAL_MAILBOX,
        TRANSFER_MODE_TRANSMIT_RECEIVE_CONCURRENT,
        TRANSFER_MODE_TRANSMIT_RECEIVE_CONCURRENT_MAILBOX
    };
    
    //--------------------------------------------------------------------------
    // Public typedefs
    //--------------------------------------------------------------------------

    typedef ErrorTemplate<ErrorCode> Error;

    //--------------------------------------------------------------------------
    // Public structures
    //--------------------------------------------------------------------------
    
    struct Config
    {
    	uint32_t clockFrequencyHz;
        Mode mode;
        DataBits dataBits;
        ClockPolarity clockPolarity;
        ClockPhase clockPhase;
        BitOrder bitOrder;
    };
    
	struct Transfer
	{
		TransferMode transferMode;
		GpioPin* chipSelectGpioPin;
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
    
    Config getConfig() const;

    Error masterTransfer(const TransferMode transferMode,
                         GpioPin* chipSelectGpioPin,
                         const ByteArray& transmitByteArray,
                         ByteArray& receiveByteArray,
                         MasterSlaveBus::Mailbox* mailbox = 0);

protected:
    
    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------

    Spi(const TransmissionMode transmissionMode);
    
    //--------------------------------------------------------------------------
    // Protected virtual destructors
    //--------------------------------------------------------------------------

    virtual ~Spi();

private:
    
    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    const TransmissionMode myTransmissionMode;

    Config myConfig;
    
    BufferN<Transfer, 10> myTransferBuffer;
    
    Transfer myLastTransfer;

    //--------------------------------------------------------------------------
    // Private pure virtual methods
    //--------------------------------------------------------------------------
    
    virtual Error driverSetConfig(const Config& config) = 0;

    virtual Error driverMasterTransfer(Transfer& transfer) = 0;
};

}; // namespace Plat4m

#endif // PLAT4M_SPI_H
