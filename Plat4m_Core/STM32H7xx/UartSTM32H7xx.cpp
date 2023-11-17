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
// Copyright (c) 2022-2023 Benjamin Minerd
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
/// @file UartSTM32H7xx.cpp
/// @author Ben Minerd
/// @date 6/27/2022
/// @brief UartSTM32H7xx class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/STM32H7xx/UartSTM32H7xx.h>
#include <Plat4m_Core/CallbackMethod.h>

using namespace Plat4m;

//------------------------------------------------------------------------------
// Local variables
//------------------------------------------------------------------------------

static InterruptSTM32H7xx* interruptObjectMap[8];

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

// Constants

const ProcessorSTM32H7xx::Peripheral UartSTM32H7xx::myPeripheralMap[] =
{
    ProcessorSTM32H7xx::PERIPHERAL_USART_1, /// ID_1
    ProcessorSTM32H7xx::PERIPHERAL_USART_2, /// ID_2
    ProcessorSTM32H7xx::PERIPHERAL_USART_3, /// ID_3
    ProcessorSTM32H7xx::PERIPHERAL_UART_4,  /// ID_4
    ProcessorSTM32H7xx::PERIPHERAL_UART_5,  /// ID_5
    ProcessorSTM32H7xx::PERIPHERAL_USART_6, /// ID_6
    ProcessorSTM32H7xx::PERIPHERAL_UART_7,  /// ID_7
    ProcessorSTM32H7xx::PERIPHERAL_UART_8   /// ID_8
};

const InterruptSTM32H7xx::Id UartSTM32H7xx::myInterruptMap[] =
{
    InterruptSTM32H7xx::ID_USART_1, /// ID_1
    InterruptSTM32H7xx::ID_USART_2, /// ID_2
    InterruptSTM32H7xx::ID_USART_3, /// ID_3
    InterruptSTM32H7xx::ID_UART_4,  /// ID_4
    InterruptSTM32H7xx::ID_UART_5,  /// ID_5
    InterruptSTM32H7xx::ID_USART_6, /// ID_6
    InterruptSTM32H7xx::ID_UART_7,  /// ID_7
    InterruptSTM32H7xx::ID_UART_8   /// ID_8
};

const GpioPinSTM32H7xx::AlternateFunction
                                       UartSTM32H7xx::myAlternateFunctionMap[] =
{
    GpioPinSTM32H7xx::ALTERNATE_FUNCTION_USART_1, /// ID_1
    GpioPinSTM32H7xx::ALTERNATE_FUNCTION_USART_2, /// ID_2
    GpioPinSTM32H7xx::ALTERNATE_FUNCTION_USART_3, /// ID_3
    GpioPinSTM32H7xx::ALTERNATE_FUNCTION_UART_4,  /// ID_4
    GpioPinSTM32H7xx::ALTERNATE_FUNCTION_UART_5,  /// ID_5
    GpioPinSTM32H7xx::ALTERNATE_FUNCTION_USART_6, /// ID_6
    GpioPinSTM32H7xx::ALTERNATE_FUNCTION_UART_7,  /// ID_7
    GpioPinSTM32H7xx::ALTERNATE_FUNCTION_UART_8   /// ID_8
};

const std::uint32_t UartSTM32H7xx::myInterruptEventEnableMap[] =
{
    Registers::CR1::TXFNEIE, /// INTERRUPT_EVENT_TRANSMIT_FIFO_EMPTY
    Registers::CR1::TCIE,    /// INTERRUPT_EVENT_TRANSMISSION_COMPLETE
    Registers::CR1::RXFNEIE  /// INTERRUPT_EVENT_RECEIVE_FIFO_NOT_EMPTY
};

const std::uint32_t UartSTM32H7xx::myInterruptEventPendingMap[] =
{
    Registers::ISR::TXFNF, /// INTERRUPT_EVENT_TRANSMIT_FIFO_EMPTY
    Registers::ISR::TC,   /// INTERRUPT_EVENT_TRANSMISSION_COMPLETE
    Registers::ISR::RXFNE /// INTERRUPT_EVENT_RECEIVE_FIFO_NOT_EMPTY
};

// Variables

USART_TypeDef* UartSTM32H7xx::myUartMap[] =
{
    USART1, /// ID_1
    USART2, /// ID_2
    USART3, /// ID_3
    UART4,  /// ID_4
    UART5,  /// ID_5
    USART6, /// ID_6
    UART7,  /// ID_7
    UART8   /// ID_8
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
UartSTM32H7xx::UartSTM32H7xx(const Id id,
                             const TransferMode transferMode,
                             GpioPinSTM32H7xx& transmitReceiveGpioPin) :
    Uart(),
    myId(id),
    myTransferMode(transferMode),
    myUart(myUartMap[id]),
    myTransmitGpioPin(transmitReceiveGpioPin),
    myReceiveGpioPin(0),
    myInterrupt(myInterruptMap[id],
                createCallback(this, &UartSTM32H7xx::interruptHandler)),
    myState(STATE_IDLE)
{
    initialize();
}

//------------------------------------------------------------------------------
UartSTM32H7xx::UartSTM32H7xx(const Id id,
                             GpioPinSTM32H7xx& transmitGpioPin,
                             GpioPinSTM32H7xx& receiveGpioPin) :
    Uart(),
    myId(id),
    myTransferMode(TRANSFER_MODE_FULL_DUPLEX),
    myUart(myUartMap[id]),
    myTransmitGpioPin(transmitGpioPin),
    myReceiveGpioPin(&receiveGpioPin),
    myInterrupt(myInterruptMap[id],
                createCallback(this, &UartSTM32H7xx::interruptHandler)),
    myState(STATE_IDLE)
{
    initialize();
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GpioPinSTM32H7xx& UartSTM32H7xx::getTransmitGpioPin()
{
	return myTransmitGpioPin;
}

//------------------------------------------------------------------------------
GpioPinSTM32H7xx* UartSTM32H7xx::getReceiveGpioPin()
{
	return myReceiveGpioPin;
}

//------------------------------------------------------------------------------
InterruptSTM32H7xx& UartSTM32H7xx::getInterrupt()
{
	return myInterrupt;
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error UartSTM32H7xx::driverSetEnabled(const bool enabled)
{
    myTransmitGpioPin.setEnabled(enabled);

    if (isValidPointer(myReceiveGpioPin))
    {
        myReceiveGpioPin->setEnabled(enabled);
    }

    ProcessorSTM32H7xx::setPeripheralClockEnabled(myPeripheralMap[myId],
                                                  enabled);

    if (enabled)
    {
        GpioPin::Config gpioPinConfig;
        gpioPinConfig.mode     = GpioPin::MODE_ALTERNATE_FUNCTION;
        gpioPinConfig.resistor = GpioPin::RESISTOR_PULL_UP;

        myTransmitGpioPin.configure(gpioPinConfig);

        GpioPinSTM32H7xx::STM32H7xxConfig config;
        config.alternateFunction = myAlternateFunctionMap[myId];
        config.outputSpeed = GpioPinSTM32H7xx::OUTPUT_SPEED_HIGH;

        myTransmitGpioPin.setSTM32H7xxConfig(config);

        if (myTransferMode == TRANSFER_MODE_HALF_DUPLEX)
        {
            myTransmitGpioPin.setOutputType(
                                      GpioPinSTM32H7xx::OUTPUT_TYPE_OPEN_DRAIN);

            setBits(myUart->CR3, Registers::CR3::HDSEL);
        }
        else
        {
            if (isValidPointer(myReceiveGpioPin))
            {
                myReceiveGpioPin->configure(gpioPinConfig);
                myReceiveGpioPin->setSTM32H7xxConfig(config);
            }
        }
    }

    setInterruptEventEnabled(INTERRUPT_EVENT_RECEIVE_FIFO_NOT_EMPTY, true);

    setUsartEnabled(enabled);

    InterruptSTM32H7xx::Config interruptConfig;
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
Uart::Error UartSTM32H7xx::driverSetConfig(const Config& config)
{
    setBaudRate(config.baudRate);
    setWordBits(config.wordBits);
    setStopBits(config.stopBits);
    setParity(config.parity);
    setHardwareFlowControl(config.hardwareFlowControl);
    setTransferMode(myTransferMode);

    return Uart::Error(Uart::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
ComInterface::Error UartSTM32H7xx::driverTransmitBytes(
                                                     const ByteArray& byteArray,
                                                     const bool waitUntilDone)
{
    ComInterface::Error error;

    std::uint32_t i = 0;
    std::uint32_t nBytes = byteArray.getSize();

    if (nBytes == 0)
    {
        return ComInterface::Error(ComInterface::ERROR_CODE_NONE);
    }

    if (myTransferMode == TRANSFER_MODE_HALF_DUPLEX)
    {
        myTransmitGpioPin.setOutputType(
                                       GpioPinSTM32H7xx::OUTPUT_TYPE_PUSH_PULL);

        setInterruptEventEnabled(INTERRUPT_EVENT_RECEIVE_FIFO_NOT_EMPTY,
                                 false);
    }

    while (i < nBytes)
    {
        setInterruptEventEnabled(INTERRUPT_EVENT_TRANSMIT_FIFO_EMPTY, false);

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
        setInterruptEventEnabled(INTERRUPT_EVENT_TRANSMIT_FIFO_EMPTY, true);
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
                                      GpioPinSTM32H7xx::OUTPUT_TYPE_OPEN_DRAIN);

        setInterruptEventEnabled(INTERRUPT_EVENT_RECEIVE_FIFO_NOT_EMPTY,
                                 true);
    }

    return error;
}

//------------------------------------------------------------------------------
std::uint32_t UartSTM32H7xx::driverGetReceivedBytesCount()
{
    // TODO: Check to see if receive interrupt is enabled

    setInterruptEventEnabled(INTERRUPT_EVENT_RECEIVE_FIFO_NOT_EMPTY, false);

    std::uint32_t nBytes = getReceiveBuffer()->count();

    setInterruptEventEnabled(INTERRUPT_EVENT_RECEIVE_FIFO_NOT_EMPTY, true);

    return nBytes;
}

//------------------------------------------------------------------------------
ComInterface::Error UartSTM32H7xx::driverGetReceivedBytes(
                                                     ByteArray& byteArray,
                                                     const std::uint32_t nBytes)
{
    std::uint8_t byte;
    std::uint32_t nBytesToRead;

    if (nBytes == 0)
    {
        nBytesToRead = driverGetReceivedBytesCount();
    }
    else
    {
        nBytesToRead = nBytes;
    }

    while (nBytesToRead--)
    {
        setInterruptEventEnabled(INTERRUPT_EVENT_RECEIVE_FIFO_NOT_EMPTY, false);

        if (getReceiveBuffer()->read(byte))
        {
            byteArray.append(byte);
        }

        setInterruptEventEnabled(INTERRUPT_EVENT_RECEIVE_FIFO_NOT_EMPTY, true);
    }

    return ComInterface::Error(ComInterface::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private inline methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void UartSTM32H7xx::initialize()
{
    if (isValidPointer(interruptObjectMap[myId]))
    {
        Error(ERROR_CODE_DUPLICATE_PERIPHERAL_INSTANTIATION);

        // Instantiating same peripheral twice, lockup
        while (true)
        {
        }
    }

    interruptObjectMap[myId] = &myInterrupt;
}

//------------------------------------------------------------------------------
void UartSTM32H7xx::setUsartEnabled(const bool enabled)
{
    setBitsSet(myUart->CR1, Registers::CR1::UE, enabled);
}

//------------------------------------------------------------------------------
void UartSTM32H7xx::setBaudRate(const std::uint32_t baudRate)
{
    float inputClockFrequencyHz =
        static_cast<float>(
            ProcessorSTM32H7xx::getPeripheralInputClockFrequencyHz(
                                                        myPeripheralMap[myId]));

    float divider = inputClockFrequencyHz / static_cast<float>(baudRate);

    std::uint16_t mantissa = static_cast<std::uint16_t>(round(divider));

    clearAndSetBits(myUart->BRR, Registers::BRR, mantissa);
}

//------------------------------------------------------------------------------
void UartSTM32H7xx::setWordBits(const WordBits wordBits)
{
    setBitsSet(myUart->CR1, Registers::CR1::M0, isBitSet(wordBits, 0));
    setBitsSet(myUart->CR1, Registers::CR1::M1, isBitSet(wordBits, 1));
}

//------------------------------------------------------------------------------
void UartSTM32H7xx::setStopBits(const StopBits stopBits)
{
    switch (stopBits)
    {
        case STOP_BITS_1:
        {
            clearBits(myUart->CR2, Registers::CR2::STOP);

            break;
        }
        case STOP_BITS_2:
        {
            clearAndSetBits(myUart->CR2, Registers::CR2::STOP, 2 << 12);

            break;
        }
        default:
        {
            break;
        }
    }
}

//------------------------------------------------------------------------------
void UartSTM32H7xx::setParity(const Parity parity)
{
    switch (parity)
    {
        case PARITY_NONE:
        {
            clearBits(myUart->CR1, Registers::CR1::PCE);

            break;
        }
        case PARITY_EVEN:
        {
            setBit(myUart->CR1, 10);
            clearBit(myUart->CR1, 9);

            break;
        }
        case PARITY_ODD:
        {
            setBit(myUart->CR1, 10);
            setBit(myUart->CR1, 9);

            break;
        }
        default:
        {
            break;
        }
    }
}

//------------------------------------------------------------------------------
void UartSTM32H7xx::setHardwareFlowControl(
                                  const HardwareFlowControl hardwareFlowControl)
{
    switch (hardwareFlowControl)
    {
        case HARDWARE_FLOW_CONTROL_NONE:
        {
            clearBits(myUart->CR3, Registers::CR3::RTSE);
            clearBits(myUart->CR3, Registers::CR3::CTSE);

            break;
        }
        default:
        {
            break;
        }
    }
}

//------------------------------------------------------------------------------
void UartSTM32H7xx::setTransferMode(const TransferMode transferMode)
{
    clearBits(myUart->CR1, Registers::CR1::TE);
    clearBits(myUart->CR1, Registers::CR1::RE);
    clearBits(myUart->CR3, Registers::CR3::HDSEL);

    switch (transferMode)
    {
        case TRANSFER_MODE_TRANSMIT_ONLY:
        {
            setBits(myUart->CR1, Registers::CR1::TE);

            break;
        }
        case TRANSFER_MODE_RECEIVE_ONLY:
        {
            setBits(myUart->CR1, Registers::CR1::RE);

            break;
        }
        case TRANSFER_MODE_HALF_DUPLEX:
        {
            setBits(myUart->CR1, Registers::CR1::TE);
            setBits(myUart->CR1, Registers::CR1::RE);
            setBits(myUart->CR3, Registers::CR3::HDSEL);

            break;
        }
        case TRANSFER_MODE_FULL_DUPLEX:
        {
            setBits(myUart->CR1, Registers::CR1::TE);
            setBits(myUart->CR1, Registers::CR1::RE);

            break;
        }
        default:
        {
            break;
        }
    }
}

//------------------------------------------------------------------------------
void UartSTM32H7xx::writeByte(const std::uint8_t byte)
{
    myUart->TDR = static_cast<std::uint16_t>(byte);
}

//------------------------------------------------------------------------------
uint8_t UartSTM32H7xx::readByte()
{
    return (static_cast<std::uint8_t>(myUart->RDR));
}

//------------------------------------------------------------------------------
void UartSTM32H7xx::setInterruptEventEnabled(const InterruptEvent interruptFlag,
                                             const bool enabled)
{
    setBitsSet(myUart->CR1, myInterruptEventEnableMap[interruptFlag], enabled);
}

//------------------------------------------------------------------------------
bool UartSTM32H7xx::isInterruptEventEnabled(const InterruptEvent interruptEvent)
{
    return areBitsSet(myUart->CR1, myInterruptEventEnableMap[interruptEvent]);
}

//------------------------------------------------------------------------------
bool UartSTM32H7xx::isInterruptEventPending(const InterruptEvent interruptEvent)
{
    return areBitsSet(myUart->ISR, myInterruptEventPendingMap[interruptEvent]);
}

//------------------------------------------------------------------------------
void UartSTM32H7xx::setTransmitDmaEnabled(const bool enabled)
{
    setBitsSet(myUart->CR3, Registers::CR3::DMAT, enabled);
}

//------------------------------------------------------------------------------
bool UartSTM32H7xx::isTransmitDmaEnabled()
{
    return areBitsSet(myUart->CR3, Registers::CR3::DMAT);
}

//------------------------------------------------------------------------------
void UartSTM32H7xx::setReceiveDmaEnabled(const bool enabled)
{
    setBitsSet(myUart->CR3, Registers::CR3::DMAR, enabled);
}

//------------------------------------------------------------------------------
bool UartSTM32H7xx::isReceiveDmaEnabled()
{
    return areBitsSet(myUart->CR3, Registers::CR3::DMAR);
}

//------------------------------------------------------------------------------
void UartSTM32H7xx::interruptHandler()
{
    if (isInterruptEventEnabled(INTERRUPT_EVENT_TRANSMIT_FIFO_EMPTY) &&
        isInterruptEventPending(INTERRUPT_EVENT_TRANSMIT_FIFO_EMPTY))
    {
        std::uint8_t byte;

        if (getTransmitBuffer()->read(byte))
        {
            writeByte(byte);
        }
        else
        {
            setInterruptEventEnabled(INTERRUPT_EVENT_TRANSMIT_FIFO_EMPTY,
                                     false);
            myState = STATE_IDLE;
        }
    }

    if (isInterruptEventEnabled(INTERRUPT_EVENT_TRANSMISSION_COMPLETE) &&
        isInterruptEventPending(INTERRUPT_EVENT_TRANSMISSION_COMPLETE))
    {
        setBits(myUart->ICR, Registers::ICR::TCCF);
        setInterruptEventEnabled(INTERRUPT_EVENT_TRANSMISSION_COMPLETE, false);
        myState = STATE_IDLE;
    }

    if (isInterruptEventEnabled(INTERRUPT_EVENT_RECEIVE_FIFO_NOT_EMPTY))
    {
        // Overrun error interrupt flag
        if (areBitsSet(myUart->ISR, Registers::ISR::ORE))
        {
            std::uint8_t dummy = readByte();
            setBits(myUart->ICR, Registers::ICR::OREFC);
        }

        // Receive interrupt flag
        if (isInterruptEventPending(INTERRUPT_EVENT_RECEIVE_FIFO_NOT_EMPTY))
        {
            std::uint8_t byte = readByte();

            if (!(getReceiveBuffer()->write(byte)))
            {
                // Buffer overflow
            }

            byteReceived(byte);
        }
    }
}

//------------------------------------------------------------------------------
// Interrupt service routines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
extern "C" void USART1_IRQHandler(void)
{
    interruptObjectMap[UartSTM32H7xx::ID_1]->handler();
}

//------------------------------------------------------------------------------
extern "C" void USART2_IRQHandler(void)
{
    interruptObjectMap[UartSTM32H7xx::ID_2]->handler();
}

//------------------------------------------------------------------------------
extern "C" void USART3_IRQHandler(void)
{
    interruptObjectMap[UartSTM32H7xx::ID_3]->handler();
}

//------------------------------------------------------------------------------
extern "C" void UART4_IRQHandler(void)
{
    interruptObjectMap[UartSTM32H7xx::ID_4]->handler();
}

//------------------------------------------------------------------------------
extern "C" void UART5_IRQHandler(void)
{
    interruptObjectMap[UartSTM32H7xx::ID_5]->handler();
}

//------------------------------------------------------------------------------
extern "C" void USART6_IRQHandler(void)
{
    interruptObjectMap[UartSTM32H7xx::ID_6]->handler();
}

//------------------------------------------------------------------------------
extern "C" void UART7_IRQHandler(void)
{
    interruptObjectMap[UartSTM32H7xx::ID_7]->handler();
}

//------------------------------------------------------------------------------
extern "C" void UART8_IRQHandler(void)
{
    interruptObjectMap[UartSTM32H7xx::ID_8]->handler();
}
