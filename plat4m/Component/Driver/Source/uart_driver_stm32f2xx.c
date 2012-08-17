/*------------------------------------------------------------------------------
 *                  __                           ___
 *                 || |             __          //  |
 *       _______   || |   _______  || |__      //   |    _____  ___
 *      ||  ___ \  || |  // ___  | ||  __|    // _  |   ||  _ \/ _ \
 *      || |  || | || | || |  || | || |      // /|| |   || |\\  /\\ \
 *      || |__|| | || | || |__|| | || |     // /_|| |_  || | || | || |
 *      ||  ____/  || |  \\____  | || |__  //_____   _| || | || | || |
 *      || |       ||_|       ||_|  \\___|       ||_|   ||_| ||_| ||_|
 *      || |
 *      ||_|
 *
 * Copyright (c) 2012 Ben Minerd. All rights reserved.
 *
 * GNU Lesser General Public License Usage
 * This file may be used under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation and
 * appearing in the file LICENSE.LGPL included in the packaging of this file.
 * Please review the following information to ensure the GNU Lesser General
 * Public License version 2.1 requirements will be met:
 * http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 *----------------------------------------------------------------------------*/

/**
 * @file uart_driver_template.c
 * @author Ben Minerd
 * @date 2/3/12
 * @brief TODO Comment!
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <uart_interface.h>

// plat4m
//
// Replace below with proper driver header file.
// ex.
//      #include <uart_driver_stm32f10x.h>

#include <uart_driver_stm32f2xx.h>

// plat4m
//
// Include necessary manufacturer driver header files here.
// ex.
//      #include <stm32f10x_usart.h>

#include <stm322xg_eval.h>
#include <stm32f2xx_usart.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

// plat4m
//
// Declare local UART driver functions here.
//
// Example:
//
//      static void uart1SetEnabled(bool enabled);
//      static void uart1Tx(uint8_t data);
//      ...

/**
 * TODO Comment!
 */
static void uart1SetEnabled(bool enabled);

/**
 * TODO Comment!
 */
static uart_error_e uart1Tx(uint8_t data);

/**
 * TODO Comment!
 */
static uart_error_e uart1Rx(uint8_t* data);

/**
 * TODO Comment!
 */
static void uart1TxIntSetEnabled(bool enabled);

/**
 * TODO Comment!
 */
static void uart1RxIntSetEnabled(bool enabled);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void uartDriverInit(void)
{
    // plat4m
    //
    // Initialize UART driver.
    // Add drivers to UART interface.
    //
    // Example:
    //
    //      uart_driver_t uarts[] =
    //      {
    //          // UART_1
    //          {
    //              .id         = UART_DRIVER_ID_1,
    //              .setEnabled = uart1SetEnabled,
    //              ...
    //          },
    //          // UART_2
    //          {
    //              .id         = UART_DRIVER_ID_2,
    //              .setEnabled = uart2SetEnabled,
    //              ...
    //          }
    //          ...
    //      };
    //
    //      uartAddDrivers(uarts, ARRAY_SIZE(uarts));
    
    uart_driver_t uarts[] =
    {
        // UART_1
        {
            .id                 = UART_DRIVER_ID_1,
            .setEnabled         = uart1SetEnabled,
            .tx                 = uart1Tx,
            .rx                 = uart1Rx,
            .txIntSetEnabled    = uart1TxIntSetEnabled,
            .rxIntSetEnabled    = uart1RxIntSetEnabled
        }
    };

    uartAddDrivers(uarts, ARRAY_SIZE(uarts));
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

// plat4m
//
// Define here local UART driver functions declared above.

//------------------------------------------------------------------------------
static void uart1SetEnabled(bool enabled)
{
    USART_InitTypeDef uartInit;
    NVIC_InitTypeDef nvicInit;
    
    uartInit.USART_BaudRate             = 9600;
    uartInit.USART_WordLength           = USART_WordLength_8b;
    uartInit.USART_StopBits             = USART_StopBits_1;
    uartInit.USART_Parity               = USART_Parity_No;
    uartInit.USART_Mode                 = USART_Mode_Rx | USART_Mode_Tx;
    uartInit.USART_HardwareFlowControl  = USART_HardwareFlowControl_None;
    
    nvicInit.NVIC_IRQChannel                      = EVAL_COM1_IRQn;
    nvicInit.NVIC_IRQChannelPreemptionPriority    = 0;
    nvicInit.NVIC_IRQChannelSubPriority           = 0;
    
    if (enabled)
    {
        nvicInit.NVIC_IRQChannelCmd = ENABLE;

        STM_EVAL_COMInit(COM1, &uartInit);
        NVIC_Init(&nvicInit);
    }
    else
    {
        nvicInit.NVIC_IRQChannelCmd = DISABLE;

        NVIC_Init(&nvicInit);
    }
}

//------------------------------------------------------------------------------
static uart_error_e uart1Tx(uint8_t data)
{
    USART_SendData(EVAL_COM1, data);
    
    return UART_ERROR_NONE;
}

//------------------------------------------------------------------------------
static uart_error_e uart1Rx(uint8_t* data)
{
    *data = (uint8_t)USART_ReceiveData(EVAL_COM1);
    
    return UART_ERROR_NONE;
}

//------------------------------------------------------------------------------
static void uart1TxIntSetEnabled(bool enabled)
{
    if (enabled)
    {
        USART_ITConfig(EVAL_COM1, USART_IT_TXE, ENABLE);
    }
    else
    {
        USART_ITConfig(EVAL_COM1, USART_IT_TXE, DISABLE);
    }
}

//------------------------------------------------------------------------------
static void uart1RxIntSetEnabled(bool enabled)
{
    if (enabled)
    {
        USART_ITConfig(EVAL_COM1, USART_IT_RXNE, ENABLE);
    }
    else
    {
        USART_ITConfig(EVAL_COM1, USART_IT_RXNE, DISABLE);
    }
}

/*------------------------------------------------------------------------------
 * Interrupt service routines
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void USART3_IRQHandler(void)
{
    // Transmit interrupt
    if (USART_GetITStatus(EVAL_COM1, USART_IT_TXE) != RESET)
    {
        USART_ClearITPendingBit(EVAL_COM1, USART_IT_TXE);
        uartIntHandler(UART_DRIVER_ID_1, UART_INTERRUPT_TX);
    }

    // Receive interrupt
    if (USART_GetITStatus(EVAL_COM1, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(EVAL_COM1, USART_IT_RXNE);
        uartIntHandler(UART_DRIVER_ID_1, UART_INTERRUPT_RX);
    }
}
