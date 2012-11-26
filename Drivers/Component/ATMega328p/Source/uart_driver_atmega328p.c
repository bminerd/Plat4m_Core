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
#include <uart_driver.h>

#include <avr/io.h>
#include <avr/interrupt.h>

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

static uart_interface_int_handler_f* intHandler;

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
 * @brief Sets the given UART enabled.
 * @param id UART driver ID.
 * @param enabled Flag that indicates if the UART should be enabled or disabled.
 */
static void setEnabled(const uart_e uart, const bool enabled);

/**
 * @brief Configures the given UART.
 * @param id UART driver ID.
 * @param config UART configuration.
 */
static void configure(const uart_e uart, const uart_config_t* config);

/**
 * @brief Transmits the given byte array over the given UART.
 * @param id UART driver ID.
 * @param byte Byte to transmit.
 */
static void tx(const uart_e uart, const uint8_t byte);

/**
 * @brief Transmits the given byte array over the given UART.
 * @param id UART driver ID.
 * @param byte Byte received.
 */
static void rx(const uart_e uart, uint8_t* byte);

/**
 * @brief Sets the given UART interrupt enabled or disabled.
 * @param id UART driver ID.
 * @param interrupt UART interrupt to set enabled.
 * @param enabled Flag that indicates if the interrupt should be enabled or
 * disabled.
 */
static void intSetEnabled(const uart_e uart,
                          const uart_interrupt_e interrupt,
                          const bool enabled);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void uartDriverInit(const uart_interface_int_handler_f* handler)
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
    
    uart_driver_t driver =
    {
        .setEnabled		= setEnabled,
        .configure		= configure,
        .tx				= tx,
        .rx				= rx,
        .intSetEnabled	= intSetEnabled
    };

    uartSetDriver(&driver);

    intHandler = handler;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static void setEnabled(const uart_e uart, const bool enabled)
{
	// Set baud rate
//	UBRR0H = (unsigned char) (ubrr>>8);
//	UBRR0L = (unsigned char) ubrr;
	//Enable receiver and transmitter
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	// Set frame format: 8data, 2stop bit
	UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}

//------------------------------------------------------------------------------
static void configure(const uart_e uart, const uart_config_t* config)
{

}

//------------------------------------------------------------------------------
static void tx(const uart_e uart, const uint8_t byte)
{
	UDR0 = byte;
}

//------------------------------------------------------------------------------
static void rx(const uart_e uart, uint8_t* byte)
{
	*byte = UDR0;
}

//------------------------------------------------------------------------------
static void intSetEnabled(const uart_e uart,
                          const uart_interrupt_e interrupt,
                          const bool enabled)
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
	intHandler(UART_0, UART_INTERRUPT_TX);
}

//------------------------------------------------------------------------------
ISR(USART_RX_vect)
{
	intHandler(UART_0, UART_INTERRUPT_RX);
}
