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

#include <uart_driver_atmega328p.h>

// plat4m
//
// Include necessary manufacturer driver header files here.
// ex.
//      #include <stm32f10x_usart.h>

#include <avr/io.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

#define USART0_DATA_REGISTER_EMPTY_INT_BIT	(5)
#define USART0_RX_INT_BIT					(7)

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
//      static void uart0SetEnabled(bool enabled);
//      static void uart0Tx(uint8_t data);
//      ...

/**
 * TODO Comment!
 */
static void uart0SetEnabled(bool enabled);

/**
 * TODO Comment!
 */
static uart_error_e uart0Tx(uint8_t data);

/**
 * TODO Comment!
 */
static uart_error_e uart0Rx(uint8_t* data);

/**
 * TODO Comment!
 */
static void uart0TxIntSetEnabled(bool enabled);

/**
 * TODO Comment!
 */
static void uart0RxIntSetEnabled(bool enabled);

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
    //              .setEnabled = uart0SetEnabled,
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
        // UART_0
        {
            .id                 = UART_ID_0,
            .isEnabled          = true,
            .setEnabled         = uart0SetEnabled,
            .tx                 = uart0Tx,
            .rx                 = uart0Rx,
            .txIntSetEnabled    = uart0TxIntSetEnabled,
            .rxIntSetEnabled    = uart0RxIntSetEnabled
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
static void uart0SetEnabled(bool enabled)
{
	// Set baud rate
	UBRR0H = (unsigned char) (ubrr>>8);
	UBRR0L = (unsigned char) ubrr;
	//Enable receiver and transmitter
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	// Set frame format: 8data, 2stop bit
	UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}

//------------------------------------------------------------------------------
static uart_error_e uart0Tx(uint8_t data)
{
    UDR0 = data;
    
    return UART_ERROR_NONE;
}

//------------------------------------------------------------------------------
static uart_error_e uart0Rx(uint8_t* data)
{
    *data = UDR0;
    
    return UART_ERROR_NONE;
}

//------------------------------------------------------------------------------
static void uart0TxIntSetEnabled(bool enabled)
{
    if (enabled)
    {
    	// Set the USART data register empty interrupt bit
    	UCSR0B |= (1 << USART0_DATA_REGISTER_EMPTY_INT_BIT);
    }
    else
    {
    	// Clear the USART data register empty interrupt bit
    	UCSR0B &= ~(1 << USART0_DATA_REGISTER_EMPTY_INT_BIT);
    }
}

//------------------------------------------------------------------------------
static void uart0RxIntSetEnabled(bool enabled)
{
	if (enabled)
	{
		// Set the USART data register empty interrupt bit
		UCSR0B |= (1 << USART0_RX_INT_BIT);
	}
	else
	{
		// Clear the USART data register empty interrupt bit
		UCSR0B &= ~(1 << USART0_RX_INT_BIT);
	}
}

/*------------------------------------------------------------------------------
 * Interrupt service routines
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
ISR(USART_UDRE_vect)
{
	uartIntHandler(UART_ID_0, UART_INTERRUPT_TX);
}

//------------------------------------------------------------------------------
ISR(USART_RX_vect)
{
	uartIntHandler(UART_ID_0, UART_INTERRUPT_RX);
}
