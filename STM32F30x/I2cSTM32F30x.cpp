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
/// @file I2cSTM32F30x.cpp
/// @author Ben Minerd
/// @date 2/4/2016
/// @brief I2cSTM32F30x class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <stm32f30x_rcc.h>
#include <stm32f30x_misc.h>

#include <Plat4m_Core/STM32F30x/I2cSTM32F30x.h>
#include <Plat4m_Core/System.h>
#include <Plat4m_Core/CallbackMethod.h>
#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/Interrupt.h>

using Plat4m::I2cSTM32F30x;
using Plat4m::I2c;
using Plat4m::GpioPinSTM32F30x;
using Plat4m::InterruptSTM32F30x;
using Plat4m::Module;

//------------------------------------------------------------------------------
// Local variables
//------------------------------------------------------------------------------

static InterruptSTM32F30x* interruptObjectMap[3][2];

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const uint32_t I2cSTM32F30x::myClockMap[] =
{
    RCC_APB1Periph_I2C1, /// ID_1
    RCC_APB1Periph_I2C2, /// ID_2
    RCC_APB1Periph_I2C3  /// ID_3
};

const InterruptSTM32F30x::Id I2cSTM32F30x::myInterruptMap[][2] =
{
    /// I2cSTM32F30x::ID_1
    {
        InterruptSTM32F30x::ID_I2C_1_EV,
        InterruptSTM32F30x::ID_I2C_1_ER
    },
    /// I2cSTM32F30x::ID_2
    {
        InterruptSTM32F30x::ID_I2C_2_EV,
        InterruptSTM32F30x::ID_I2C_2_ER
    },
    /// I2cSTM32F30x::ID_3
    {
        InterruptSTM32F30x::ID_I2C_3_EV,
        InterruptSTM32F30x::ID_I2C_3_ER
    }
};

const uint16_t I2cSTM32F30x::myAddressBitsMap[] =
{
    I2C_AcknowledgedAddress_7bit, /// I2c::ADDRESS_BITS_7
    I2C_AcknowledgedAddress_10bit /// I2c::ADDRESS_BITS_10
};

I2C_TypeDef* I2cSTM32F30x::myI2cMap[] =
{
    I2C1, /// ID_1
    I2C2, /// ID_2
    I2C3  /// ID_3
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
I2cSTM32F30x::I2cSTM32F30x(const Id id,
                           GpioPinSTM32F30x& sclGpioPin,
                           GpioPinSTM32F30x& sdaGpioPin,
                           const uint8_t ownAddress) :
    I2c(),
    myId(id),
    mySclGpioPin(sclGpioPin),
    mySdaGpioPin(sdaGpioPin),
    myOwnAddress(ownAddress),
    myI2c(myI2cMap[id]),
    myEventInterrupt(
                    myInterruptMap[myId][0],
                    createCallback(this, &I2cSTM32F30x::eventInterruptHandler)),
    myErrorInterrupt(
                    myInterruptMap[myId][1],
                    createCallback(this, &I2cSTM32F30x::errorInterruptHandler)),
    myState(STATE_IDLE),
    myLastIsrValue(0),
    myCurrentTransfer(0)
{
    // If this entry in the table is null, the peripheral hasn't been
    // initialized yet
    if (isNullPointer(interruptObjectMap[myId][0]))
    {
        interruptObjectMap[myId][0] = &myEventInterrupt;
        interruptObjectMap[myId][1] = &myErrorInterrupt;
    }
    else
    {
        // Instantiating same peripheral twice, lockup the system
        while (true)
        {
        }
    }
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GpioPinSTM32F30x& I2cSTM32F30x::getSclGpioPin()
{
	return mySclGpioPin;
}

//------------------------------------------------------------------------------
GpioPinSTM32F30x& I2cSTM32F30x::getSdaGpioPin()
{
	return mySdaGpioPin;
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error I2cSTM32F30x::driverSetEnabled(const bool enabled)
{
    mySclGpioPin.setEnabled(enabled);
    mySdaGpioPin.setEnabled(enabled);

    RCC_APB1PeriphClockCmd(myClockMap[myId], (FunctionalState) enabled);

    if (enabled)
    {
        // Generic GpioPin configuration
        GpioPin::Config gpioPinConfig;
        gpioPinConfig.mode     = GpioPin::MODE_ALTERNATE_FUNCTION;
        gpioPinConfig.resistor = GpioPin::RESISTOR_PULL_UP;

        mySclGpioPin.configure(gpioPinConfig);
        mySdaGpioPin.configure(gpioPinConfig);

        softwareReset();
    }

    I2C_Cmd(myI2c, (FunctionalState) enabled);

    setInterruptEnabled(INTERRUPT_ERROR, enabled);
    setInterruptEnabled(INTERRUPT_NACK, enabled);

    myEventInterrupt.setEnabled(enabled);
    myErrorInterrupt.setEnabled(enabled);

    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods implemented from I2c
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
I2c::Error I2cSTM32F30x::driverSetConfig(const Config& config)
{
    // TODO: Copied value from ST example for now
    uint32_t timingValue = 0x40110405;

    I2C_InitTypeDef i2cInit;
    i2cInit.I2C_Timing              = timingValue;
    i2cInit.I2C_AnalogFilter        = I2C_AnalogFilter_Enable;
    i2cInit.I2C_DigitalFilter       = 0x00;
    i2cInit.I2C_Mode                = I2C_Mode_I2C;
    i2cInit.I2C_OwnAddress1         = myOwnAddress;
    i2cInit.I2C_Ack                 = I2C_Ack_Enable;
    i2cInit.I2C_AcknowledgedAddress = myAddressBitsMap[config.addressBits];

    I2C_Init(myI2c, &i2cInit);

    Plat4m::Interrupt::Config interruptConfig;
    interruptConfig.priority = 7;
    myEventInterrupt.configure(interruptConfig);
    myErrorInterrupt.configure(interruptConfig);

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
I2c::Error I2cSTM32F30x::driverMasterTransfer(Transfer& transfer)
{
    setInterruptEnabled(INTERRUPT_STOP, false);

    if (!(getTransferBuffer().write(transfer)))
    {
        // TODO: Check this
        setInterruptEnabled(INTERRUPT_STOP, true);

        return Error(ERROR_CODE_TRANSFER_BUFFER_FULL);
    }

    setInterruptEnabled(INTERRUPT_STOP, true);

    if (myState == STATE_IDLE)
    {
        myState = STATE_BUSY;
        getTransferBuffer().peek(myCurrentTransfer);
        setupMasterTransfer();
        masterStart();
    }
    else if (myState == STATE_ERROR)
    {
        reset();
    }

	if (isValidPointer(transfer.mailbox))
	{
		// Return immediately and let the I2C peripheral continue concurrently
		return Error(ERROR_CODE_NONE);
	}

	while (myState == STATE_BUSY)
	{
	}

	if (myCurrentTransfer->error.getCode() != ERROR_CODE_NONE)
    {
        reset();
	}

	myCurrentTransfer = 0;
	getTransferBuffer().emptyRead();

	return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private inline methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void I2cSTM32F30x::masterSetSlaveAddress(const uint8_t slaveAddress)
{
    clearBits(myI2c->CR2, I2C_CR2_SADD);
    setBits(myI2c->CR2, ((uint32_t) slaveAddress) << 1);
}

//------------------------------------------------------------------------------
void I2cSTM32F30x::masterSetTransferDirection(
                                      const TransferDirection transferDirection)
{
    if (transferDirection == TRANSFER_DIRECTION_WRITE)
    {
        clearBits(myI2c->CR2, I2C_CR2_RD_WRN);
    }
    else // transferDirection == TRANSFER_DIRECTION_READ
    {
        setBits(myI2c->CR2, I2C_CR2_RD_WRN);
    }
}

//------------------------------------------------------------------------------
void I2cSTM32F30x::masterSetTransferByteCount(const uint8_t byteCount)
{
    clearBits(myI2c->CR2, I2C_CR2_NBYTES);
    setBits(myI2c->CR2, ((uint32_t) byteCount) << 16);
}

//------------------------------------------------------------------------------
void I2cSTM32F30x::masterSetAutoEndEnabled(const bool enabled)
{
    setBitsSet(myI2c->CR2, I2C_CR2_AUTOEND, enabled);
}

//------------------------------------------------------------------------------
void I2cSTM32F30x::masterSetReloadEnabled(const bool enabled)
{
    setBitsSet(myI2c->CR2, I2C_CR2_RELOAD, enabled);
}

//------------------------------------------------------------------------------
void I2cSTM32F30x::masterStart()
{
    setBits(myI2c->CR2, I2C_CR2_START);
}

//------------------------------------------------------------------------------
void I2cSTM32F30x::masterStop()
{
    setBits(myI2c->CR2, I2C_CR2_STOP);
}

//------------------------------------------------------------------------------
void I2cSTM32F30x::writeByte(const uint8_t data)
{
    myI2c->TXDR = data;
}

//------------------------------------------------------------------------------
uint8_t I2cSTM32F30x::readByte()
{
    return ((uint8_t) (myI2c->RXDR));
}

//------------------------------------------------------------------------------
void I2cSTM32F30x::setInterruptEnabled(const Interrupt interrupt,
                                       const bool enable)
{
	setBitsSet(myI2c->CR1, interrupt, enable);
}

//------------------------------------------------------------------------------
void I2cSTM32F30x::clearStopInterrupt()
{
    setBits(myI2c->ICR, I2C_ICR_STOPCF);
}

//------------------------------------------------------------------------------
bool I2cSTM32F30x::isBusy()
{
    return areBitsSet(myI2c->ISR, I2C_ISR_BUSY);
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void I2cSTM32F30x::softwareReset()
{
    I2C_Cmd(myI2c, DISABLE);
    I2C_Cmd(myI2c, ENABLE);
}

//------------------------------------------------------------------------------
void I2cSTM32F30x::reset()
{
    softwareReset();
    driverSetEnabled(true);
    driverSetConfig(getConfig());
    getTransferBuffer().clear();
    myState = STATE_IDLE;
}

//------------------------------------------------------------------------------
void I2cSTM32F30x::setupMasterTransfer()
{
    masterSetSlaveAddress(myCurrentTransfer->address);

    if (myCurrentTransfer->transmitCount > 0)
    {
        masterSetTransferDirection(TRANSFER_DIRECTION_WRITE);
        setInterruptEnabled(INTERRUPT_TRANSMIT_BUFFER_EMPTY, true);

        if (myCurrentTransfer->transmitCount <= 255)
        {
            masterSetTransferByteCount(myCurrentTransfer->transmitCount);

            if (isValidPointer(myCurrentTransfer->mailbox))
            {
                masterSetAutoEndEnabled(false);
            }
            else
            {
                masterSetAutoEndEnabled(true);
                setInterruptEnabled(INTERRUPT_TRANSFER_COMPLETE, true);
            }

            masterSetReloadEnabled(false);
        }
        else
        {
            masterSetTransferByteCount(255);
            masterSetAutoEndEnabled(false);
            masterSetReloadEnabled(true);
            setInterruptEnabled(INTERRUPT_TRANSFER_COMPLETE, true);
        }

        if (myCurrentTransfer->receiveCount > 0)
        {
            // Setup the peripheral for a transmit then receive transfer
            // (using a restart)

            masterSetAutoEndEnabled(false);
            setInterruptEnabled(INTERRUPT_TRANSFER_COMPLETE, true);
        }
    }
    else if (myCurrentTransfer->receiveCount > 0)
    {
        masterSetTransferDirection(TRANSFER_DIRECTION_READ);
        setInterruptEnabled(INTERRUPT_RECEIVE_BUFFER_NOT_EMPTY, true);

        if (myCurrentTransfer->receiveCount <= 255)
        {
            masterSetTransferByteCount(myCurrentTransfer->receiveCount);

            if (isValidPointer(myCurrentTransfer->mailbox))
            {
                masterSetAutoEndEnabled(false);
            }
            else
            {
                masterSetAutoEndEnabled(true);
                setInterruptEnabled(INTERRUPT_TRANSFER_COMPLETE, true);
            }

            masterSetReloadEnabled(false);
        }
        else
        {
            masterSetTransferByteCount(255);
            masterSetAutoEndEnabled(false);
            masterSetReloadEnabled(true);
            setInterruptEnabled(INTERRUPT_TRANSFER_COMPLETE, true);
        }
    }

    setInterruptEnabled(INTERRUPT_STOP, true);
}

//------------------------------------------------------------------------------
void I2cSTM32F30x::eventInterruptHandler()
{
    uint32_t transmitBufferCount = myCurrentTransfer->transmitBuffer.count();
    uint32_t receiveBufferCount = myCurrentTransfer->receiveBuffer.count();
    uint8_t byte = 0;

    const uint32_t isrValue = myI2c->ISR;

    switch (isrValue)
    {
        case EVENT_TRANSMIT_BUFFER_EMPTY:
        {
            myCurrentTransfer->transmitBuffer.read(byte);
            writeByte(byte);

            break;
        }
        case EVENT_RECEIVE_BUFFER_NOT_EMPTY_STOP: // Fall-through
        case EVENT_RECEIVE_BUFFER_NOT_EMPTY:
        {
            byte = readByte();
            myCurrentTransfer->receiveBuffer.write(byte);

            break;
        }
        case EVENT_STOP:
        {
            clearStopInterrupt();

            setInterruptEnabled(INTERRUPT_TRANSMIT_BUFFER_EMPTY, false);
            setInterruptEnabled(INTERRUPT_RECEIVE_BUFFER_NOT_EMPTY,  false);
            setInterruptEnabled(INTERRUPT_STOP,                  false);

            if (isValidPointer(myCurrentTransfer->mailbox))
            {
                myCurrentTransfer->mailbox->receiveByteArray->setSize(
                                               myCurrentTransfer->receiveCount);
                myCurrentTransfer->mailbox->error.setCode(
                                          (MasterSlaveBus::ErrorCode)
                                          (myCurrentTransfer->error.getCode()));
                myCurrentTransfer->mailbox->mailboxFullCallback->call();
            }

            if (getTransferBuffer().count() > 1)
            {
                getTransferBuffer().emptyRead();
                getTransferBuffer().peek(myCurrentTransfer);
                setupMasterTransfer();
                masterStart();
            }
            else
            {
                myState = STATE_IDLE;

                if (isValidPointer(myCurrentTransfer->mailbox))
                {
                    myCurrentTransfer = 0;
                    getTransferBuffer().emptyRead();
                }
            }

            break;
        }
        case EVENT_WRITE_TRANSFER_COMPLETE_RELOAD:
        {
            if (transmitBufferCount > 0)
            {
                if (transmitBufferCount <= 255)
                {
                    masterSetTransferByteCount(transmitBufferCount);
                    masterSetReloadEnabled(false);
                }
                else
                {
                    masterSetTransferByteCount(255);
                    masterSetReloadEnabled(true);
                }
            }
            else if (myCurrentTransfer->receiveCount != receiveBufferCount)
            {
                int remainingRxBytes =
                           myCurrentTransfer->receiveCount - receiveBufferCount;

                if (remainingRxBytes <= 255)
                {
                    masterSetTransferByteCount(remainingRxBytes);
                    masterSetReloadEnabled(false);
                }
                else
                {
                    masterSetTransferByteCount(255);
                    masterSetReloadEnabled(true);
                }
            }

            break;
        }
        case EVENT_READ_WRITE_TRANSFER_COMPLETE:
        {
            setInterruptEnabled(INTERRUPT_TRANSMIT_BUFFER_EMPTY, false);

            if ((myCurrentTransfer->receiveCount > 0) &&
                (receiveBufferCount == 0)             &&
                (myCurrentTransfer->transmitCount > 0))
            {
                // Restart condition
                masterSetTransferDirection(TRANSFER_DIRECTION_READ);
                setInterruptEnabled(INTERRUPT_RECEIVE_BUFFER_NOT_EMPTY, true);

                int remainingRxBytes =
                           myCurrentTransfer->receiveCount - receiveBufferCount;

                if (remainingRxBytes <= 255)
                {
                    masterSetTransferByteCount(remainingRxBytes);
                    masterStart();

                    if (isValidPointer(myCurrentTransfer->mailbox))
                    {
                        masterSetAutoEndEnabled(false);
                        setInterruptEnabled(INTERRUPT_TRANSFER_COMPLETE, true);
                    }
                    else
                    {
                        masterSetAutoEndEnabled(true);
                        setInterruptEnabled(INTERRUPT_TRANSFER_COMPLETE, false);
                    }
                }
                else
                {
                    masterSetTransferByteCount(255);
                    masterStart();
                }
            }
            else
            {
                // Should only reach here if mailbox is being used, otherwise
                // auto-end is enabled
                setInterruptEnabled(INTERRUPT_RECEIVE_BUFFER_NOT_EMPTY, false);
                setInterruptEnabled(INTERRUPT_TRANSFER_COMPLETE, false);
                masterStop();
            }

            break;
        }
        case EVENT_READ_TRANSFER_COMPLETE_RELOAD:
        {
            int remainingRxBytes =
                           myCurrentTransfer->receiveCount - receiveBufferCount;

            if (remainingRxBytes <= 255)
            {
                masterSetTransferByteCount(remainingRxBytes);

                if (isValidPointer(myCurrentTransfer->mailbox))
                {
                    masterSetAutoEndEnabled(false);
                }
                else
                {
                    masterSetAutoEndEnabled(true);
                }

                masterSetReloadEnabled(false);
            }
            else
            {
                masterSetTransferByteCount(255);
                masterSetReloadEnabled(true);
            }

            break;
        }
        case EVENT_READ_TRANSFER_COMPLETE:
        {
            // Should only reach here if mailbox is being used, otherwise
            // auto-end is enabled

            byte = readByte();
            myCurrentTransfer->receiveBuffer.write(byte);

            setInterruptEnabled(INTERRUPT_RECEIVE_BUFFER_NOT_EMPTY, false);
            setInterruptEnabled(INTERRUPT_TRANSFER_COMPLETE, false);
            masterStop();

            break;
        }
        default:
        {
            // Check errors

            setInterruptEnabled(INTERRUPT_TRANSMIT_BUFFER_EMPTY, false);
            setInterruptEnabled(INTERRUPT_RECEIVE_BUFFER_NOT_EMPTY, false);
            setInterruptEnabled(INTERRUPT_STOP, false);
            setInterruptEnabled(INTERRUPT_TRANSFER_COMPLETE, false);
            setInterruptEnabled(INTERRUPT_ERROR, false);
            setInterruptEnabled(INTERRUPT_NACK, false);

            myCurrentTransfer->error.setCode(ERROR_CODE_BUS);

            myState = STATE_ERROR;

            break;
        }
    }

    myLastIsrValue = isrValue;
}

//------------------------------------------------------------------------------
void I2cSTM32F30x::errorInterruptHandler()
{
    setInterruptEnabled(INTERRUPT_TRANSMIT_BUFFER_EMPTY, false);
    setInterruptEnabled(INTERRUPT_RECEIVE_BUFFER_NOT_EMPTY, false);
    setInterruptEnabled(INTERRUPT_STOP, false);
    setInterruptEnabled(INTERRUPT_TRANSFER_COMPLETE, false);
    setInterruptEnabled(INTERRUPT_ERROR, false);
    setInterruptEnabled(INTERRUPT_NACK, false);

    myCurrentTransfer->error.setCode(ERROR_CODE_BUS);

    myState = STATE_ERROR;
}

//------------------------------------------------------------------------------
// Interrupt service routines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
extern "C" void I2C1_EV_IRQHandler(void)
{
    interruptObjectMap[I2cSTM32F30x::ID_1][0]->handler();
}

//------------------------------------------------------------------------------
extern "C" void I2C1_ER_IRQHandler(void)
{
    interruptObjectMap[I2cSTM32F30x::ID_1][1]->handler();
}

//------------------------------------------------------------------------------
extern "C" void I2C2_EV_IRQHandler(void)
{
    interruptObjectMap[I2cSTM32F30x::ID_2][0]->handler();
}

//------------------------------------------------------------------------------
extern "C" void I2C2_ER_IRQHandler(void)
{
    interruptObjectMap[I2cSTM32F30x::ID_2][1]->handler();
}

//------------------------------------------------------------------------------
extern "C" void I2C3_EV_IRQHandler(void)
{
    interruptObjectMap[I2cSTM32F30x::ID_3][0]->handler();
}

//------------------------------------------------------------------------------
extern "C" void I2C3_ER_IRQHandler(void)
{
    interruptObjectMap[I2cSTM32F30x::ID_3][1]->handler();
}
