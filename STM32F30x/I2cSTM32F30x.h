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
/// @file I2cSTM32F30x.h
/// @author Ben Minerd
/// @date 2/4/2016
/// @brief I2cSTM32F30x class header file.
///

#ifndef PLAT4M_I2C_STM32F30X_H
#define PLAT4M_I2C_STM32F30X_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m.h>
#include <I2c.h>
#include <GpioPinSTM32F30x.h>
#include <InterruptSTM32F30x.h>
#include <Module.h>

#include <stm32f30x.h>
#include <stm32f30x_i2c.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class I2cSTM32F30x : public I2c
{
public:
    
    //--------------------------------------------------------------------------
    // Public enumerations
    //--------------------------------------------------------------------------

    enum Id
    {
        ID_1 = 0,
        ID_2,
        ID_3
    };

	enum Interrupt
	{
	    INTERRUPT_ADDRESS                  = I2C_CR1_ADDRIE,
		INTERRUPT_TRANSMIT_BUFFER_EMPTY    = I2C_CR1_TXIE,
		INTERRUPT_RECEIVE_BUFFER_NOT_EMPTY = I2C_CR1_RXIE,
		INTERRUPT_TRANSFER_COMPLETE        = I2C_CR1_TCIE,
		INTERRUPT_STOP                     = I2C_CR1_STOPIE,
		INTERRUPT_NACK                     = I2C_CR1_NACKIE,
		INTERRUPT_ERROR                    = I2C_CR1_ERRIE
	};

	enum Event
	{
        EVENT_ADDRESS_MATCHED                = I2C_ISR_BUSY | I2C_ISR_ADDR,

	    EVENT_TRANSMIT_BUFFER_EMPTY          = I2C_ISR_BUSY |
                                               I2C_ISR_TXE  |
                                               I2C_ISR_TXIS,

	    EVENT_RECEIVE_BUFFER_NOT_EMPTY       = I2C_ISR_BUSY |
                                               I2C_ISR_TXE  |
                                               I2C_ISR_RXNE,

        EVENT_RECEIVE_BUFFER_NOT_EMPTY_STOP  = I2C_ISR_TXE  |
                                               I2C_ISR_RXNE |
                                               I2C_ISR_STOPF,

        EVENT_STOP                           = I2C_ISR_TXE | I2C_ISR_STOPF,

	    EVENT_WRITE_TRANSFER_COMPLETE_RELOAD = I2C_ISR_BUSY |
                                               I2C_ISR_TXE  |
                                               I2C_ISR_TCR,

	    EVENT_READ_WRITE_TRANSFER_COMPLETE   = I2C_ISR_BUSY |
                                               I2C_ISR_TXE  |
                                               I2C_ISR_TC,

        EVENT_READ_TRANSFER_COMPLETE_RELOAD  = I2C_ISR_BUSY |
                                               I2C_ISR_TXE  |
                                               I2C_ISR_RXNE |
                                               I2C_ISR_TCR,

        EVENT_READ_TRANSFER_COMPLETE         = I2C_ISR_BUSY |
                                               I2C_ISR_TXE  |
                                               I2C_ISR_RXNE |
                                               I2C_ISR_TC,

	    EVENT_NACK_RECEPTION           = I2C_ISR_NACKF,
	    EVENT_BUS_ERROR                = I2C_ISR_BERR,
	    EVENT_ARBITRATION_LOSS         = I2C_ISR_ARLO,
	    EVENT_OVERRUN_UNDERRUN         = I2C_ISR_OVR,
	    EVENT_PEC_ERROR                = I2C_ISR_PECERR,
	    EVENT_TIMEOUT_ERROR            = I2C_ISR_TIMEOUT,
	    EVENT_SMBUS_ALERT              = I2C_ISR_ALERT
	};

	enum TransferDirection
	{
	    TRANSFER_DIRECTION_WRITE = 0,
	    TRANSFER_DIRECTION_READ
	};

	enum State
	{
		STATE_IDLE,
		STATE_BUSY,
		STATE_ERROR
	};

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    I2cSTM32F30x(const Id id,
                 GpioPinSTM32F30x& sclGpioPin,
                 GpioPinSTM32F30x& sdaGpioPin,
                 const uint8_t ownAddress = 0x01);

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    // Constants

    static const uint32_t myClockMap[];

    static const InterruptSTM32F30x::Id myInterruptMap[][2];

    static const uint16_t myAddressBitsMap[];

    // Variables

    static I2C_TypeDef* myI2cMap[];

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    const Id myId;

    GpioPinSTM32F30x& mySclGpioPin;

    GpioPinSTM32F30x& mySdaGpioPin;

    const uint8_t myOwnAddress;

    I2C_TypeDef* myI2c;

    InterruptSTM32F30x myEventInterrupt;

    InterruptSTM32F30x myErrorInterrupt;

    volatile State myState;

    volatile uint32_t myLastIsrValue;

    Transfer* myCurrentTransfer;

    //--------------------------------------------------------------------------
    // Private methods implemented from Module
    //--------------------------------------------------------------------------

    Module::Error driverSetEnabled(const bool enabled);

    //--------------------------------------------------------------------------
    // Private methods implemented from I2c
    //--------------------------------------------------------------------------

    I2c::Error driverSetConfig(const Config& config);

    I2c::Error driverMasterTransfer(Transfer& transfer);

    //--------------------------------------------------------------------------
    // Private inline methods
    //--------------------------------------------------------------------------

    inline void masterSetSlaveAddress(const uint8_t slaveAddress);

    inline void masterSetTransferDirection(
                                     const TransferDirection transferDirection);

    inline void masterSetTransferByteCount(const uint8_t byteCount);

    inline void masterSetAutoEndEnabled(const bool enabled);

    inline void masterSetReloadEnabled(const bool enabled);

    inline void masterStart();

    inline void masterStop();

    inline void writeByte(const uint8_t byte);

    inline uint8_t readByte();

    inline void setInterruptEnabled(const Interrupt interrupt,
                                    const bool enabled);

    inline void clearStopInterrupt();

    inline bool isBusy();

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------

    void softwareReset();

    void reset();

    void setupMasterTransfer();

    void eventInterruptHandler();

    void errorInterruptHandler();
};

}; // namespace Plat4m

#endif // PLAT4M_I2C_STM32F30X_H
