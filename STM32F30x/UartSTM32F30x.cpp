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
/// @file UartSTM32F30x.cpp
/// @author Ben Minerd
/// @date 2/4/2016
/// @brief UartSTM32F30x class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/STM32F30x/UartSTM32F30x.h>
#include <Plat4m_Core/CallbackMethod.h>

using Plat4m::UartSTM32F30x;
using Plat4m::Uart;
using Plat4m::Module;
using Plat4m::ComInterface;
using Plat4m::InterruptSTM32F30x;
using Plat4m::GpioPinSTM32F30x;
using Plat4m::ProcessorSTM32F30x;

//------------------------------------------------------------------------------
// Local variables
//------------------------------------------------------------------------------

static InterruptSTM32F30x* interruptObjectMap[5];

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

// Constants

const ProcessorSTM32F30x::Peripheral UartSTM32F30x::myPeripheralMap[] =
{
    ProcessorSTM32F30x::PERIPHERAL_USART_1, /// ID_1
    ProcessorSTM32F30x::PERIPHERAL_USART_2, /// ID_2
    ProcessorSTM32F30x::PERIPHERAL_USART_3, /// ID_3
    ProcessorSTM32F30x::PERIPHERAL_UART_4,  /// ID_4
    ProcessorSTM32F30x::PERIPHERAL_UART_5   /// ID_5
};

const InterruptSTM32F30x::Id UartSTM32F30x::myInterruptMap[] =
{
    InterruptSTM32F30x::ID_USART_1, /// ID_1
    InterruptSTM32F30x::ID_USART_2, /// ID_2
    InterruptSTM32F30x::ID_USART_3, /// ID_3
    InterruptSTM32F30x::ID_UART_4,  /// ID_4
    InterruptSTM32F30x::ID_UART_5   /// ID_5
};

const uint32_t UartSTM32F30x::myInterruptEventEnableMap[] =
{
    USART_CR1_TXEIE, /// INTERRUPT_EVENT_TRANSMIT_BUFFER_EMPTY
    USART_CR1_TCIE,  /// INTERRUPT_EVENT_TRANSMISSION_COMPLETE
    USART_CR1_RXNEIE /// INTERRUPT_EVENT_RECEIVE_BUFFER_NOT_EMPTY
};

const uint32_t UartSTM32F30x::myInterruptEventPendingMap[] =
{
    USART_ISR_TXE, /// INTERRUPT_EVENT_TRANSMIT_BUFFER_EMPTY
    USART_ISR_TC,  /// INTERRUPT_EVENT_TRANSMISSION_COMPLETE
    USART_ISR_RXNE /// INTERRUPT_EVENT_RECEIVE_BUFFER_NOT_EMPTY
};

// Variables

USART_TypeDef* UartSTM32F30x::myUartMap[] =
{
    USART1, /// ID_1
    USART2, /// ID_2
    USART3, /// ID_3
    UART4,  /// ID_4
    UART5   /// ID_5
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
UartSTM32F30x::UartSTM32F30x(const Id id,
                             const TransferMode transferMode,
                             GpioPinSTM32F30x& transmitReceiveGpioPin) :
    Uart(),
    myId(id),
    myTransferMode(transferMode),
    myUart(myUartMap[id]),
    myTransmitGpioPin(transmitReceiveGpioPin),
    myReceiveGpioPin(0),
    myInterrupt(myInterruptMap[id],
                createCallback(this, &UartSTM32F30x::interruptHandler)),
    myState(STATE_IDLE),
    myTransmitDmaChannel(0),
    myReceiveDmaChannel(0)
{
    initialize();
}

//------------------------------------------------------------------------------
UartSTM32F30x::UartSTM32F30x(const Id id,
                             GpioPinSTM32F30x& transmitGpioPin,
                             GpioPinSTM32F30x& receiveGpioPin) :
    Uart(),
    myId(id),
    myTransferMode(TRANSFER_MODE_FULL_DUPLEX),
    myUart(myUartMap[id]),
    myTransmitGpioPin(transmitGpioPin),
    myReceiveGpioPin(&receiveGpioPin),
    myInterrupt(myInterruptMap[id],
                createCallback(this, &UartSTM32F30x::interruptHandler)),
    myState(STATE_IDLE),
    myTransmitDmaChannel(0),
    myReceiveDmaChannel(0)
{
    initialize();
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GpioPinSTM32F30x& UartSTM32F30x::getTransmitGpioPin()
{
	return myTransmitGpioPin;
}

//------------------------------------------------------------------------------
GpioPinSTM32F30x* UartSTM32F30x::getReceiveGpioPin()
{
	return myReceiveGpioPin;
}

//------------------------------------------------------------------------------
InterruptSTM32F30x& UartSTM32F30x::getInterrupt()
{
	return myInterrupt;
}

//------------------------------------------------------------------------------
void UartSTM32F30x::setTransmitDmaChannel(
                                        DmaChannelSTM32F30x& transmitDmaChannel)
{
    myTransmitDmaChannel = &transmitDmaChannel;

    DmaChannelSTM32F30x::Config dmaChannelConfig;
    dmaChannelConfig.dataTransferDirection =
                  DmaChannelSTM32F30x::DATA_TRANSFER_DIRECTION_READ_FROM_MEMORY;
    dmaChannelConfig.circularModeEnabled            = false,
    dmaChannelConfig.peripheralIncrementModeEnabled = false;
    dmaChannelConfig.memoryIncrementModeEnabled     = true;
    dmaChannelConfig.peripheralSize =
                                   DmaChannelSTM32F30x::PERIPHERAL_SIZE_8_BITS;
    dmaChannelConfig.memorySize      = DmaChannelSTM32F30x::MEMORY_SIZE_8_BITS;
    dmaChannelConfig.priorityLevel   = DmaChannelSTM32F30x::PRIORITY_LEVEL_HIGH;
    dmaChannelConfig.memoryToMemoryModeEnabled      = false;
    dmaChannelConfig.nDataToTransfer   = 1;
    dmaChannelConfig.peripheralAddress = (uint32_t) &(myUart->TDR);
    dmaChannelConfig.memoryAddress     =
                                   (uint32_t) (getTransmitBuffer()->getItems());

    myTransmitDmaChannel->setConfig(dmaChannelConfig);

    myTransmitDmaChannel->setTransferCompleteCallback(
			 createCallback(this, &UartSTM32F30x::transmitDmaTransferComplete));

    InterruptSTM32F30x::Config interruptConfig;
    interruptConfig.priority = 7;

    myTransmitDmaChannel->getInterrupt().configure(interruptConfig);

    setTransmitDmaEnabled(true);
}

//------------------------------------------------------------------------------
void UartSTM32F30x::setReceiveDmaChannel(DmaChannelSTM32F30x& receiveDmaChannel)
{
    myReceiveDmaChannel = &receiveDmaChannel;

    DmaChannelSTM32F30x::Config dmaChannelConfig;
    dmaChannelConfig.dataTransferDirection =
              DmaChannelSTM32F30x::DATA_TRANSFER_DIRECTION_READ_FROM_PERIPHERAL;
    dmaChannelConfig.circularModeEnabled            = true,
    dmaChannelConfig.peripheralIncrementModeEnabled = false;
    dmaChannelConfig.memoryIncrementModeEnabled     = true;
    dmaChannelConfig.peripheralSize =
                                    DmaChannelSTM32F30x::PERIPHERAL_SIZE_8_BITS;
    dmaChannelConfig.memorySize      = DmaChannelSTM32F30x::MEMORY_SIZE_8_BITS;
    dmaChannelConfig.priorityLevel   = DmaChannelSTM32F30x::PRIORITY_LEVEL_HIGH;
    dmaChannelConfig.memoryToMemoryModeEnabled      = false;
    dmaChannelConfig.nDataToTransfer   = getReceiveBuffer()->getSize();
    dmaChannelConfig.peripheralAddress = (uint32_t) &(myUart->RDR);
    dmaChannelConfig.memoryAddress     =
    							    (uint32_t) (getReceiveBuffer()->getItems());

    myReceiveDmaChannel->setConfig(dmaChannelConfig);

    setReceiveDmaEnabled(true);

    myReceiveDmaChannel->setEnabledPrivate(true);
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error UartSTM32F30x::driverSetEnabled(const bool enabled)
{
    myTransmitGpioPin.setEnabled(enabled);
    
    if (isValidPointer(myReceiveGpioPin))
    {
        myReceiveGpioPin->setEnabled(enabled);
    }

    ProcessorSTM32F30x::setPeripheralClockEnabled(myPeripheralMap[myId],
                                                  enabled);
    
    if (enabled)
    {
        if (myTransferMode == TRANSFER_MODE_HALF_DUPLEX)
        {
            GpioPin::Config gpioPinConfig;
            gpioPinConfig.mode     = GpioPin::MODE_ALTERNATE_FUNCTION;
            gpioPinConfig.resistor = GpioPin::RESISTOR_PULL_UP;

            myTransmitGpioPin.configure(gpioPinConfig);

            myTransmitGpioPin.setOutputType(
                                      GpioPinSTM32F30x::OUTPUT_TYPE_OPEN_DRAIN);

            setBits(myUart->CR3, USART_CR3_HDSEL);
        }
        else
        {
            GpioPin::Config gpioPinConfig;
            gpioPinConfig.mode     = GpioPin::MODE_ALTERNATE_FUNCTION;
            gpioPinConfig.resistor = GpioPin::RESISTOR_PULL_UP;

            myTransmitGpioPin.configure(gpioPinConfig);
        }
    }

	setInterruptEventEnabled(INTERRUPT_EVENT_RECEIVE_BUFFER_NOT_EMPTY,
							 true);

    setUsartEnabled(enabled);

    InterruptSTM32F30x::Config interruptConfig;
    interruptConfig.priority = 7;
//    interruptConfig.priority = 6;
    myInterrupt.configure(interruptConfig);

	myInterrupt.setEnabled(enabled);

    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods implemented from Uart
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Uart::Error UartSTM32F30x::driverSetConfig(const Config& config)
{
    setBaudRate(config.baudRate);
    setWordBits(config.wordBits);
    setStopBits(config.stopBits);
    setParity(config.parity);
    setHardwareFlowControl(config.hardwareFlowControl);
    setTransferMode(myTransferMode);

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
ComInterface::Error UartSTM32F30x::driverTransmitBytes(
                                                     const ByteArray& byteArray,
                                                     const bool waitUntilDone)
{
    ComInterface::Error error;

    uint32_t i = 0;
    uint32_t nBytes = byteArray.getSize();

    if (nBytes == 0)
    {
    	return ComInterface::Error(ComInterface::ERROR_CODE_NONE);
    }

    if (myTransferMode == TRANSFER_MODE_HALF_DUPLEX)
    {
        myTransmitGpioPin.setOutputType(
                                       GpioPinSTM32F30x::OUTPUT_TYPE_PUSH_PULL);

        setInterruptEventEnabled(INTERRUPT_EVENT_RECEIVE_BUFFER_NOT_EMPTY,
        						 false);
    }

    if (isTransmitDmaEnabled())
    {
        while ((myState != STATE_IDLE) ||
        		!isInterruptEventPending(INTERRUPT_EVENT_TRANSMISSION_COMPLETE))
        {
        }

        getTransmitBuffer()->clear();
        getTransmitBuffer()->write(byteArray);

        myTransmitDmaChannel->setEnabledPrivate(false);
        myTransmitDmaChannel->setNDataToTransfer(nBytes);
        myState = STATE_BUSY;
        myTransmitDmaChannel->setEnabledPrivate(true);
    }
    else
    {
        while (i < nBytes)
        {
            setInterruptEventEnabled(INTERRUPT_EVENT_TRANSMIT_BUFFER_EMPTY,
                                     false);

            if (getTransmitBuffer()->write(byteArray[i]))
            {
                i++;
            }
            else
            {
                error.setCode(ComInterface::ERROR_CODE_TRANSMIT_BUFFER_FULL);
                i = nBytes;
            }

            myState = STATE_BUSY;
            setInterruptEventEnabled(INTERRUPT_EVENT_TRANSMIT_BUFFER_EMPTY,
                                     true);
        }
    }

    if (waitUntilDone)
    {
        while (myState == STATE_BUSY)
        {
        }
    }

    if (myTransferMode == TRANSFER_MODE_HALF_DUPLEX)
    {
        myTransmitGpioPin.setOutputType(
                                      GpioPinSTM32F30x::OUTPUT_TYPE_OPEN_DRAIN);

        setInterruptEventEnabled(INTERRUPT_EVENT_RECEIVE_BUFFER_NOT_EMPTY,
                                 true);
    }

    return error;
}

//------------------------------------------------------------------------------
uint32_t UartSTM32F30x::driverGetReceivedBytesCount()
{
    // TODO: Check to see if receive interrupt is enabled

    setInterruptEventEnabled(INTERRUPT_EVENT_RECEIVE_BUFFER_NOT_EMPTY, false);

    uint32_t nBytes = getReceiveBuffer()->count();

    setInterruptEventEnabled(INTERRUPT_EVENT_RECEIVE_BUFFER_NOT_EMPTY, true);

    return nBytes;
}

//------------------------------------------------------------------------------
ComInterface::Error UartSTM32F30x::driverGetReceivedBytes(ByteArray& byteArray,
                                                          const uint32_t nBytes)
{
    uint8_t byte;

    if (nBytes == 0)
    {
        while (getReceiveBuffer()->read(byte))
        {
        	byteArray.append(byte);
        }
    }
    else
    {
        uint32_t nBytesToRead = nBytes;

        while (nBytesToRead--)
        {
            setInterruptEventEnabled(INTERRUPT_EVENT_RECEIVE_BUFFER_NOT_EMPTY,
                                     false);

            if (getReceiveBuffer()->read(byte))
            {
                byteArray.append(byte);
            }

            setInterruptEventEnabled(INTERRUPT_EVENT_RECEIVE_BUFFER_NOT_EMPTY,
                                     true);
        }
    }

    return ComInterface::Error(ComInterface::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private inline methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void UartSTM32F30x::initialize()
{
    if (isValidPointer(interruptObjectMap[myId]))
    {
        // Instantiating same peripheral twice, lockup
        while (true)
        {
        }
    }

    interruptObjectMap[myId] = &myInterrupt;
}


//------------------------------------------------------------------------------
void UartSTM32F30x::setUsartEnabled(const bool enabled)
{
    setBitsSet(myUart->CR1, USART_CR1_UE, enabled);
}

//------------------------------------------------------------------------------
void UartSTM32F30x::setBaudRate(const uint32_t baudRate)
{
    RealNumber inputClockFrequencyHz =
        (RealNumber)
            ProcessorSTM32F30x::getPeripheralInputClockFrequencyHz(
                                                         myPeripheralMap[myId]);

    RealNumber divider = inputClockFrequencyHz / ((RealNumber) baudRate);

    uint16_t mantissa = (uint16_t) (round(divider));

    clearAndSetBits(myUart->BRR, 0xFFFF, mantissa);
}

//------------------------------------------------------------------------------
void UartSTM32F30x::setWordBits(const WordBits wordBits)
{
    clearAndSetBits(myUart->CR1, USART_CR1_M, ((uint16_t) wordBits) << 12);
}

//------------------------------------------------------------------------------
void UartSTM32F30x::setStopBits(const StopBits stopBits)
{
    switch (stopBits)
    {
        case STOP_BITS_1:
        {
            clearBits(myUart->CR1, USART_CR2_STOP);

            break;
        }
        case STOP_BITS_2:
        {
            clearAndSetBits(myUart->CR1, USART_CR2_STOP, USART_CR2_STOP_1);

            break;
        }
        default:
        {
            break;
        }
    }
}

//------------------------------------------------------------------------------
void UartSTM32F30x::setParity(const Parity parity)
{
    switch (parity)
    {
        case PARITY_NONE:
        {
            clearBits(myUart->CR1, USART_CR1_PCE);

            break;
        }
        case PARITY_EVEN:
        {
            setBits(myUart->CR1, USART_CR1_PCE);
            clearBits(myUart->CR1, USART_CR1_PS);

            break;
        }
        case PARITY_ODD:
        {
            setBits(myUart->CR1, USART_CR1_PCE);
            setBits(myUart->CR1, USART_CR1_PS);

            break;
        }
        default:
        {
            break;
        }
    }
}

//------------------------------------------------------------------------------
void UartSTM32F30x::setHardwareFlowControl(
                                  const HardwareFlowControl hardwareFlowControl)
{
    switch (hardwareFlowControl)
    {
        case HARDWARE_FLOW_CONTROL_NONE:
        {
            clearBits(myUart->CR3, USART_CR3_CTSE);
            clearBits(myUart->CR3, USART_CR3_RTSE);

            break;
        }
        default:
        {
            break;
        }
    }
}

//------------------------------------------------------------------------------
void UartSTM32F30x::setTransferMode(const TransferMode transferMode)
{
    clearBits(myUart->CR1, USART_CR1_TE | USART_CR1_RE);
    clearBits(myUart->CR3, USART_CR3_HDSEL);

    switch (transferMode)
    {
        case TRANSFER_MODE_TRANSMIT_ONLY:
        {
            setBits(myUart->CR1, USART_CR1_TE);

            break;
        }
        case TRANSFER_MODE_RECEIVE_ONLY:
        {
            setBits(myUart->CR1, USART_CR1_RE);

            break;
        }
        case TRANSFER_MODE_HALF_DUPLEX:
        {
            setBits(myUart->CR1, USART_CR1_TE);
            setBits(myUart->CR1, USART_CR1_RE);
            setBits(myUart->CR3, USART_CR3_HDSEL);

            break;
        }
        case TRANSFER_MODE_FULL_DUPLEX:
        {
            setBits(myUart->CR1, USART_CR1_TE);
            setBits(myUart->CR1, USART_CR1_RE);

            break;
        }
        default:
        {
            break;
        }
    }
}

//------------------------------------------------------------------------------
void UartSTM32F30x::writeByte(const uint8_t byte)
{
    myUart->TDR = (uint16_t) byte;
}

//------------------------------------------------------------------------------
uint8_t UartSTM32F30x::readByte()
{
    return ((uint8_t) myUart->RDR);
}

//------------------------------------------------------------------------------
void UartSTM32F30x::setInterruptEventEnabled(const InterruptEvent interruptFlag,
                                             const bool enabled)
{
    setBitsSet(myUart->CR1, myInterruptEventEnableMap[interruptFlag], enabled);
}

//------------------------------------------------------------------------------
bool UartSTM32F30x::isInterruptEventEnabled(const InterruptEvent interruptEvent)
{
    return areBitsSet(myUart->CR1, myInterruptEventEnableMap[interruptEvent]);
}

//------------------------------------------------------------------------------
bool UartSTM32F30x::isInterruptEventPending(const InterruptEvent interruptEvent)
{
    return areBitsSet(myUart->ISR, myInterruptEventPendingMap[interruptEvent]);
}

//------------------------------------------------------------------------------
void UartSTM32F30x::setTransmitDmaEnabled(const bool enabled)
{
    setBitsSet(myUart->CR3, USART_CR3_DMAT, enabled);
}

//------------------------------------------------------------------------------
bool UartSTM32F30x::isTransmitDmaEnabled()
{
    return areBitsSet(myUart->CR3, USART_CR3_DMAT);
}

//------------------------------------------------------------------------------
void UartSTM32F30x::setReceiveDmaEnabled(const bool enabled)
{
    setBitsSet(myUart->CR3, USART_CR3_DMAR, enabled);
}

//------------------------------------------------------------------------------
bool UartSTM32F30x::isReceiveDmaEnabled()
{
    return areBitsSet(myUart->CR3, USART_CR3_DMAR);
}

//------------------------------------------------------------------------------
void UartSTM32F30x::interruptHandler()
{
    if (isInterruptEventEnabled(INTERRUPT_EVENT_TRANSMIT_BUFFER_EMPTY) &&
        isInterruptEventPending(INTERRUPT_EVENT_TRANSMIT_BUFFER_EMPTY))
    {
        uint8_t byte;

        if (getTransmitBuffer()->read(byte))
        {
            writeByte(byte);
        }
        else
        {
            setInterruptEventEnabled(INTERRUPT_EVENT_TRANSMIT_BUFFER_EMPTY,
                                     false);
            myState = STATE_IDLE;
        }
    }

    if (isInterruptEventEnabled(INTERRUPT_EVENT_TRANSMISSION_COMPLETE) &&
        isInterruptEventPending(INTERRUPT_EVENT_TRANSMISSION_COMPLETE))
    {
        setBits(myUart->ICR, USART_ICR_TCCF);
        setInterruptEventEnabled(INTERRUPT_EVENT_TRANSMISSION_COMPLETE, false);
        myState = STATE_IDLE;
    }

    if (isInterruptEventEnabled(INTERRUPT_EVENT_RECEIVE_BUFFER_NOT_EMPTY))
    {
        // Overrun error interrupt flag
        if (areBitsSet(myUart->ISR, USART_ISR_ORE))
        {
            uint8_t dummy = readByte();
            setBits(myUart->ICR, USART_ICR_ORECF);
        }

        // Receive interrupt flag
        if (isInterruptEventPending(INTERRUPT_EVENT_RECEIVE_BUFFER_NOT_EMPTY))
        {
			uint8_t byte = readByte();

			if (!(getReceiveBuffer()->write(byte)))
			{
				// Buffer overflow
			}

            byteReceived(byte);
        }
        else if (isReceiveDmaEnabled())
    	{
        	uint32_t size = getReceiveBuffer()->getSize();
        	uint32_t writeIndex = size -
        						  myReceiveDmaChannel->getDmaChannel()->CNDTR;
        	getReceiveBuffer()->setWriteIndex(writeIndex);

        	uint32_t readIndex = getReceiveBuffer()->getReadIndex();

        	uint32_t count = 0;

        	if (writeIndex >= readIndex)
        	{
        		count = writeIndex - readIndex;
        	}
        	else
        	{
        		count = writeIndex + (size - readIndex);
        	}

    		getReceiveBuffer()->setCount(count);

    		uint8_t byte;

    		while (getReceiveBuffer()->read(byte))
    		{
    			byteReceived(byte);
    		}
    	}
    }
}

//------------------------------------------------------------------------------
void UartSTM32F30x::transmitDmaTransferComplete()
{
	myState = STATE_IDLE;
}

//------------------------------------------------------------------------------
// Interrupt service routines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
extern "C" void USART1_IRQHandler(void)
{
    interruptObjectMap[UartSTM32F30x::ID_1]->handler();
}

//------------------------------------------------------------------------------
extern "C" void USART2_IRQHandler(void)
{
    interruptObjectMap[UartSTM32F30x::ID_2]->handler();
}

//------------------------------------------------------------------------------
extern "C" void USART3_IRQHandler(void)
{
    interruptObjectMap[UartSTM32F30x::ID_3]->handler();
}

//------------------------------------------------------------------------------
extern "C" void UART4_IRQHandler(void)
{
    interruptObjectMap[UartSTM32F30x::ID_4]->handler();
}

//------------------------------------------------------------------------------
extern "C" void UART5_IRQHandler(void)
{
    interruptObjectMap[UartSTM32F30x::ID_5]->handler();
}
