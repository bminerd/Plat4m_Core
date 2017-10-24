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
/// @file I2cSTM32F10x.cpp
/// @author Ben Minerd
/// @date 1/6/2016
/// @brief I2cSTM32F10x class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <I2cSTM32F10x.h>
#include <System.h>
#include <CallbackMethod.h>

using Plat4m::I2cSTM32F10x;
using Plat4m::I2c;
using Plat4m::GpioPinSTM32F10x;
using Plat4m::Module;
using Plat4m::ProcessorSTM32F10x;
using Plat4m::InterruptSTM32F10x;

//------------------------------------------------------------------------------
// Local variables
//------------------------------------------------------------------------------

static InterruptSTM32F10x* interruptObjectMap[2][2];

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

// Constants

const ProcessorSTM32F10x::Peripheral I2cSTM32F10x::myPeripheralMap[] =
{
    ProcessorSTM32F10x::PERIPHERAL_I2C_1, /// ID_1
    ProcessorSTM32F10x::PERIPHERAL_I2C_2  /// ID_2
};

const InterruptSTM32F10x::Id I2cSTM32F10x::myInterruptIdMap[][2] =
{
    /// ID_1
    {InterruptSTM32F10x::ID_I2C_1_EV, InterruptSTM32F10x::ID_I2C_1_ER},
    /// ID_2
    {InterruptSTM32F10x::ID_I2C_2_EV, InterruptSTM32F10x::ID_I2C_2_ER}
};

const uint16_t I2cSTM32F10x::myInterruptMap[] =
{
    I2C_IT_EVT, /// INTERRUPT_EVENT
    I2C_IT_BUF, /// INTERRUPT_BUFFER
    I2C_IT_ERR  /// INTERRUPT_ERROR
};


const uint16_t I2cSTM32F10x::myAddressBitsMap[] =
{
    I2C_AcknowledgedAddress_7bit, /// I2c::ADDRESS_BITS_7
    I2C_AcknowledgedAddress_10bit /// I2c::ADDRESS_BITS_10
};

const GpioPinSTM32F10x::OutputSpeed I2cSTM32F10x::myDefaultOutputSpeed =
                                           GpioPinSTM32F10x::OUTPUT_SPEED_50MHZ;

const uint8_t I2cSTM32F10x::myOwnAddress = 0xA0;

// Variables

I2C_TypeDef* I2cSTM32F10x::myI2cMap[] =
{
    I2C1, /// ID_1
    I2C2  /// ID_2
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
I2cSTM32F10x::I2cSTM32F10x(const Id id,
                           GpioPinSTM32F10x& sclGpioPin,
                           GpioPinSTM32F10x& sdaGpioPin) :
    I2c(),
    myId(id),
    myI2c(myI2cMap[myId]),
    mySclGpioPin(sclGpioPin),
    mySdaGpioPin(sdaGpioPin),
    myState(STATE_IDLE),
    myTransfer(0),
    myEventInterrupt(
                    myInterruptIdMap[myId][0],
                    createCallback(this, &I2cSTM32F10x::eventInterruptHandler)),
    myErrorInterrupt(myInterruptIdMap[myId][1],
                     createCallback(this, &I2cSTM32F10x::errorInterruptHandler))
{
    if (isValidPointer(interruptObjectMap[myId][0]))
    {
        // Trying to instantiate peripheral twice, lockup
        while (true)
        {
        }
    }

    interruptObjectMap[myId][0] = &myEventInterrupt;
    interruptObjectMap[myId][1] = &myErrorInterrupt;
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
I2cSTM32F10x::~I2cSTM32F10x()
{
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error I2cSTM32F10x::driverSetEnabled(const bool enabled)
{
    mySclGpioPin.setEnabled(enabled);
    mySdaGpioPin.setEnabled(enabled);

    ProcessorSTM32F10x::setPeripheralClockEnabled(myPeripheralMap[myId],
                                                  enabled);

    if (enabled)
    {
        // Generic GpioPin configuration
        GpioPin::Config gpioConfig;
        gpioConfig.mode     = GpioPin::MODE_ALTERNATE_FUNCTION;
        gpioConfig.resistor = GpioPin::RESISTOR_PULL_UP;

        mySclGpioPin.configure(gpioConfig);
        mySdaGpioPin.configure(gpioConfig);

        // Specific GpioPinSTM32F10x configuration
        GpioPinSTM32F10x::STM32F10xConfig gpioDriverConfig;
        gpioDriverConfig.outputType  = GpioPinSTM32F10x::OUTPUT_TYPE_OPEN_DRAIN;
        gpioDriverConfig.outputSpeed = myDefaultOutputSpeed;

        mySclGpioPin.setSTM32F10xConfig(gpioDriverConfig);
        mySdaGpioPin.setSTM32F10xConfig(gpioDriverConfig);

        softwareReset();
    }

    I2C_Cmd(myI2c, (FunctionalState) enabled);

    myEventInterrupt.setEnabled(enabled);
    myErrorInterrupt.setEnabled(enabled);

    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods implemented from I2c
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
I2c::Error I2cSTM32F10x::driverSetConfig(const Config& config)
{
    I2C_InitTypeDef i2cInit;
    i2cInit.I2C_ClockSpeed          = config.clockSpeedHz;
    i2cInit.I2C_Mode                = I2C_Mode_I2C;
    i2cInit.I2C_DutyCycle           = I2C_DutyCycle_2;
    i2cInit.I2C_OwnAddress1         = myOwnAddress;
    i2cInit.I2C_Ack                 = I2C_Ack_Enable;
    i2cInit.I2C_AcknowledgedAddress = myAddressBitsMap[config.addressBits];

    I2C_Init(myI2c, &i2cInit);

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
I2c::Error I2cSTM32F10x::driverMasterTransfer(Transfer& transfer)
{
	myTransfer = &transfer;

	masterStart();

	myState = STATE_BUSY;

	interruptEnable(INTERRUPT_ERROR, true);
	interruptEnable(INTERRUPT_EVENT, true);

	if ((transfer.transferMode == TRANSFER_MODE_TRANSMIT_MAILBOX) ||
        (transfer.transferMode == TRANSFER_MODE_TRANSMIT_RECEIVE_MAILBOX))
	{
		// Return immediately and let the I2C interface continue asynchronously
		return Error(ERROR_CODE_NONE);
	}

	while (myState == STATE_BUSY)
	{
	}

	switch (myState)
	{
		case STATE_ERROR:
		{
			reset();

			break;
		}
		default:
		{
			break;
		}
	}

	myTransfer = 0;

	return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private inline methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void I2cSTM32F10x::masterStart()
{
    setBits(myI2c->CR1, I2C_CR1_START);
}

//------------------------------------------------------------------------------
void I2cSTM32F10x::masterStop()
{
    setBits(myI2c->CR1, I2C_CR1_STOP);
}

//------------------------------------------------------------------------------
void I2cSTM32F10x::masterWriteAddress(const uint8_t address,
									  const MasterMode masterMode)
{
    // Shift 7-bit address left by 1 and OR master mode in LSB
    myI2c->DR = ((uint8_t) address << 1) | (uint8_t) masterMode;
}

//------------------------------------------------------------------------------
void I2cSTM32F10x::writeByte(const uint8_t data)
{
    myI2c->DR = data;
}

//------------------------------------------------------------------------------
uint8_t I2cSTM32F10x::readByte()
{
    return ((uint8_t) (myI2c->DR));
}

//------------------------------------------------------------------------------
void I2cSTM32F10x::enableAcknowledge(const bool enable)
{
	setBitsSet(myI2c->CR1, I2C_CR1_ACK, enable);
}

//------------------------------------------------------------------------------
void I2cSTM32F10x::enableAcknowledgePec(const bool enable)
{
	setBitsSet(myI2c->CR1, I2C_CR1_POS, enable);
}

//------------------------------------------------------------------------------
void I2cSTM32F10x::interruptEnable(const Interrupt interrupt, const bool enable)
{
	setBitsSet(myI2c->CR2, myInterruptMap[interrupt], enable);
}

//------------------------------------------------------------------------------
void I2cSTM32F10x::handleI2cEventMasterModeSelect()
{

}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void I2cSTM32F10x::softwareReset()
{
	setBits(myI2c->CR1, I2C_CR1_SWRST);
	clearBits(myI2c->CR1, I2C_CR1_SWRST);
}

//------------------------------------------------------------------------------
void I2cSTM32F10x::reset()
{
    softwareReset();
    driverSetEnabled(true);
    driverSetConfig(getConfig());
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void I2cSTM32F10x::eventInterruptHandler()
{
    if (isNullPointer(myTransfer))
    {
        return;
    }

    unsigned int txBufferCount = 0;
    uint8_t byte = 0;

    txBufferCount = myTransfer->transmitBuffer.count();

    const Event event = (Event) I2C_GetLastEvent(myI2c);

    switch (event)
    {
        case EVENT_MASTER_MODE_SELECT:
        {
            if (txBufferCount > 0)
            {
                masterWriteAddress(myTransfer->address, MASTER_MODE_TX);
            }
            else if (myTransfer->receiveCount > 0)
            {
                masterWriteAddress(myTransfer->address, MASTER_MODE_RX);
            }

            break;
        }
        case EVENT_MASTER_TRANSMITTER_MODE_SELECTED:
        {
            interruptEnable(INTERRUPT_BUFFER, true);

            myTransfer->transmitBuffer.read(byte);
            writeByte(byte);

            break;
        }
        case EVENT_MASTER_RECEIVER_MODE_SELECTED:
        {
            if (myTransfer->receiveCount < 3)
            {
                enableAcknowledge(false);

                if (myTransfer->receiveCount == 2)
                {
                    enableAcknowledgePec(true);
                }
            }
            else
            {
                enableAcknowledge(true);
            }

            interruptEnable(INTERRUPT_BUFFER, true);

            break;
        }
        case EVENT_MASTER_BYTE_TRANSMITTING:
        {
            if (txBufferCount > 0)
            {
                myTransfer->transmitBuffer.read(byte);

                if ((txBufferCount == 1) && (myTransfer->receiveCount == 0))
                {
                    interruptEnable(INTERRUPT_BUFFER, false);
                }

                writeByte(byte);
            }
            else
            {
                interruptEnable(INTERRUPT_BUFFER, false);
            }

            break;
        }
        case EVENT_MASTER_BYTE_TRANSMITTED:
        {
            if (txBufferCount > 0)
            {
                myTransfer->transmitBuffer.read(byte);

                if (txBufferCount == 1)
                {
                    interruptEnable(INTERRUPT_BUFFER, false);
                }

                writeByte(byte);
            }
            else
            {
                if ((myTransfer->receiveCount) > 0)
                {
                    // Restart condition
                    masterStart();
                }
                else
                {
                    interruptEnable(INTERRUPT_BUFFER, false);
                    interruptEnable(INTERRUPT_EVENT, false);
                    masterStop();
                    myState = STATE_IDLE;
                }
            }

            break;
        }
        case EVENT_MASTER_BYTE_RECEIVED:
        {
            unsigned int rxBufferCount = myTransfer->receiveBuffer.count();

            // About to read last byte?
            if (rxBufferCount == (myTransfer->receiveCount - 2))
            {
                enableAcknowledge(false);
                masterStop();
            }
            else if (rxBufferCount == (myTransfer->receiveCount - 1))
            {
                interruptEnable(INTERRUPT_EVENT, false);
                interruptEnable(INTERRUPT_BUFFER, false);

                myState = STATE_IDLE;
            }

            byte = readByte();

            myTransfer->receiveBuffer.write(byte);

            break;
        }
        case EVENT_MASTER_BYTE_RECEIVED_BTF: // RXNE and BTF
        {
            unsigned int rxBufferCount = myTransfer->receiveBuffer.count();

            // About to read byte N-2
            if (rxBufferCount == (myTransfer->receiveCount - 3))
            {
                enableAcknowledge(false);
            }
            // About to read byte N-1
            else if (rxBufferCount == (myTransfer->receiveCount - 2))
            {
                enableAcknowledge(false);
                masterStop();
            }
            else if (rxBufferCount == (myTransfer->receiveCount - 1))
            {
                interruptEnable(INTERRUPT_EVENT, false);
                interruptEnable(INTERRUPT_BUFFER, false);

                myState = STATE_IDLE;
            }

            byte = readByte();

            myTransfer->receiveBuffer.write(byte);

            break;
        }
        default: // Unknown event
        {
            interruptEnable(INTERRUPT_EVENT, false);
            interruptEnable(INTERRUPT_BUFFER, false);

            myState = STATE_ERROR;
            myTransfer->error.setCode(I2c::ERROR_CODE_BUS);

            break;
        }
    }
}

//------------------------------------------------------------------------------
void I2cSTM32F10x::errorInterruptHandler()
{
    reset();
}

//------------------------------------------------------------------------------
// Interrupt service routines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
extern "C" void I2C1_EV_IRQHandler(void)
{
	interruptObjectMap[I2cSTM32F10x::ID_1][0]->handler();
}

//------------------------------------------------------------------------------
extern "C" void I2C1_ER_IRQHandler(void)
{
    interruptObjectMap[I2cSTM32F10x::ID_1][1]->handler();
}

//------------------------------------------------------------------------------
extern "C" void I2C2_EV_IRQHandler(void)
{
    interruptObjectMap[I2cSTM32F10x::ID_2][0]->handler();
}

//------------------------------------------------------------------------------
extern "C" void I2C2_ER_IRQHandler(void)
{
    interruptObjectMap[I2cSTM32F10x::ID_2][1]->handler();
}
