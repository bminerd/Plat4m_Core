/*------------------------------------------------------------------------------
 *       _______    __                           ___
 *      ||  ___ \  || |             __          //  |
 *      || |  || | || |   _______  || |__      //   |    _____  ___
 *      || |__|| | || |  // ___  | ||  __|    // _  |   ||  _ \/ _ \
 *      ||  ____/  || | || |  || | || |      // /|| |   || |\\  /\\ \
 *      || |       || | || |__|| | || |     // /_|| |_  || | || | || |
 *      || |       || |  \\____  | || |__  //_____   _| || | || | || |
 *      ||_|       ||_|       ||_|  \\___|       ||_|   ||_| ||_| ||_|
 *
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2013 Benjamin Minerd
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *----------------------------------------------------------------------------*/

/**
 * @file UartStm32f4xx.cpp
 * @author Ben Minerd
 * @date 5/14/13
 * @brief UartStm32f4xx class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <UartStm32f4xx.h>

#include <stm32f4xx.h>
#include <stm32f4xx_rcc.h>

using Plat4m::UartStm32f4xx;
using Plat4m::Uart;
using Plat4m::GpioStm32f4xx;

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 * @brief USART map.
 */
static USART_TypeDef* uartMap[6] =
{
    USART1, /// ID_1
    USART2, /// ID_2
    USART3, /// ID_3
    UART4,  /// ID_4
    UART5,  /// ID_5
    USART6  /// ID_6
};

/**
 * @brief UART clock map.
 */
static const uint32_t clockMap[6] =
{
    RCC_APB2Periph_USART1,  /// ID_1
    RCC_APB1Periph_USART2,  /// ID_2
    RCC_APB1Periph_USART3,  /// ID_3
    RCC_APB1Periph_UART4,   /// ID_4
    RCC_APB1Periph_UART5,   /// ID_5
    RCC_APB2Periph_USART6   /// ID_6
};

/**
 * @brief UART IRQ map.
 */
static const IRQn_Type irqMap[6] =
{
    USART1_IRQn,    /// ID_1
    USART2_IRQn,    /// ID_2
    USART3_IRQn,    /// ID_3
    UART4_IRQn,     /// ID_4
    UART5_IRQn,     /// ID_5
    USART6_IRQn     /// ID_6
};

/**
 * @brief UART alternate function map.
 */
static const GpioStm32f4xx::AlternateFunction afMap[6] =
{
    GpioStm32f4xx::ALTERNATE_FUNCTION_USART1,   /// ID_1
    GpioStm32f4xx::ALTERNATE_FUNCTION_USART2,   /// ID_2
    GpioStm32f4xx::ALTERNATE_FUNCTION_USART3,   /// ID_3
    GpioStm32f4xx::ALTERNATE_FUNCTION_UART4,    /// ID_4
    GpioStm32f4xx::ALTERNATE_FUNCTION_UART5,    /// ID_5
    GpioStm32f4xx::ALTERNATE_FUNCTION_USART6    /// ID_6
};

/**
 * @brief UART word length map.
 */
static const uint16_t wordLengthMap[] =
{
    USART_WordLength_8b,    /// ID_WORD_BITS_8
    USART_WordLength_9b     /// ID_WORD_BITS_9
};

/**
 * @brief UART stop bits map.
 */
static const uint16_t stopBitsMap[] =
{
    USART_StopBits_1,   /// ID_STOP_BITS_1
    USART_StopBits_2    /// ID_STOP_BITS_2
};

/**
 * @brief UART parity map.
 */
static const uint16_t parityMap[] =
{
    USART_Parity_No,    /// ID_PARITY_NONE
    USART_Parity_Even,  /// ID_PARITY_EVEN
    USART_Parity_Odd    /// ID_PARITY_ODD
};

/**
 * @brief UART hardware flow control map.
 */
static const uint16_t flowControlMap[] =
{
    USART_HardwareFlowControl_None  /// ID_HARDWARE_FLOW_CONTROL_NONE
};

/**
 * @brief UART interrupt map.
 */
static const uint16_t interruptMap[] =
{
    USART_IT_TXE,   /// Uart::INTERRUPT_TX
    USART_IT_RXNE   /// Uart::INTERRUPT_RX
};

static const GpioStm32f4xx::OutputSpeed outputSpeed =
                                              GpioStm32f4xx::OUTPUT_SPEED_50MHZ;

static UartStm32f4xx* objectMap[6];

/*------------------------------------------------------------------------------
 * Public static methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
UartStm32f4xx& UartStm32f4xx::get(const Id id,
                                  GpioPinStm32f4xx& txGpioPin,
                                  GpioPinStm32f4xx& rxGpioPin)
{
    if (IS_NULL_POINTER(objectMap[id]))
    {
        objectMap[id] = new UartStm32f4xx(id, txGpioPin, rxGpioPin);
    }
    else
    {
        // Error?
    }
    
    return *(objectMap[id]);
}

/*------------------------------------------------------------------------------
 * Private constructors and destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
UartStm32f4xx::UartStm32f4xx(const Id id,
                             GpioPinStm32f4xx& txGpioPin,
                             GpioPinStm32f4xx& rxGpioPin) :
    Uart(),
    myId(id),
    myUart(uartMap[id]),
    myTxGpioPin(txGpioPin),
    myRxGpioPin(rxGpioPin)
{
}

/*------------------------------------------------------------------------------
 * Private implemented methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Uart::Error UartStm32f4xx::driverEnable(const bool enable)
{
    myTxGpioPin.enable(enable);
    myRxGpioPin.enable(enable);
    
    switch (myId)
    {
        case ID_1:
        case ID_6:
        {
            RCC_APB2PeriphClockCmd(clockMap[myId], (FunctionalState) enable);

            break;
        }
        default:
        {
            RCC_APB1PeriphClockCmd(clockMap[myId], (FunctionalState) enable);

            break;
        }
    }
    
    if (enable)
    {
        GpioPin::Config gpioConfig;
        gpioConfig.mode     = GpioPin::MODE_ALTERNATE_FUNCTION;
        gpioConfig.resistor = GpioPin::RESISTOR_PULL_UP;
        
        myTxGpioPin.configure(gpioConfig);
        myRxGpioPin.configure(gpioConfig);
        
        GpioStm32f4xx::Config gpioDriverConfig;
        gpioDriverConfig.alternateFunction  = afMap[myId];
        gpioDriverConfig.outputSpeed        = outputSpeed;
        
        myTxGpioPin.configureDriver(gpioDriverConfig);
        myRxGpioPin.configureDriver(gpioDriverConfig);
    }

    USART_Cmd(myUart, (FunctionalState) enable);

    NVIC_InitTypeDef nvicInit;
    nvicInit.NVIC_IRQChannel                    = irqMap[myId];
    nvicInit.NVIC_IRQChannelPreemptionPriority  = 0;
    nvicInit.NVIC_IRQChannelSubPriority         = 0;
    nvicInit.NVIC_IRQChannelCmd                 = (FunctionalState) enable;

    NVIC_Init(&nvicInit);
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
Uart::Error UartStm32f4xx::driverConfigure(const Config& config)
{
    USART_InitTypeDef uartInit;
    uartInit.USART_BaudRate             = config.baudRate;
    uartInit.USART_WordLength           = wordLengthMap[config.wordBits];
    uartInit.USART_StopBits             = stopBitsMap[config.stopBits];
    uartInit.USART_Parity               = parityMap[config.parity];
    uartInit.USART_HardwareFlowControl  = flowControlMap[
                                                    config.hardwareFlowControl];
    uartInit.USART_Mode                 = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(myUart, &uartInit);
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
Uart::Error UartStm32f4xx::driverTx(const uint8_t byte)
{
    myUart->DR = (uint16_t) byte;
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
Uart::Error UartStm32f4xx::driverRx(uint8_t& byte)
{
    byte = (uint8_t) (myUart->DR);
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
Uart::Error UartStm32f4xx::driverInterruptEnable(const Interrupt interrupt,
                                                 const bool enable)
{
    USART_ITConfig(myUart, interruptMap[interrupt], (FunctionalState) enable);
    
    return ERROR_NONE;
}

/*------------------------------------------------------------------------------
 * Interrupt service routines
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern "C" void USART1_IRQHandler(void)
{
    // Transmit interrupt
    if (USART_GetITStatus(USART1, USART_IT_TXE))
    {
        objectMap[UartStm32f4xx::ID_1]->interruptHandler(Uart::INTERRUPT_TX);
    }

    // Receive interrupt
    if (USART_GetITStatus(USART1, USART_IT_RXNE))
    {
        objectMap[UartStm32f4xx::ID_1]->interruptHandler(Uart::INTERRUPT_RX);
    }
}

//------------------------------------------------------------------------------
extern "C" void USART2_IRQHandler(void)
{
    // Transmit interrupt
    if (USART_GetITStatus(USART2, USART_IT_TXE))
    {
        objectMap[UartStm32f4xx::ID_2]->interruptHandler(Uart::INTERRUPT_TX);
    }

    // Receive interrupt
    if (USART_GetITStatus(USART2, USART_IT_RXNE))
    {
        objectMap[UartStm32f4xx::ID_2]->interruptHandler(Uart::INTERRUPT_RX);
    }
    
    if (USART_GetITStatus(USART2, USART_IT_ORE_RX))
    {
        USART_ClearITPendingBit(USART2, USART_IT_ORE_RX);
    }
}

//------------------------------------------------------------------------------
extern "C" void USART3_IRQHandler(void)
{
    // Transmit interrupt
    if (USART_GetITStatus(USART3, USART_IT_TXE))
    {
        objectMap[UartStm32f4xx::ID_3]->interruptHandler(Uart::INTERRUPT_TX);
    }

    // Receive interrupt
    if (USART_GetITStatus(USART3, USART_IT_RXNE))
    {
        objectMap[UartStm32f4xx::ID_3]->interruptHandler(Uart::INTERRUPT_RX);
    }
}

//------------------------------------------------------------------------------
extern "C" void USART4_IRQHandler(void)
{
    // Transmit interrupt
    if (USART_GetITStatus(UART4, USART_IT_TXE))
    {
        objectMap[UartStm32f4xx::ID_4]->interruptHandler(Uart::INTERRUPT_TX);
    }

    // Receive interrupt
    if (USART_GetITStatus(UART4, USART_IT_RXNE))
    {
        objectMap[UartStm32f4xx::ID_4]->interruptHandler(Uart::INTERRUPT_RX);
    }
}

//------------------------------------------------------------------------------
extern "C" void USART5_IRQHandler(void)
{
    // Transmit interrupt
    if (USART_GetITStatus(UART5, USART_IT_TXE))
    {
        objectMap[UartStm32f4xx::ID_5]->interruptHandler(Uart::INTERRUPT_TX);
    }

    // Receive interrupt
    if (USART_GetITStatus(UART5, USART_IT_RXNE))
    {
        objectMap[UartStm32f4xx::ID_5]->interruptHandler(Uart::INTERRUPT_RX);
    }
}

//------------------------------------------------------------------------------
extern "C" void USART6_IRQHandler(void)
{
    // Transmit interrupt
    if (USART_GetITStatus(USART6, USART_IT_TXE))
    {
        objectMap[UartStm32f4xx::ID_6]->interruptHandler(Uart::INTERRUPT_TX);
    }

    // Receive interrupt
    if (USART_GetITStatus(USART6, USART_IT_RXNE))
    {
        objectMap[UartStm32f4xx::ID_6]->interruptHandler(Uart::INTERRUPT_RX);
    }
}
