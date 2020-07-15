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
/// @file UartSTM32F4xx.cpp
/// @author Ben Minerd
/// @date 5/14/2013
/// @brief UartSTM32F4xx class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <stm32f4xx_usart.h>

#include <Plat4m_Core/STM32F4xx/UartSTM32F4xx.h>
#include <Plat4m_Core/CallbackMethod.h>

using Plat4m::UartSTM32F4xx;
using Plat4m::Uart;
using Plat4m::Module;
using Plat4m::ComInterface;
using Plat4m::InterruptSTM32F4xx;
using Plat4m::ProcessorSTM32F4xx;

//------------------------------------------------------------------------------
// Local variables
//------------------------------------------------------------------------------

static InterruptSTM32F4xx* interruptObjectMap[6];

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const ProcessorSTM32F4xx::Peripheral UartSTM32F4xx::myPeripheralMap[] =
{
	ProcessorSTM32F4xx::PERIPHERAL_USART_1, /// ID_1
	ProcessorSTM32F4xx::PERIPHERAL_USART_2, /// ID_2
	ProcessorSTM32F4xx::PERIPHERAL_USART_3, /// ID_3
	ProcessorSTM32F4xx::PERIPHERAL_UART_4,  /// ID_4
	ProcessorSTM32F4xx::PERIPHERAL_UART_5,  /// ID_5
	ProcessorSTM32F4xx::PERIPHERAL_USART_6, /// ID_6
};

const InterruptSTM32F4xx::Id UartSTM32F4xx::myInterruptIdMap[] =
{
	InterruptSTM32F4xx::ID_USART_1, /// ID_1
	InterruptSTM32F4xx::ID_USART_2, /// ID_2
	InterruptSTM32F4xx::ID_USART_3, /// ID_3
	InterruptSTM32F4xx::ID_UART_4,  /// ID_4
	InterruptSTM32F4xx::ID_UART_5,  /// ID_5
	InterruptSTM32F4xx::ID_USART_6  /// ID_6
};

USART_TypeDef* UartSTM32F4xx::myUartMap[] =
{
	USART1, /// ID_1
	USART2, /// ID_2
	USART3, /// ID_3
	UART4,  /// ID_4
	UART5,  /// ID_5
	USART6  /// ID_6
};

static const uint16_t wordLengthMap[] =
{
    USART_WordLength_8b, /// ID_WORD_BITS_8
    USART_WordLength_9b  /// ID_WORD_BITS_9
};

static const uint16_t stopBitsMap[] =
{
    USART_StopBits_1, /// ID_STOP_BITS_1
    USART_StopBits_2  /// ID_STOP_BITS_2
};

static const uint16_t parityMap[] =
{
    USART_Parity_No,   /// ID_PARITY_NONE
    USART_Parity_Even, /// ID_PARITY_EVEN
    USART_Parity_Odd   /// ID_PARITY_ODD
};

static const uint16_t flowControlMap[] =
{
    USART_HardwareFlowControl_None /// ID_HARDWARE_FLOW_CONTROL_NONE
};

static const uint16_t interruptMap[] =
{
    USART_IT_TXE, /// INTERRUPT_TX
    USART_IT_RXNE /// INTERRUPT_RX
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
UartSTM32F4xx::UartSTM32F4xx(const Id id,
                             GpioPinSTM32F4xx& txGpioPin,
                             GpioPinSTM32F4xx& rxGpioPin) :
    Uart(),
    myId(id),
    myUart(myUartMap[myId]),
    myTransmitGpioPin(txGpioPin),
    myReceiveGpioPin(rxGpioPin),
    myInterrupt(myInterruptIdMap[myId],
                createCallback(this, &UartSTM32F4xx::interruptHandler))
{
    if (isNullPointer(interruptObjectMap[myId]))
    {
        interruptObjectMap[myId] = &myInterrupt;
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
// Public virtual  destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
UartSTM32F4xx::~UartSTM32F4xx()
{
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error UartSTM32F4xx::driverSetEnabled(const bool enabled)
{
    myTransmitGpioPin.setEnabled(enabled);
    myReceiveGpioPin.setEnabled(enabled);
    
    ProcessorSTM32F4xx::setPeripheralClockEnabled(myPeripheralMap[myId],
    											  enabled);
    
    if (enabled)
    {
        GpioPin::Config gpioPinConfig;
        gpioPinConfig.mode     = GpioPin::MODE_ALTERNATE_FUNCTION;
        gpioPinConfig.resistor = GpioPin::RESISTOR_PULL_UP;
        
        myTransmitGpioPin.configure(gpioPinConfig);
        myReceiveGpioPin.configure(gpioPinConfig);
    }

    setInterruptFlagEnabled(INTERRUPT_FLAG_RECEIVE_BUFFER_NOT_EMPTY, enabled);

    USART_Cmd(myUart, (FunctionalState) enabled);

    myInterrupt.setEnabled(enabled);
    
    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods implemented from Uart
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Uart::Error UartSTM32F4xx::driverSetConfig(const Config& config)
{
    USART_InitTypeDef uartInit;
    uartInit.USART_BaudRate            = config.baudRate;
    uartInit.USART_WordLength          = wordLengthMap[config.wordBits];
    uartInit.USART_StopBits            = stopBitsMap[config.stopBits];
    uartInit.USART_Parity              = parityMap[config.parity];
    uartInit.USART_HardwareFlowControl = flowControlMap[
                                                    config.hardwareFlowControl];
    uartInit.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(myUart, &uartInit);
    
    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
ComInterface::Error UartSTM32F4xx::driverTransmitBytes(
													 const ByteArray& byteArray,
													 const bool waitUntilDone)
{
    ComInterface::Error error;

    uint32_t i = 0;
    uint32_t nBytes = byteArray.getSize();

    while (i < nBytes)
    {
        setInterruptFlagEnabled(INTERRUPT_FLAG_TRANSMIT_BUFFER_EMPTY, false);

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
        setInterruptFlagEnabled(INTERRUPT_FLAG_TRANSMIT_BUFFER_EMPTY, true);
    }

    if (waitUntilDone)
    {
        while (myState == STATE_BUSY)
        {
        }
    }

    return error;
}

//------------------------------------------------------------------------------
uint32_t UartSTM32F4xx::driverGetReceivedBytesCount()
{
    // TODO: Check to see if receive interrupt is enabled

    setInterruptFlagEnabled(INTERRUPT_FLAG_RECEIVE_BUFFER_NOT_EMPTY, false);

    uint32_t nBytes = getReceiveBuffer()->count();

    setInterruptFlagEnabled(INTERRUPT_FLAG_RECEIVE_BUFFER_NOT_EMPTY, true);

    return nBytes;
}

//------------------------------------------------------------------------------
ComInterface::Error UartSTM32F4xx::driverGetReceivedBytes(ByteArray& byteArray,
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
			setInterruptFlagEnabled(INTERRUPT_FLAG_RECEIVE_BUFFER_NOT_EMPTY,
									false);

			if (getReceiveBuffer()->read(byte))
			{
				byteArray.append(byte);
			}

			setInterruptFlagEnabled(INTERRUPT_FLAG_RECEIVE_BUFFER_NOT_EMPTY,
									true);
		}
    }

    return ComInterface::Error(ComInterface::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void UartSTM32F4xx::writeByte(const uint8_t byte)
{
    myUart->DR = (uint16_t) byte;
}

//------------------------------------------------------------------------------
uint8_t UartSTM32F4xx::readByte()
{
    return ((uint8_t) (myUart->DR));
}

//------------------------------------------------------------------------------
void UartSTM32F4xx::setInterruptFlagEnabled(const InterruptFlag interrupt,
                                            const bool enable)
{
    USART_ITConfig(myUart, interruptMap[interrupt], (FunctionalState) enable);
}

//------------------------------------------------------------------------------
void UartSTM32F4xx::interruptHandler()
{
    // Transmit interrupt flag
    if (areBitsSet(myUart->CR1, USART_CR1_TXEIE) &&
        areBitsSet(myUart->SR, USART_SR_TXE))
    {
        uint8_t byte;

        if (getTransmitBuffer()->read(byte))
        {
            writeByte(byte);
        }
        else
        {
            setInterruptFlagEnabled(INTERRUPT_FLAG_TRANSMIT_BUFFER_EMPTY,
                                    false);
            myState = STATE_IDLE;
        }
    }

    if (areBitsSet(myUart->CR1, USART_CR1_RXNEIE))
    {
        // Receive interrupt flag
        if (areBitsSet(myUart->SR, USART_SR_RXNE))
        {
            uint8_t byte = readByte();

            if (!(getReceiveBuffer()->write(byte)))
            {
                // Buffer overflow
            }
        }

        // Overrun error interrupt flag
        if (areBitsSet(myUart->SR, USART_SR_ORE))
        {
            uint8_t dummyByte = readByte();
        }
    }
}

//------------------------------------------------------------------------------
// Interrupt service routines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
extern "C" void USART1_IRQHandler(void)
{
    interruptObjectMap[UartSTM32F4xx::ID_1]->handler();
}

//------------------------------------------------------------------------------
extern "C" void USART2_IRQHandler(void)
{
    interruptObjectMap[UartSTM32F4xx::ID_2]->handler();
}

//------------------------------------------------------------------------------
extern "C" void USART3_IRQHandler(void)
{
    interruptObjectMap[UartSTM32F4xx::ID_3]->handler();
}

//------------------------------------------------------------------------------
extern "C" void UART4_IRQHandler(void)
{
    interruptObjectMap[UartSTM32F4xx::ID_4]->handler();
}

//------------------------------------------------------------------------------
extern "C" void UART5_IRQHandler(void)
{
    interruptObjectMap[UartSTM32F4xx::ID_5]->handler();
}

//------------------------------------------------------------------------------
extern "C" void USART6_IRQHandler(void)
{
    interruptObjectMap[UartSTM32F4xx::ID_6]->handler();
}
