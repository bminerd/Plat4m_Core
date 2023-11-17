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
/// @file I2cSTM32F4xx.cpp
/// @author Ben Minerd
/// @date 3/25/2013
/// @brief I2cSTM32F4xx class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/STM32F4xx/I2cSTM32F4xx.h>
#include <Plat4m_Core/System.h>
#include <Plat4m_Core/CallbackMethod.h>

using Plat4m::I2cSTM32F4xx;
using Plat4m::I2c;
using Plat4m::GpioPinSTM32F4xx;
using Plat4m::InterruptSTM32F4xx;
using Plat4m::Module;
using Plat4m::ProcessorSTM32F4xx;

//------------------------------------------------------------------------------
// Local variables
//------------------------------------------------------------------------------

static InterruptSTM32F4xx* interruptObjectMap[3][2];

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const ProcessorSTM32F4xx::Peripheral I2cSTM32F4xx::myPeripheralMap[] =
{
    ProcessorSTM32F4xx::PERIPHERAL_I2C_1, /// ID_1
    ProcessorSTM32F4xx::PERIPHERAL_I2C_2  /// ID_2
};

const InterruptSTM32F4xx::Id I2cSTM32F4xx::myInterruptIdMap[][2] =
{
    /// ID_1
    {
        InterruptSTM32F4xx::ID_I2C_1_EV,
        InterruptSTM32F4xx::ID_I2C_1_ER
    },
    /// ID_2
    {
        InterruptSTM32F4xx::ID_I2C_2_EV,
        InterruptSTM32F4xx::ID_I2C_2_ER
    },
    /// ID_3
    {
        InterruptSTM32F4xx::ID_I2C_3_EV,
        InterruptSTM32F4xx::ID_I2C_3_ER
    }
};

const uint16_t I2cSTM32F4xx::myAddressBitsMap[] =
{
    I2C_AcknowledgedAddress_7bit, /// I2c::ADDRESS_BITS_7
    I2C_AcknowledgedAddress_10bit /// I2c::ADDRESS_BITS_10
};

const GpioPinSTM32F4xx::OutputSpeed I2cSTM32F4xx::myDefaultOutputSpeed =
                                           GpioPinSTM32F4xx::OUTPUT_SPEED_50MHZ;

const uint16_t I2cSTM32F4xx::myInterruptMap[] =
{
    I2C_IT_EVT, /// INTERRUPT_EVENT
    I2C_IT_BUF, /// INTERRUPT_BUFFER
    I2C_IT_ERR  /// INTERRUPT_ERROR
};

I2C_TypeDef* I2cSTM32F4xx::myI2cMap[] =
{
    I2C1, /// ID_1
    I2C2, /// ID_2
    I2C3  /// ID_3
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
I2cSTM32F4xx::I2cSTM32F4xx(const Id id,
                           GpioPinSTM32F4xx& sclGpioPin,
                           GpioPinSTM32F4xx& sdaGpioPin,
                           const uint8_t ownAddress) :
    I2c(),
    myId(id),
    mySclGpioPin(sclGpioPin),
    mySdaGpioPin(sdaGpioPin),
    myOwnAddress(ownAddress),
    myI2c(myI2cMap[id]),
    myEventInterrupt(
                    myInterruptIdMap[myId][0],
                    createCallback(this, &I2cSTM32F4xx::eventInterruptHandler)),
    myErrorInterrupt(
                    myInterruptIdMap[myId][1],
                    createCallback(this, &I2cSTM32F4xx::errorInterruptHandler)),
    myState(STATE_IDLE),
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
// Private virtual methods overridden for Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error I2cSTM32F4xx::driverSetEnabled(const bool enabled)
{
    mySclGpioPin.setEnabled(enabled);
    mySdaGpioPin.setEnabled(enabled);

    ProcessorSTM32F4xx::setPeripheralClockEnabled(myPeripheralMap[myId],
                                                  enabled);

    if (enabled)
    {
        // Configure GPIO

        // Generic GpioPin configuration
        GpioPin::Config gpioConfig;
        gpioConfig.mode     = GpioPin::MODE_ALTERNATE_FUNCTION;
        gpioConfig.resistor = GpioPin::RESISTOR_PULL_UP;

        mySclGpioPin.configure(gpioConfig);
        mySdaGpioPin.configure(gpioConfig);

        softwareReset();
    }

    I2C_Cmd(myI2c, (FunctionalState) enabled);

    myEventInterrupt.setEnabled(enabled);
    myErrorInterrupt.setEnabled(enabled);

    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private virtual methods overridden for I2c
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
I2c::Error I2cSTM32F4xx::driverSetConfig(const Config& config)
{
    // Configure I2C peripheral

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
I2c::Error I2cSTM32F4xx::driverMasterTransfer(Transfer& transfer)
{
    setInterruptEnabled(INTERRUPT_EVENT, false);

    if (!(getTransferBuffer().write(transfer)))
    {
        // TODO: Check this
        setInterruptEnabled(INTERRUPT_EVENT, true);

        return Error(ERROR_CODE_TRANSFER_BUFFER_FULL);
    }

    setInterruptEnabled(INTERRUPT_EVENT, true);
    setInterruptEnabled(INTERRUPT_ERROR, true);

    if (myState == STATE_IDLE)
    {
        myState = STATE_BUSY;
        getTransferBuffer().peek(myCurrentTransfer);
        masterStart();
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

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private inline methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void I2cSTM32F4xx::masterStart()
{
    setBits(myI2c->CR1, Register::Cr1::BIT_START_GENERATION);
}

//------------------------------------------------------------------------------
void I2cSTM32F4xx::masterStop()
{
    setBits(myI2c->CR1, Register::Cr1::BIT_STOP_GENERATION);
}

//------------------------------------------------------------------------------
void I2cSTM32F4xx::masterWriteAddress(const uint8_t address,
									  const MasterMode masterMode)
{
    // Shift 7-bit address left by 1 and OR master mode in LSB
    myI2c->DR = ((uint8_t) address << 1) | (uint8_t) masterMode;
}

//------------------------------------------------------------------------------
void I2cSTM32F4xx::writeByte(const uint8_t data)
{
    myI2c->DR = data;
}

//------------------------------------------------------------------------------
uint8_t I2cSTM32F4xx::readByte()
{
    return ((uint8_t) (myI2c->DR));
}

//------------------------------------------------------------------------------
void I2cSTM32F4xx::enableAcknowledge(const bool enable)
{
    setBitsSet(myI2c->CR1,
               Register::Cr1::BIT_ACKNOWLEDGE_ENABLE,
               enable);
}

//------------------------------------------------------------------------------
void I2cSTM32F4xx::enableAcknowledgePec(const bool enable)
{
    setBitsSet(myI2c->CR1,
               Register::Cr1::BIT_ACKNOWLEDGE_PEC_POSITION,
               enable);
}

//------------------------------------------------------------------------------
void I2cSTM32F4xx::setInterruptEnabled(const Interrupt interrupt,
                                       const bool enable)
{
    setBitsSet(myI2c->CR2, myInterruptMap[interrupt], enable);
}

//------------------------------------------------------------------------------
void I2cSTM32F4xx::disableAllInterrupts()
{
    setInterruptEnabled(INTERRUPT_EVENT,  false);
    setInterruptEnabled(INTERRUPT_BUFFER, false);
    setInterruptEnabled(INTERRUPT_ERROR,  false);
}

//------------------------------------------------------------------------------
void I2cSTM32F4xx::handleMasterModeSelectEvent()
{
    if (myCurrentTransfer->transmitBuffer.count() > 0)
    {
        masterWriteAddress(myCurrentTransfer->address, MASTER_MODE_TX);
    }
    else if (myCurrentTransfer->receiveCount > 0)
    {
        masterWriteAddress(myCurrentTransfer->address, MASTER_MODE_RX);
    }
}

//------------------------------------------------------------------------------
void I2cSTM32F4xx::handleMasterTransmitterModeSelectedEvent()
{
    uint8_t byte;

    setInterruptEnabled(INTERRUPT_BUFFER, true);

    myCurrentTransfer->transmitBuffer.read(byte);
    writeByte(byte);
}

//------------------------------------------------------------------------------
void I2cSTM32F4xx::handleMasterReceiverModeSelectedEvent()
{
    if (myCurrentTransfer->receiveCount < 3)
    {
        enableAcknowledge(false);

        if (myCurrentTransfer->receiveCount == 2)
        {
            enableAcknowledgePec(true);
        }
    }
    else
    {
        enableAcknowledge(true);
    }

    setInterruptEnabled(INTERRUPT_BUFFER, true);
}

//------------------------------------------------------------------------------
void I2cSTM32F4xx::handleMasterByteTransmittingEvent()
{
    uint32_t transmitBufferCount = myCurrentTransfer->transmitBuffer.count();

    if (transmitBufferCount > 0)
    {
        uint8_t byte;
        myCurrentTransfer->transmitBuffer.read(byte);

        if (transmitBufferCount == 1)
        {
            setInterruptEnabled(INTERRUPT_BUFFER, false);
        }

        writeByte(byte);
    }
    else
    {
        setInterruptEnabled(INTERRUPT_BUFFER, false);
    }
}

//------------------------------------------------------------------------------
void I2cSTM32F4xx::handleMasterByteTransmittedEvent()
{
    uint32_t transmitBufferCount = myCurrentTransfer->transmitBuffer.count();
    uint8_t byte;

    if (transmitBufferCount > 0)
    {
        myCurrentTransfer->transmitBuffer.read(byte);

        if (transmitBufferCount == 1)
        {
            setInterruptEnabled(INTERRUPT_BUFFER, false);
        }

        writeByte(byte);
    }
    else
    {
        if ((myCurrentTransfer->receiveCount) > 0)
        {
            // Restart condition
            setInterruptEnabled(INTERRUPT_EVENT, false);
            masterStart();
            setInterruptEnabled(INTERRUPT_EVENT, true);
        }
        else
        {
            setInterruptEnabled(INTERRUPT_BUFFER, false);
            setInterruptEnabled(INTERRUPT_EVENT, false);
            masterStop();
            myState = STATE_IDLE;
        }
    }
}

//------------------------------------------------------------------------------
void I2cSTM32F4xx::handleMasterByteReceivingEvent()
{
    uint32_t receiveBufferCount = myCurrentTransfer->receiveBuffer.count();

    if ((myCurrentTransfer->receiveCount != 2) &&
        (receiveBufferCount != (myCurrentTransfer->receiveCount - 3)))
    {
        uint8_t byte;

        if (receiveBufferCount == (myCurrentTransfer->receiveCount - 1))
        {
            setInterruptEnabled(INTERRUPT_EVENT, false);
            setInterruptEnabled(INTERRUPT_BUFFER, false);

            masterStop();

            byte = readByte();
            myCurrentTransfer->receiveBuffer.write(byte);

            myState = STATE_IDLE;
        }
        else if (receiveBufferCount == myCurrentTransfer->receiveCount)
        {
            byte = readByte();
        }
        else
        {
            byte = readByte();
            myCurrentTransfer->receiveBuffer.write(byte);
        }
    }
}

//------------------------------------------------------------------------------
void I2cSTM32F4xx::handleMasterByteReceivedEvent()
{
    uint32_t receiveBufferCount = myCurrentTransfer->receiveBuffer.count();
    uint8_t byte;

    // About to read byte N-2
    if (receiveBufferCount == (myCurrentTransfer->receiveCount - 3))
    {
        enableAcknowledge(false);
        byte = readByte();
        myCurrentTransfer->receiveBuffer.write(byte);
    }
    // About to read byte N-1 (this covers 2-byte and N-byte reception)
    else if (receiveBufferCount == (myCurrentTransfer->receiveCount - 2))
    {
        setInterruptEnabled(INTERRUPT_BUFFER, false);
        masterStop();

        // Read data N-1
        byte = readByte();
        myCurrentTransfer->receiveBuffer.write(byte);

        // Read data N
        byte = readByte();
        myCurrentTransfer->receiveBuffer.write(byte);

        if (isValidPointer(myCurrentTransfer->mailbox))
        {
            myCurrentTransfer->mailbox->receiveByteArray->setSize(
                                               myCurrentTransfer->receiveCount);
            myCurrentTransfer->mailbox->error.setCode(
                                          (MasterSlaveBus::ErrorCode)
                                          (myCurrentTransfer->error.getCode()));
            myCurrentTransfer->mailbox->mailboxFullCallback->call();
        }

        getTransferBuffer().emptyRead();

        if (getTransferBuffer().count() != 0)
        {
            getTransferBuffer().peek(myCurrentTransfer);
            masterStart();
        }
        else
        {
            myState = STATE_IDLE;
            myCurrentTransfer = 0;
        }
    }
    else if (receiveBufferCount == (myCurrentTransfer->receiveCount - 1))
    {
        byte = readByte();
        myCurrentTransfer->receiveBuffer.write(byte);

        if (isValidPointer(myCurrentTransfer->mailbox))
        {
            myCurrentTransfer->mailbox->receiveByteArray->setSize(
                                               myCurrentTransfer->receiveCount);
            myCurrentTransfer->mailbox->error.setCode(
                                          (MasterSlaveBus::ErrorCode)
                                          (myCurrentTransfer->error.getCode()));
            myCurrentTransfer->mailbox->mailboxFullCallback->call();
        }

        getTransferBuffer().emptyRead();

        if (getTransferBuffer().count() != 0)
        {
            getTransferBuffer().peek(myCurrentTransfer);
            masterStart();
        }
        else
        {
            myState = STATE_IDLE;
            myCurrentTransfer = 0;
        }
    }
    else
    {
        byte = readByte();
        myCurrentTransfer->receiveBuffer.write(byte);
    }
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void I2cSTM32F4xx::softwareReset()
{
    setBits(myI2c->CR1, Register::Cr1::BIT_SOFTWARE_RESET);
    clearBits(myI2c->CR1, Register::Cr1::BIT_SOFTWARE_RESET);
}

//------------------------------------------------------------------------------
void I2cSTM32F4xx::reset()
{
    softwareReset();
    driverSetEnabled(true);
    driverSetConfig(getConfig());
}

//------------------------------------------------------------------------------
void I2cSTM32F4xx::eventInterruptHandler()
{
    const uint32_t event = I2C_GetLastEvent(myI2c);

    switch (event)
    {
        case EVENT_MASTER_MODE_SELECT:
        {
            handleMasterModeSelectEvent();

            break;
        }
        case EVENT_MASTER_TRANSMITTER_MODE_SELECTED:
        {
            handleMasterTransmitterModeSelectedEvent();

            break;
        }
        case EVENT_MASTER_RECEIVER_MODE_SELECTED:
        {
            handleMasterReceiverModeSelectedEvent();

            break;
        }
        case EVENT_MASTER_BYTE_TRANSMITTING:
        {
            handleMasterByteTransmittingEvent();

            break;
        }
        case EVENT_MASTER_BYTE_TRANSMITTED:
        {
            handleMasterByteTransmittedEvent();

            break;
        }
        case EVENT_MASTER_BYTE_RECEIVING:
        {
            handleMasterByteReceivingEvent();

            break;
        }
        case EVENT_MASTER_BYTE_RECEIVED:
        {
            handleMasterByteReceivedEvent();

            break;
        }
        default: // Unknown event
        {
            if (areBitsClear(myI2c->CR1, Register::Cr1::BIT_START_GENERATION))
            {
                disableAllInterrupts();

                myState = STATE_ERROR;
                myCurrentTransfer->error.setCode(I2c::ERROR_CODE_BUS);
            }

            break;
        }
    }
}

//------------------------------------------------------------------------------
void I2cSTM32F4xx::errorInterruptHandler()
{
    reset();
}

//------------------------------------------------------------------------------
// Interrupt service routines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
extern "C" void I2C1_EV_IRQHandler(void)
{
    interruptObjectMap[I2cSTM32F4xx::ID_1][0]->handler();
}

//------------------------------------------------------------------------------
extern "C" void I2C1_ER_IRQHandler(void)
{
    interruptObjectMap[I2cSTM32F4xx::ID_1][1]->handler();
}

//------------------------------------------------------------------------------
extern "C" void I2C2_EV_IRQHandler(void)
{
    interruptObjectMap[I2cSTM32F4xx::ID_2][0]->handler();
}

//------------------------------------------------------------------------------
extern "C" void I2C2_ER_IRQHandler(void)
{
    interruptObjectMap[I2cSTM32F4xx::ID_2][1]->handler();
}

//------------------------------------------------------------------------------
extern "C" void I2C3_EV_IRQHandler(void)
{
    interruptObjectMap[I2cSTM32F4xx::ID_3][0]->handler();
}

//------------------------------------------------------------------------------
extern "C" void I2C3_ER_IRQHandler(void)
{
    interruptObjectMap[I2cSTM32F4xx::ID_3][1]->handler();
}
