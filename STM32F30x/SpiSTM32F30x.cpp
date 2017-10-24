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
/// @file SpiSTM32F30x.cpp
/// @author Ben Minerd
/// @date 5/15/2017
/// @brief SpiSTM32F30x class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <SpiSTM32F30x.h>
#include <Buffer.h>
#include <CallbackMethod.h>

using Plat4m::SpiSTM32F30x;
using Plat4m::Spi;
using Plat4m::GpioPinSTM32F30x;
using Plat4m::Module;
using Plat4m::InterruptSTM32F30x;
using Plat4m::ProcessorSTM32F30x;

//------------------------------------------------------------------------------
// Local variables
//------------------------------------------------------------------------------

static InterruptSTM32F30x* interruptObjectMap[3];

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

// Constants

const ProcessorSTM32F30x::Peripheral SpiSTM32F30x::myPeripheralMap[] =
{
    ProcessorSTM32F30x::PERIPHERAL_SPI_1, /// ID_1
    ProcessorSTM32F30x::PERIPHERAL_SPI_2, /// ID_2
    ProcessorSTM32F30x::PERIPHERAL_SPI_3  /// ID_3
};

const InterruptSTM32F30x::Id SpiSTM32F30x::myInterruptIdMap[] =
{
    InterruptSTM32F30x::ID_SPI_1, /// ID_1
    InterruptSTM32F30x::ID_SPI_2, /// ID_2
    InterruptSTM32F30x::ID_SPI_3  /// ID_3
};

const uint16_t SpiSTM32F30x::myModeMap[] =
{
    SPI_CR1_MSTR, /// Spi::MODE_MASTER
    0             /// Spi::MODE_SLAVE
};

const uint16_t SpiSTM32F30x::myDataBitsMap[] =
{
    /// Spi::DATA_BITS_8
    SPI_CR2_DS_0 | SPI_CR2_DS_1 | SPI_CR2_DS_2,
    /// Spi::DATA_BITS_16
    SPI_CR2_DS_0 | SPI_CR2_DS_1 | SPI_CR2_DS_2 | SPI_CR2_DS_3
};

const uint16_t SpiSTM32F30x::myClockPolarityMap[] =
{
    0,           /// Spi::CLOCK_POLARITY_LOW
    SPI_CR1_CPOL /// Spi::CLOCK_POLARITY_HIGH
};

const uint16_t SpiSTM32F30x::myClockPhaseMap[] =
{
    0,           /// Spi::CLOCK_PHASE_LEADING_EDGE
    SPI_CR1_CPHA /// Spi::CLOCK_PHASE_TRAILING_EDGE
};

const SpiSTM32F30x::Prescaler SpiSTM32F30x::myClockPrescalerMap[] =
{
    {2,   SPI_BaudRatePrescaler_2},   /// CLOCK_PRESCALER_2
    {4,   SPI_BaudRatePrescaler_4},   /// CLOCK_PRESCALER_4
    {8,   SPI_BaudRatePrescaler_8},   /// CLOCK_PRESCALER_8
    {16,  SPI_BaudRatePrescaler_16},  /// CLOCK_PRESCALER_16
    {32,  SPI_BaudRatePrescaler_32},  /// CLOCK_PRESCALER_32
    {64,  SPI_BaudRatePrescaler_64},  /// CLOCK_PRESCALER_64
    {128, SPI_BaudRatePrescaler_128}, /// CLOCK_PRESCALER_128
    {256, SPI_BaudRatePrescaler_256}  /// CLOCK_PRESCALER_256
};

const uint16_t SpiSTM32F30x::myBitOrderMap[] =
{
    SPI_CR1_LSBFIRST, /// Spi::BIT_ORDER_LSB_FIRST
    0                 /// Spi::BIT_ORDER_MSB_FIRST
};

const uint8_t SpiSTM32F30x::myInterruptEnableMap[] =
{
    SPI_CR2_TXEIE,  /// INTERRUPT_TRANSMIT_BUFFER_EMPTY
    SPI_CR2_RXNEIE, /// INTERRUPT_RECEIVE_BUFFER_NOT_EMPTY
	SPI_CR2_ERRIE   /// INTERRUPT_ERROR
};

const uint8_t SpiSTM32F30x::myInterruptEventPendingMap[] =
{
    SPI_SR_TXE,   /// INTERRUPT_TRANSMIT_BUFFER_EMPTY
    SPI_SR_RXNE,  /// INTERRUPT_RECEIVE_BUFFER_NOT_EMPTY
    SPI_SR_MODF,  /// INTERRUPT_EVENT_MASTER_MODE_FAULT
    SPI_SR_OVR,   /// INTERRUPT_EVENT_OVERRUN_ERROR
    SPI_SR_CRCERR /// INTERRUPT_EVENT_CRC_ERROR
};

// Variables

SPI_TypeDef* SpiSTM32F30x::mySpiMap[] =
{
    SPI1, /// ID_1
    SPI2, /// ID_2
    SPI3  /// ID_3
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
SpiSTM32F30x::SpiSTM32F30x(const Id id,
                           const TransmissionMode transmissionMode,
                           GpioPinSTM32F30x& sckGpioPin,
                           GpioPinSTM32F30x& misoGpioPin,
                           GpioPinSTM32F30x& mosiGpioPin,
                           const GpioRemap gpioRemap) :
    Spi(transmissionMode),
    myId(id),
    mySpi(mySpiMap[myId]),
    mySckGpioPin(sckGpioPin),
    myMisoGpioPin(&misoGpioPin),
    myMosiGpioPin(&mosiGpioPin),
    myGpioRemap(gpioRemap),
    myClockPrescaler(CLOCK_PRESCALER_2),
    myState(STATE_IDLE),
    myTransfer(0),
    myInterrupt(myInterruptIdMap[myId],
                createCallback(this, &SpiSTM32F30x::interruptHandler)),
    myTransmitDmaChannel(0),
    myReceiveDmaChannel(0),
    myReceivedByteCount(0),
    myTransmittedByteCount(0)
{
    if (isValidPointer(interruptObjectMap[myId]))
    {
        // Trying to instantiate peripheral twice, lockup
        while (true)
        {
        }
    }

    interruptObjectMap[myId] = &myInterrupt;
}

//------------------------------------------------------------------------------
SpiSTM32F30x::SpiSTM32F30x(const Id id,
                           const TransmissionMode transmissionMode,
                           GpioPinSTM32F30x& sckGpioPin,
                           GpioPinSTM32F30x& dataGpioPin,
                           const GpioRemap gpioRemap) :
    Spi(transmissionMode),
    myId(id),
    mySpi(mySpiMap[myId]),
    mySckGpioPin(sckGpioPin),
    myMisoGpioPin(0),
    myMosiGpioPin(0),
    myGpioRemap(gpioRemap),
    myClockPrescaler(CLOCK_PRESCALER_2),
    myState(STATE_IDLE),
    myTransfer(0),
    myInterrupt(myInterruptIdMap[myId],
                createCallback(this, &SpiSTM32F30x::interruptHandler)),
    myTransmitDmaChannel(0),
    myReceiveDmaChannel(0),
    myReceivedByteCount(0),
    myTransmittedByteCount(0)
{
    switch (transmissionMode)
    {
        case TRANSMISSION_MODE_RX:
        {
            myMisoGpioPin = &dataGpioPin;

            break;
        }
        case TRANSMISSION_MODE_TX:
        {
            myMosiGpioPin = &dataGpioPin;

            break;
        }
    }

    if (isValidPointer(interruptObjectMap[myId]))
    {
        // Trying to instantiate peripheral twice, lockup
        while (true)
        {
        }
    }

    interruptObjectMap[myId] = &myInterrupt;
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
SpiSTM32F30x::~SpiSTM32F30x()
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void SpiSTM32F30x::setTransmitDmaChannel(
                                        DmaChannelSTM32F30x& transmitDmaChannel)
{
    myTransmitDmaChannel = &transmitDmaChannel;

    myTransmitDmaChannel->setTransferCompleteCallback(
            createCallback(this,
                           &SpiSTM32F30x::transmitDmaTransferCompleteCallback));
}

//------------------------------------------------------------------------------
void SpiSTM32F30x::setReceiveDmaChannel(DmaChannelSTM32F30x& receiveDmaChannel)
{
    myReceiveDmaChannel = &receiveDmaChannel;

    myReceiveDmaChannel->setTransferCompleteCallback(
             createCallback(this,
                            &SpiSTM32F30x::receiveDmaTransferCompleteCallback));
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error SpiSTM32F30x::driverSetEnabled(const bool enabled)
{
    mySckGpioPin.setEnabled(enabled);

    if (isValidPointer(myMisoGpioPin))
    {
        myMisoGpioPin->enable(enabled);
    }
    
    if (isValidPointer(myMosiGpioPin))
    {
        myMosiGpioPin->enable(enabled);
    }

    ProcessorSTM32F30x::setPeripheralClockEnabled(myPeripheralMap[myId],
                                                  enabled);
    
    setPeripheralEnabled(enabled);
    
    InterruptSTM32F30x::Config interruptConfig;
    interruptConfig.priority = 7;
    myInterrupt.configure(interruptConfig);
    myInterrupt.setEnabled(enabled);

    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods implemented from Spi
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Spi::Error SpiSTM32F30x::driverSetConfig(const Config& config)
{
    setPeripheralEnabled(false);
//    SPI_Cmd(mySpi, DISABLE);

    GpioPin::Config gpioConfig;

    // MISO configuration

    if (isValidPointer(myMisoGpioPin))
    {
        gpioConfig.mode     = GpioPin::MODE_ALTERNATE_FUNCTION;
        gpioConfig.resistor = GpioPin::RESISTOR_NONE;
        myMisoGpioPin->configure(gpioConfig);
    }
    
    // MOSI configuration

    if (isValidPointer(myMosiGpioPin))
    {
        gpioConfig.mode     = GpioPin::MODE_ALTERNATE_FUNCTION;
        gpioConfig.resistor = GpioPin::RESISTOR_NONE;
        myMosiGpioPin->configure(gpioConfig);
    }
    
    // SCK configuration

    if ((config.clockPolarity) == CLOCK_POLARITY_HIGH)
    {
        gpioConfig.resistor = GpioPin::RESISTOR_PULL_UP;
    }
    else
    {
        gpioConfig.resistor = GpioPin::RESISTOR_PULL_DOWN;
    }
    
    gpioConfig.mode = GpioPin::MODE_ALTERNATE_FUNCTION;

    mySckGpioPin.configure(gpioConfig);

    if (!setClockFrequencyHz(config.clockFrequencyHz))
    {
        return Error(ERROR_CODE_CLOCK_FREQUENCY_INVALID);
    }

    setMode(config.mode);
    setDataBits(config.dataBits);
    setClockPolarity(config.clockPolarity);
    setClockPhase(config.clockPhase);
    setBitOrder(config.bitOrder);

    // TODO: Add configuration for this?
    setSoftwareSlaveManagementEnabled(true);
    setBits(mySpi->CR1, SPI_CR1_SSI);
    setCrcEnabled(false);
    setCrcPolynomial(7);

    if (isValidPointer(myMisoGpioPin) && isValidPointer(myMosiGpioPin))
    {
        setDataMode(DATA_MODE_2_LINE_UNIDIRECTIONAL);
        setReceiveOnlyEnabled(false);
    }
    else
    {
        setDataMode(DATA_MODE_1_LINE_BIDIRECTIONAL);
    }

    setPeripheralEnabled(true);

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
Spi::Error SpiSTM32F30x::driverMasterTransfer(Transfer& transfer)
{
	myTransfer = &transfer;

	myState = STATE_BUSY;

	// Chip select
	if (isValidPointer(transfer.chipSelectGpioPin))
	{
		transfer.chipSelectGpioPin->setLevel(GpioPin::LEVEL_LOW);
	}

	// Enable Tx and Rx interrupts to start transfer

	if (transfer.receiveCount != 0)
	{
	    if (isValidPointer(myReceiveDmaChannel))
	    {
	        DmaChannelSTM32F30x::Config dmaChannelConfig;
	        dmaChannelConfig.dataTransferDirection =
                               DmaChannelSTM32F30x::
	                               DATA_TRANSFER_DIRECTION_READ_FROM_PERIPHERAL;
	        dmaChannelConfig.circularModeEnabled            = false,
	        dmaChannelConfig.peripheralIncrementModeEnabled = false;
	        dmaChannelConfig.memoryIncrementModeEnabled     = true;
	        dmaChannelConfig.peripheralSize =
                                    DmaChannelSTM32F30x::PERIPHERAL_SIZE_8_BITS;
	        dmaChannelConfig.memorySize =
	                                    DmaChannelSTM32F30x::MEMORY_SIZE_8_BITS;
	        dmaChannelConfig.priorityLevel =
	                                   DmaChannelSTM32F30x::PRIORITY_LEVEL_HIGH;
	        dmaChannelConfig.memoryToMemoryModeEnabled      = false;
	        dmaChannelConfig.nDataToTransfer   = transfer.transmitCount +
                                                 transfer.receiveCount;
	        dmaChannelConfig.peripheralAddress = (uint32_t) &(mySpi->DR);
	        dmaChannelConfig.memoryAddress     =
	                             (uint32_t) (transfer.receiveBuffer.getItems());

	        myReceiveDmaChannel->setConfig(dmaChannelConfig);
	        myReceiveDmaChannel->setEnabledPrivate(true);

	        setReceiveDmaEnabled(true);
	    }
	    else
	    {
	        setInterruptEnabled(INTERRUPT_RECEIVE_BUFFER_NOT_EMPTY, true);

	        if (transfer.transmitCount == 0)
	        {
//	            myInterrupt.setEnabled(true);
	        }
	    }
	}

	if (transfer.transmitCount != 0)
	{
	    if (isValidPointer(myTransmitDmaChannel))
	    {
	        DmaChannelSTM32F30x::Config dmaChannelConfig;
	        dmaChannelConfig.dataTransferDirection =
                  DmaChannelSTM32F30x::DATA_TRANSFER_DIRECTION_READ_FROM_MEMORY;
	        dmaChannelConfig.circularModeEnabled            = false,
	        dmaChannelConfig.peripheralIncrementModeEnabled = false;
	        dmaChannelConfig.memoryIncrementModeEnabled     = true;
	        dmaChannelConfig.peripheralSize =
                                    DmaChannelSTM32F30x::PERIPHERAL_SIZE_8_BITS;
	        dmaChannelConfig.memorySize =
	                                    DmaChannelSTM32F30x::MEMORY_SIZE_8_BITS;
	        dmaChannelConfig.priorityLevel =
	                                   DmaChannelSTM32F30x::PRIORITY_LEVEL_HIGH;
	        dmaChannelConfig.memoryToMemoryModeEnabled      = false;
	        dmaChannelConfig.nDataToTransfer   = transfer.transmitCount +
	                                             transfer.receiveCount;
	        dmaChannelConfig.peripheralAddress = (uint32_t) &(mySpi->DR);
	        dmaChannelConfig.memoryAddress     =
	                            (uint32_t) (transfer.transmitBuffer.getItems());

	        myTransmitDmaChannel->setConfig(dmaChannelConfig);
	        myTransmitDmaChannel->setEnabledPrivate(true);

	        setTransmitDmaEnabled(true);
	    }
	    else
	    {
            setInterruptEnabled(INTERRUPT_TRANSMIT_BUFFER_EMPTY, true);
//            myInterrupt.setEnabled(true);
	    }
	}

	if ((transfer.transferMode == TRANSFER_MODE_TRANSMIT)                     ||
        (transfer.transferMode == TRANSFER_MODE_TRANSMIT_MAILBOX)             ||
        (transfer.transferMode ==
                           TRANSFER_MODE_TRANSMIT_RECEIVE_SEQUENTIAL_MAILBOX) ||
        (transfer.transferMode ==
                             TRANSFER_MODE_TRANSMIT_RECEIVE_CONCURRENT_MAILBOX))
	{
		// Return immediately and let the SPI interface continue asynchronously
		return Error(ERROR_CODE_NONE);
	}

	while (myState == STATE_BUSY)
	{
	}

	if (isValidPointer(myTransmitDmaChannel))
	{
	    setTransmitDmaEnabled(false);
	    myTransmitDmaChannel->setEnabledPrivate(false);
	}
	else
	{
//	    myInterrupt.setEnabled(false);
	}

    if (isValidPointer(myReceiveDmaChannel))
    {
        setReceiveDmaEnabled(false);
        myReceiveDmaChannel->setEnabledPrivate(false);
    }
    else
    {
//        myInterrupt.setEnabled(false);
    }

    uint8_t byte = readByte();
    uint8_t status = mySpi->SR;

    while (areBitsClear(mySpi->SR, SPI_SR_TXE))
    {
        // Do nothing
    }

    while (areBitsSet(mySpi->SR, SPI_SR_BSY))
    {
        // Do nothing
    }

    if (isValidPointer(transfer.chipSelectGpioPin))
    {
        myTransfer->chipSelectGpioPin->setLevel(GpioPin::LEVEL_HIGH);
    }

    while (isInterruptEventPending(INTERRUPT_EVENT_RECEIVE_BUFFER_NOT_EMPTY))
    {
        byte = readByte();
    }

	switch (myState)
	{
		case STATE_ERROR:
		{
			//TODO: reset(); ?

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
void SpiSTM32F30x::setPeripheralEnabled(const bool enabled)
{
    setBitsSet(mySpi->CR1, SPI_CR1_SPE, enabled);
}

//------------------------------------------------------------------------------
bool SpiSTM32F30x::setClockFrequencyHz(const uint32_t clockFrequencyHz)
{
    uint32_t i;
    uint32_t size = arraySize(myClockPrescalerMap);

    for (i = 0; i < size; i++)
    {
        if (myId == ID_1)
        {
            if ((ProcessorSTM32F30x::getApb2ClockFrequencyHz() /
                 myClockPrescalerMap[i].value) ==
                                                               clockFrequencyHz)
            {
                break;
            }
        }
        else // myId == ID_2 || myId == ID_3
        {
            if ((ProcessorSTM32F30x::getApb1ClockFrequencyHz() /
                 myClockPrescalerMap[i].value) ==
                                                               clockFrequencyHz)
            {
                break;
            }
        }
    }

    // True if no prescaler was found to make the desired clock frequency valid
    if (i == size)
    {
        return false;
    }

    myClockPrescaler = (ClockPrescaler) i;

    clearAndSetBits(mySpi->CR1,
                    SPI_CR1_BR,
                    myClockPrescalerMap[myClockPrescaler].bits);

    return true;
}

//------------------------------------------------------------------------------
void SpiSTM32F30x::setMode(const Mode mode)
{
    clearAndSetBits(mySpi->CR1, SPI_CR1_MSTR, myModeMap[mode]);
}

//------------------------------------------------------------------------------
void SpiSTM32F30x::setDataBits(const DataBits dataBits)
{
    clearAndSetBits(mySpi->CR1, SPI_CR2_DS, myDataBitsMap[dataBits]);
    clearBits(mySpi->CR2, SPI_CR2_FRXTH);

    if (dataBits == DATA_BITS_8)
    {
        setBits(mySpi->CR2, SPI_CR2_FRXTH);
    }
}

//------------------------------------------------------------------------------
void SpiSTM32F30x::setClockPolarity(const ClockPolarity clockPolarity)
{
    clearAndSetBits(mySpi->CR1,
                    SPI_CR1_CPOL,
                    myClockPolarityMap[clockPolarity]);
}

//------------------------------------------------------------------------------
void SpiSTM32F30x::setClockPhase(const ClockPhase clockPhase)
{
    clearAndSetBits(mySpi->CR1, SPI_CR1_CPHA, myClockPhaseMap[clockPhase]);
}

//------------------------------------------------------------------------------
void SpiSTM32F30x::setBitOrder(const BitOrder bitOrder)
{
    clearAndSetBits(mySpi->CR1, SPI_CR1_LSBFIRST, myBitOrderMap[bitOrder]);
}

//------------------------------------------------------------------------------
void SpiSTM32F30x::setDataMode(const DataMode dataMode)
{
    clearAndSetBits(mySpi->CR1, SPI_CR1_BIDIMODE, ((uint16_t) dataMode) << 15);
}

//------------------------------------------------------------------------------
void SpiSTM32F30x::setBidirectionalOutputEnabled(const bool enabled)
{
    setBitsSet(mySpi->CR1, SPI_CR1_BIDIOE, enabled);
}

//------------------------------------------------------------------------------
void SpiSTM32F30x::setReceiveOnlyEnabled(const bool enabled)
{
    setBitsSet(mySpi->CR1, SPI_CR1_RXONLY, enabled);
}

//------------------------------------------------------------------------------
void SpiSTM32F30x::setSoftwareSlaveManagementEnabled(const bool enabled)
{
    setBitsSet(mySpi->CR1, SPI_CR1_SSM, enabled);
}

//------------------------------------------------------------------------------
void SpiSTM32F30x::setCrcEnabled(const bool enabled)
{
    setBitsSet(mySpi->CR1, SPI_CR1_CRCEN, enabled);
}

//------------------------------------------------------------------------------
void SpiSTM32F30x::setCrcPolynomial(const uint16_t crcPolynomial)
{
    mySpi->CRCPR = crcPolynomial;
}

//------------------------------------------------------------------------------
void SpiSTM32F30x::writeByte(const uint8_t byte)
{
    *((uint8_t*) &(mySpi->DR)) = byte;
}

//------------------------------------------------------------------------------
uint8_t SpiSTM32F30x::readByte()
{
    return (*((uint8_t*) &(mySpi->DR)));
}

//------------------------------------------------------------------------------
void SpiSTM32F30x::setInterruptEnabled(const Interrupt interruptEvent,
                                       const bool enabled)
{
    setBitsSet(mySpi->CR2, myInterruptEnableMap[interruptEvent], enabled);
}

//------------------------------------------------------------------------------
bool SpiSTM32F30x::isInterruptEnabled(const Interrupt interruptEvent)
{
    return areBitsSet(mySpi->CR2, myInterruptEnableMap[interruptEvent]);
}

//------------------------------------------------------------------------------
bool SpiSTM32F30x::isInterruptEventPending(const InterruptEvent interruptEvent)
{
    return areBitsSet(mySpi->SR, myInterruptEventPendingMap[interruptEvent]);
}

//------------------------------------------------------------------------------
void SpiSTM32F30x::setTransmitDmaEnabled(const bool enabled)
{
    setBitsSet(mySpi->CR2, SPI_CR2_TXDMAEN, enabled);
}

//------------------------------------------------------------------------------
bool SpiSTM32F30x::isTransmitDmaEnabled()
{
    return areBitsSet(mySpi->CR2, SPI_CR2_TXDMAEN);
}

//------------------------------------------------------------------------------
void SpiSTM32F30x::setReceiveDmaEnabled(const bool enabled)
{
    setBitsSet(mySpi->CR2, SPI_CR2_RXDMAEN, enabled);
}

//------------------------------------------------------------------------------
bool SpiSTM32F30x::isReceiveDmaEnabled()
{
    return areBitsSet(mySpi->CR2, SPI_CR2_RXDMAEN);
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void SpiSTM32F30x::interruptHandler()
{
    // TODO Change to support 16bit data
    uint8_t byte = 0;

    // NOTE:
    // Putting this check here implies that an additional interrupt is required
    // to stop the transfer
    if ((myTransfer->transmitBuffer.isEmpty()) &&
        (((myTransfer->receiveCount) == 0) ||
         ((myTransfer->receiveCount) == myTransfer->receiveBuffer.count())))
    {
        setInterruptEnabled(INTERRUPT_TRANSMIT_BUFFER_EMPTY,    false);
        setInterruptEnabled(INTERRUPT_RECEIVE_BUFFER_NOT_EMPTY, false);

        myState = STATE_IDLE;
        myReceivedByteCount = 0;
        myTransmittedByteCount = 0;

        return;
    }

    // Transmit interrupt event
    if (isInterruptEnabled(INTERRUPT_TRANSMIT_BUFFER_EMPTY) &&
        isInterruptEventPending(INTERRUPT_EVENT_TRANSMIT_BUFFER_EMPTY))
    {
        switch (myTransfer->transferMode)
        {
            case TRANSFER_MODE_TRANSMIT_RECEIVE_SEQUENTIAL: // Fall through
            case TRANSFER_MODE_TRANSMIT_RECEIVE_SEQUENTIAL_MAILBOX:
            {
                if (myTransfer->transmitBuffer.read(byte))
                {
                    writeByte(byte);
                }
                else
                {
                    writeByte(0);
                }

                break;
            }
            case TRANSFER_MODE_TRANSMIT:                    // Fall through
            case TRANSFER_MODE_TRANSMIT_WAIT:               // Fall through
            case TRANSFER_MODE_TRANSMIT_RECEIVE_CONCURRENT: // Fall through
            case TRANSFER_MODE_TRANSMIT_RECEIVE_CONCURRENT_MAILBOX:
            {
                if (myTransfer->transmitBuffer.read(byte))
                {
                    writeByte(byte);
                }

                break;
            }
            default:
            {
                break; // Does this make sense?
            }
        }

        myTransmittedByteCount++;
    }

    // Receive flag
    if (isInterruptEventPending(INTERRUPT_EVENT_RECEIVE_BUFFER_NOT_EMPTY))
    {
        byte = readByte();
        myReceivedByteCount++;

        switch (myTransfer->transferMode)
        {
            case TRANSFER_MODE_TRANSMIT_RECEIVE_SEQUENTIAL: // Fall through
            case TRANSFER_MODE_TRANSMIT_RECEIVE_SEQUENTIAL_MAILBOX:
            {
                // Ignore the bytes received while transmitting
                if (myReceivedByteCount > myTransfer->transmitCount)
                {
                    if (!(myTransfer->receiveBuffer.write(byte)))
                    {
                    }
                }

                break;
            }
            case TRANSFER_MODE_TRANSMIT_RECEIVE_CONCURRENT: // Fall through
            case TRANSFER_MODE_TRANSMIT_RECEIVE_CONCURRENT_MAILBOX:
            {
                if (!(myTransfer->receiveBuffer.write(byte)))
                {
                    // Error?
                }

                break;
            }
            default:
            {
                break;
            }
        }
    }

    if (isInterruptEnabled(INTERRUPT_ERROR) &&
        isInterruptEventPending(INTERRUPT_EVENT_OVERRUN_ERROR))
//    if (isInterruptEventPending(INTERRUPT_EVENT_OVERRUN_ERROR))
    {
        byte = readByte();
        byte = mySpi->SR;

        int a = 0;
    }
}

//------------------------------------------------------------------------------
void SpiSTM32F30x::transmitDmaTransferCompleteCallback()
{
    if (myTransfer->receiveCount == 0)
    {
        myState = STATE_IDLE;
    }
}

//------------------------------------------------------------------------------
void SpiSTM32F30x::receiveDmaTransferCompleteCallback()
{
    myState = STATE_IDLE;
}

//------------------------------------------------------------------------------
// Interrupt service routines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
extern "C" void SPI1_IRQHandler(void)
{
	interruptObjectMap[SpiSTM32F30x::ID_1]->handler();
}

//------------------------------------------------------------------------------
extern "C" void SPI2_IRQHandler(void)
{
    interruptObjectMap[SpiSTM32F30x::ID_2]->handler();
}

//------------------------------------------------------------------------------
extern "C" void SPI3_IRQHandler(void)
{
    interruptObjectMap[SpiSTM32F30x::ID_3]->handler();
}
