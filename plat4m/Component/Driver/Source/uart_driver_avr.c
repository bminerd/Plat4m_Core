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
 * @file uart_driver_avr.c
 * @author Ben Minerd
 * @date 2/3/12
 * @brief
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <uart_interface.h>
#include <uart_driver_avr.h>

//#include <avr/interrupt.h>
//#include <avr/io.h>
//#include <avr/signal.h>

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

static uart_callback_f* txCallback = 0;

static uart_callback_f* rxCallback = 0;

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

static void uart0Init(uint16_t ubrr);

static bool uart0Tx(uint8_t data);

static bool uart0Rx(uint8_t* data);

static void uart0TxIntSetEnabled(bool enabled);

static void uart0RxIntSetEnabled(bool enabled);

static void uart0TxIsr(void);

static void uart0RxIsr(void);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void uartDriverInit(uart_callback_f* uartTxCallback,
		   	   	   	   	   uart_callback_f* uartRxCallback)
{
	txCallback = uartTxCallback;
	rxCallback = uartRxCallback;

	uart0Init(1);

	uartAdd(UART_ID_0, uart0Tx);
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static void uart0Init(uint16_t ubrr)
{
//	// Set baud rate
//	UBRR0H = (uint8_t)(ubrr >> 8);
//	UBRR0L = (uint8_t)ubrr;
//
//	// Enable receiver and transmitter
//	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
//
//	// Set frame format: 8data, 2stop bit
//	UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}

//------------------------------------------------------------------------------
static bool uart0Tx(uint8_t data)
{
//	// Wait for empty transmit buffer
//	while (!(UCSR0A & (1 << UDRE0)));
//
//	// Put data into buffer, sends the data
//	UDR0 = data;

	return true;
}

//------------------------------------------------------------------------------
static bool uart0Rx(uint8_t* data)
{
//	// Wait for data to be received
//	while (!(UCSR0A & (1 << RXC0)));
//
//	// Get and return received data from buffer
//	*data = UDR0;

	return true;
}

//------------------------------------------------------------------------------
static void uart0TxIntSetEnabled(bool enabled)
{
//	if (enabled)
//	{
//		// Enable Tx buffer empty interrupt
//		UCSR0B |= (1 << UDRIE0);
//	}
//	else
//	{
//		// Disable Tx buffer empty interrupt
//		// VERIFY THIS WORKS!
//		UCSR0B &= ~(1 << UDRIE0);
//	}
}

//------------------------------------------------------------------------------
static void uart0RxIntSetEnabled(bool enabled)
{
//	if (enabled)
//	{
//		// Enable Rx
//		UCSR0B |= (1 << RXCIE0);
//	}
//	else
//	{
//		UCSR0B &= ~(1 << RXCIE0);
//	}
}

//------------------------------------------------------------------------------
//SIGNAL(USART_UDRE_vect)
static void uart0TxIsr(void)
{
	uartTxCallback(UART_ID_0);
}

//------------------------------------------------------------------------------
static void uart0RxIsr(void)
{
	uartRxCallback(UART_ID_0);
}
