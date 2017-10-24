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
/// @file UartSTM32F10x.cpp
/// @author Ben Minerd
/// @date 12/16/2016
/// @brief UartSTM32F10x class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <UartSTM32F10x.h>
#include <CallbackMethod.h>

#include <math.h>

using Plat4m::UartSTM32F10x;
using Plat4m::Uart;
using Plat4m::Module;
using Plat4m::ComInterface;
using Plat4m::InterruptSTM32F10x;
using Plat4m::GpioPinSTM32F10x;
using Plat4m::ProcessorSTM32F10x;

//------------------------------------------------------------------------------
// Local variables
//------------------------------------------------------------------------------

static InterruptSTM32F10x* interruptObjectMap[5];

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

// Constants

const ProcessorSTM32F10x::Peripheral UartSTM32F10x::myPeripheralMap[] =
{
    ProcessorSTM32F10x::PERIPHERAL_USART_1, /// ID_1
    ProcessorSTM32F10x::PERIPHERAL_USART_2, /// ID_2
    ProcessorSTM32F10x::PERIPHERAL_USART_3, /// ID_3
    ProcessorSTM32F10x::PERIPHERAL_UART_4,  /// ID_4
    ProcessorSTM32F10x::PERIPHERAL_UART_5   /// ID_5
};

const InterruptSTM32F10x::Id UartSTM32F10x::myInterruptMap[] =
{
    InterruptSTM32F10x::ID_USART_1, /// ID_1
    InterruptSTM32F10x::ID_USART_2, /// ID_2
    InterruptSTM32F10x::ID_USART_3, /// ID_3
    InterruptSTM32F10x::ID_UART_4,  /// ID_4
    InterruptSTM32F10x::ID_UART_5   /// ID_5
};

const uint32_t UartSTM32F10x::myInterruptEventMap[] =
{
    USART_CR1_RXNEIE, /// INTERRUPT_EVENT_RECEIVE_BUFFER_NOT_EMPTY
    USART_CR1_TCIE,   /// INTERRUPT_EVENT_TRANSMISSION_COMPLETE
    USART_CR1_TXEIE   /// INTERRUPT_EVENT_TRANSMIT_BUFFER_EMPTY
};

const GpioPinSTM32F10x::OutputSpeed UartSTM32F10x::myDefaultOutputSpeed =
                                           GpioPinSTM32F10x::OUTPUT_SPEED_50MHZ;

// Variables

USART_TypeDef* UartSTM32F10x::myUartMap[] =
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
UartSTM32F10x::UartSTM32F10x(const Id id,
                             const TransferMode transferMode,
                             GpioPinSTM32F10x& transmitReceiveGpioPin) :
    Uart(),
    myId(id),
    myTransferMode(transferMode),
    myUart(myUartMap[id]),
    myTransmitGpioPin(transmitReceiveGpioPin),
    myReceiveGpioPin(0),
    myInterrupt(myInterruptMap[id],
                createCallback(this, &UartSTM32F10x::interruptHandler)),
    myState(STATE_IDLE),
    myTransmitDmaChannel(0),
    myReceiveDmaChannel(0),
    myDmaBuffer()
{
    initialize();
}

//------------------------------------------------------------------------------
UartSTM32F10x::UartSTM32F10x(const Id id,
                             GpioPinSTM32F10x& transmitGpioPin,
                             GpioPinSTM32F10x& receiveGpioPin) :
    Uart(),
    myId(id),
    myTransferMode(TRANSFER_MODE_FULL_DUPLEX),
    myUart(myUartMap[id]),
    myTransmitGpioPin(transmitGpioPin),
    myReceiveGpioPin(&receiveGpioPin),
    myInterrupt(myInterruptMap[id],
                createCallback(this, &UartSTM32F10x::interruptHandler)),
    myState(STATE_IDLE),
    myTransmitDmaChannel(0),
    myReceiveDmaChannel(0),
    myDmaBuffer()
{
    initialize();
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
UartSTM32F10x::~UartSTM32F10x()
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void UartSTM32F10x::setTransmitDmaChannel(
                                        DmaChannelSTM32F10x& transmitDmaChannel)
{
    myTransmitDmaChannel = &transmitDmaChannel;

    DmaChannelSTM32F10x::Config dmaChannelConfig;
    dmaChannelConfig.dataTransferDirection =
                  DmaChannelSTM32F10x::DATA_TRANSFER_DIRECTION_READ_FROM_MEMORY;
    dmaChannelConfig.circularModeEnabled            = false,
    dmaChannelConfig.peripheralIncrementModeEnabled = false;
    dmaChannelConfig.memoryIncrementModeEnabled     = true;
    dmaChannelConfig.peripheralSize =
                                   DmaChannelSTM32F10x::PERIPHERAL_SIZE_8_BITS;
    dmaChannelConfig.memorySize      = DmaChannelSTM32F10x::MEMORY_SIZE_8_BITS;
    dmaChannelConfig.priorityLevel   = DmaChannelSTM32F10x::PRIORITY_LEVEL_HIGH;
    dmaChannelConfig.memoryToMemoryModeEnabled      = false;
    dmaChannelConfig.nDataToTransfer   = 1;
    dmaChannelConfig.peripheralAddress = (uint32_t) &(myUart->DR);
    dmaChannelConfig.memoryAddress     =
                                   (uint32_t) (getTransmitBuffer()->getItems());

    myTransmitDmaChannel->setConfig(dmaChannelConfig);

    InterruptSTM32F10x::Config interruptConfig;
    interruptConfig.priority = 6;

    myTransmitDmaChannel->getInterrupt().configure(interruptConfig);

    setTransmitDmaEnabled(true);
}

//------------------------------------------------------------------------------
void UartSTM32F10x::setReceiveDmaChannel(DmaChannelSTM32F10x& receiveDmaChannel)
{
    myReceiveDmaChannel = &receiveDmaChannel;

    DmaChannelSTM32F10x::Config dmaChannelConfig;
    dmaChannelConfig.dataTransferDirection =
              DmaChannelSTM32F10x::DATA_TRANSFER_DIRECTION_READ_FROM_PERIPHERAL;
    dmaChannelConfig.circularModeEnabled            = true,
    dmaChannelConfig.peripheralIncrementModeEnabled = false;
    dmaChannelConfig.memoryIncrementModeEnabled     = true;
    dmaChannelConfig.peripheralSize =
                                    DmaChannelSTM32F10x::PERIPHERAL_SIZE_8_BITS;
    dmaChannelConfig.memorySize      = DmaChannelSTM32F10x::MEMORY_SIZE_8_BITS;
    dmaChannelConfig.priorityLevel   = DmaChannelSTM32F10x::PRIORITY_LEVEL_HIGH;
    dmaChannelConfig.memoryToMemoryModeEnabled      = false;
    dmaChannelConfig.nDataToTransfer   = 2;
    dmaChannelConfig.peripheralAddress = (uint32_t) &(myUart->DR);
    dmaChannelConfig.memoryAddress     = (uint32_t) &myDmaBuffer;

    myReceiveDmaChannel->setConfig(dmaChannelConfig);

    myReceiveDmaChannel->setHalfTransferCallback(
                        createCallback(this, &UartSTM32F10x::dmaByte1Received));
    myReceiveDmaChannel->setTransferCompleteCallback(
                        createCallback(this, &UartSTM32F10x::dmaByte2Received));

    setInterruptEventEnabled(INTERRUPT_EVENT_RECEIVE_BUFFER_NOT_EMPTY, false);
    setReceiveDmaEnabled(true);

    InterruptSTM32F10x::Config interruptConfig;
    interruptConfig.priority = 6;

    myReceiveDmaChannel->getInterrupt().configure(interruptConfig);

    myReceiveDmaChannel->setEnabledPrivate(true);
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error UartSTM32F10x::driverSetEnabled(const bool enabled)
{
    ProcessorSTM32F10x::setPeripheralClockEnabled(myPeripheralMap[myId],
                                                  enabled);

    myTransmitGpioPin.setEnabled(enabled);
    
    if (isValidPointer(myReceiveGpioPin))
    {
        myReceiveGpioPin->setEnabled(enabled);
    }

    if (enabled)
    {

        if (myTransferMode == TRANSFER_MODE_HALF_DUPLEX)
        {
            GpioPin::Config gpioPinConfig;
            gpioPinConfig.mode     = GpioPin::MODE_DIGITAL_INPUT;
            gpioPinConfig.resistor = GpioPin::RESISTOR_NONE;

            myTransmitGpioPin.configure(gpioPinConfig);
//
//            GpioPinSTM32F10x::STM32F10xConfig gpioPinSTM32F10xConfig;
//            gpioPinSTM32F10xConfig.outputType  =
//                                       GpioPinSTM32F10x::OUTPUT_TYPE_OPEN_DRAIN;
//            gpioPinSTM32F10xConfig.outputSpeed = myDefaultOutputSpeed;
//
//            myTransmitGpioPin.setSTM32F10xConfig(gpioPinSTM32F10xConfig);
        }
        else
        {
            GpioPin::Config gpioPinConfig;
            gpioPinConfig.mode     = GpioPin::MODE_ALTERNATE_FUNCTION;
            gpioPinConfig.resistor = GpioPin::RESISTOR_NONE;
            myTransmitGpioPin.configure(gpioPinConfig);

            gpioPinConfig.mode = GpioPin::MODE_DIGITAL_INPUT;
            myReceiveGpioPin->configure(gpioPinConfig);

            GpioPinSTM32F10x::STM32F10xConfig gpioPinSTM32F10xConfig;
            gpioPinSTM32F10xConfig.outputType  =
                                        GpioPinSTM32F10x::OUTPUT_TYPE_PUSH_PULL;
            gpioPinSTM32F10xConfig.outputSpeed = myDefaultOutputSpeed;

            myTransmitGpioPin.setSTM32F10xConfig(gpioPinSTM32F10xConfig);
        }
    }

    if (!isReceiveDmaEnabled())
    {
        setInterruptEventEnabled(INTERRUPT_EVENT_RECEIVE_BUFFER_NOT_EMPTY,
                                 true);
    }

    setUsartEnabled(enabled);

    if (!(isTransmitDmaEnabled()) || !(isReceiveDmaEnabled()))
    {
        myInterrupt.setEnabled(enabled);
    }

    InterruptSTM32F10x::Config interruptConfig;
    interruptConfig.priority = 6;

    myInterrupt.configure(interruptConfig);

    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods implemented from Uart
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Uart::Error UartSTM32F10x::driverSetConfig(const Config& config)
{
    setBaudRate(config.baudRate);
    setWordBits(config.wordBits);
    setStopBits(config.stopBits);
    setParity(config.parity);
    setHardwareFlowControl(config.hardwareFlowControl);
    setTransferMode(myTransferMode);

    setUsartEnabled(true);

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
ComInterface::Error UartSTM32F10x::driverTransmitBytes(
                                                     const ByteArray& byteArray,
                                                     const bool waitUntilDone)
{
    ComInterface::Error error;

    uint32_t i = 0;
    uint32_t nBytes = byteArray.getSize();

    if (myTransferMode == TRANSFER_MODE_HALF_DUPLEX)
    {
        GpioPin::Config gpioPinConfig;
        gpioPinConfig.mode     = GpioPin::MODE_ALTERNATE_FUNCTION;
        gpioPinConfig.resistor = GpioPin::RESISTOR_PULL_UP;

        myTransmitGpioPin.configure(gpioPinConfig);

        GpioPinSTM32F10x::STM32F10xConfig gpioPinSTM32F10xConfig;
        gpioPinSTM32F10xConfig.outputType  =
                                    GpioPinSTM32F10x::OUTPUT_TYPE_PUSH_PULL;
        gpioPinSTM32F10xConfig.outputSpeed = myDefaultOutputSpeed;

        myTransmitGpioPin.setSTM32F10xConfig(gpioPinSTM32F10xConfig);
    }

    if (isTransmitDmaEnabled())
    {
        while (myState != STATE_IDLE)
        {
        }

        getTransmitBuffer()->clear();
        getTransmitBuffer()->write(byteArray);

        myTransmitDmaChannel->setEnabledPrivate(false);
        myTransmitDmaChannel->setNDataToTransfer(nBytes);
        myState = STATE_BUSY;
        setInterruptEventEnabled(INTERRUPT_EVENT_TRANSMISSION_COMPLETE, true);
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
        while (myState != STATE_IDLE)
        {
        }
    }

    if (myTransferMode == TRANSFER_MODE_HALF_DUPLEX)
    {
        GpioPin::Config gpioPinConfig;
        gpioPinConfig.mode     = GpioPin::MODE_DIGITAL_INPUT;
        gpioPinConfig.resistor = GpioPin::RESISTOR_NONE;

        myTransmitGpioPin.configure(gpioPinConfig);

//        GpioPinSTM32F10x::STM32F10xConfig gpioPinSTM32F10xConfig;
//        gpioPinSTM32F10xConfig.outputType  =
//                                    GpioPinSTM32F10x::OUTPUT_TYPE_OPEN_DRAIN;
//        gpioPinSTM32F10xConfig.outputSpeed = myDefaultOutputSpeed;
//
//        myTransmitGpioPin.setSTM32F10xConfig(gpioPinSTM32F10xConfig);
    }

    return error;
}

//------------------------------------------------------------------------------
uint32_t UartSTM32F10x::driverGetReceivedBytesCount()
{
    // TODO: Check to see if receive interrupt is enabled

    bool wasEnabled =
              isInterruptEventEnabled(INTERRUPT_EVENT_RECEIVE_BUFFER_NOT_EMPTY);

    setInterruptEventEnabled(INTERRUPT_EVENT_RECEIVE_BUFFER_NOT_EMPTY,
                             false);

    uint32_t nBytes = getReceiveBuffer()->count();

    if (wasEnabled)
    {
        setInterruptEventEnabled(INTERRUPT_EVENT_RECEIVE_BUFFER_NOT_EMPTY,
                                 true);
    }

    return nBytes;
}

//------------------------------------------------------------------------------
ComInterface::Error UartSTM32F10x::driverGetReceivedBytes(ByteArray& byteArray,
                                                          const uint32_t nBytes)
{
    uint32_t nBytesToRead;

    if (nBytes == 0)
    {
        nBytesToRead = getReceiveBuffer()->getSize();
    }
    else
    {
        nBytesToRead = nBytes;
    }

    bool wasEnabled =
              isInterruptEventEnabled(INTERRUPT_EVENT_RECEIVE_BUFFER_NOT_EMPTY);

    while (nBytesToRead--)
    {
        setInterruptEventEnabled(INTERRUPT_EVENT_RECEIVE_BUFFER_NOT_EMPTY,
                                 false);

        uint8_t byte;

        if (getReceiveBuffer()->read(byte))
        {
            byteArray.append(byte);
        }

        if (wasEnabled)
        {
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
void UartSTM32F10x::initialize()
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
void UartSTM32F10x::setUsartEnabled(const bool enabled)
{
    setBitsSet(myUart->CR1, USART_CR1_UE, enabled);
}

//------------------------------------------------------------------------------
void UartSTM32F10x::setBaudRate(const uint32_t baudRate)
{
    FloatingPointType inputClockFrequencyHz =
        (FloatingPointType)
            ProcessorSTM32F10x::getPeripheralInputClockFrequencyHz(
                                                         myPeripheralMap[myId]);

    FloatingPointType divider =
                inputClockFrequencyHz / (16.0 * ((FloatingPointType) baudRate));

    uint16_t mantissa = (uint16_t) (floor(divider));
    uint16_t fraction = (uint16_t) round((divider - floor(divider)) * 16.0);

    clearAndSetBits(myUart->BRR, USART_BRR_DIV_Mantissa, mantissa << 4);
    clearAndSetBits(myUart->BRR, USART_BRR_DIV_Fraction, fraction & 0xF);
}

//------------------------------------------------------------------------------
void UartSTM32F10x::setWordBits(const WordBits wordBits)
{
    clearAndSetBits(myUart->CR1, USART_CR1_M, ((uint16_t) wordBits) << 12);
}

//------------------------------------------------------------------------------
void UartSTM32F10x::setStopBits(const StopBits stopBits)
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
void UartSTM32F10x::setParity(const Parity parity)
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
void UartSTM32F10x::setHardwareFlowControl(
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
void UartSTM32F10x::setTransferMode(const TransferMode transferMode)
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
void UartSTM32F10x::writeByte(const uint8_t byte)
{
    myUart->DR = (uint16_t) byte;
}

//------------------------------------------------------------------------------
uint8_t UartSTM32F10x::readByte()
{
    return ((uint8_t) myUart->DR);
}

//------------------------------------------------------------------------------
void UartSTM32F10x::setInterruptEventEnabled(
                                            const InterruptEvent interruptEvent,
                                            const bool enabled)
{
    setBitsSet(myUart->CR1, myInterruptEventMap[interruptEvent], enabled);
}

//------------------------------------------------------------------------------
bool UartSTM32F10x::isInterruptEventEnabled(const InterruptEvent interruptEvent)
{
    return areBitsSet(myUart->CR1, myInterruptEventMap[interruptEvent]);
}

//------------------------------------------------------------------------------
void UartSTM32F10x::setTransmitDmaEnabled(const bool enabled)
{
    setBitsSet(myUart->CR3, USART_CR3_DMAT, enabled);
}

//------------------------------------------------------------------------------
bool UartSTM32F10x::isTransmitDmaEnabled()
{
    return areBitsSet(myUart->CR3, USART_CR3_DMAT);
}

//------------------------------------------------------------------------------
void UartSTM32F10x::setReceiveDmaEnabled(const bool enabled)
{
    setBitsSet(myUart->CR3, USART_CR3_DMAR, enabled);
}

//------------------------------------------------------------------------------
bool UartSTM32F10x::isReceiveDmaEnabled()
{
    return areBitsSet(myUart->CR3, USART_CR3_DMAR);
}

//------------------------------------------------------------------------------
void UartSTM32F10x::interruptHandler()
{
    if (isInterruptEventEnabled(INTERRUPT_EVENT_RECEIVE_BUFFER_NOT_EMPTY))
    {
        // Overrun error interrupt flag
        if (areBitsSet(myUart->SR, USART_SR_ORE))
        {
            uint8_t dummy = readByte();
            // TODO: Set error flag with ComInterface?
        }

        // Receive interrupt flag
        if (areBitsSet(myUart->SR, USART_SR_RXNE))
        {
            uint8_t byte = readByte();

            if (!(getReceiveBuffer()->write(byte)))
            {
                // Buffer overflow
            }

            bytesReceived();
        }
    }

    if (isInterruptEventEnabled(INTERRUPT_EVENT_TRANSMISSION_COMPLETE) &&
        areBitsSet(myUart->SR, USART_SR_TC))
    {
        clearBits(myUart->SR, USART_SR_TC);
        setInterruptEventEnabled(INTERRUPT_EVENT_TRANSMISSION_COMPLETE, false);
        myState = STATE_IDLE;
    }

    if (isInterruptEventEnabled(INTERRUPT_EVENT_TRANSMIT_BUFFER_EMPTY) &&
        areBitsSet(myUart->SR, USART_SR_TXE))
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
}

//------------------------------------------------------------------------------
void UartSTM32F10x::dmaByte1Received()
{
    getReceiveBuffer()->write(myDmaBuffer[0]);
    bytesReceived();
}

//------------------------------------------------------------------------------
void UartSTM32F10x::dmaByte2Received()
{
    getReceiveBuffer()->write(myDmaBuffer[1]);
    bytesReceived();
}

//------------------------------------------------------------------------------
// Interrupt service routines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
extern "C" void USART1_IRQHandler(void)
{
    interruptObjectMap[UartSTM32F10x::ID_1]->handler();
}

//------------------------------------------------------------------------------
extern "C" void USART2_IRQHandler(void)
{
    interruptObjectMap[UartSTM32F10x::ID_2]->handler();
}

//------------------------------------------------------------------------------
extern "C" void USART3_IRQHandler(void)
{
    interruptObjectMap[UartSTM32F10x::ID_3]->handler();
}

//------------------------------------------------------------------------------
extern "C" void UART4_IRQHandler(void)
{
    interruptObjectMap[UartSTM32F10x::ID_4]->handler();
}

//------------------------------------------------------------------------------
extern "C" void UART5_IRQHandler(void)
{
    interruptObjectMap[UartSTM32F10x::ID_5]->handler();
}
