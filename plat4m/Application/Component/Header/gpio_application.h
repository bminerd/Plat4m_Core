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
 * @file gpio_application.h
 * @author Ben Minerd
 * @date 9/21/2012
 * @brief TODO Comment!
 */

#ifndef _GPIO_APPLICATION_H_
#define _GPIO_APPLICATION_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * @brief Enumeration of GPIO IDs.
 */
typedef enum _gpio_id_e_
{
    GPIO_ID_LED_1 = 0,
    GPIO_ID_LED_2,
    GPIO_ID_LED_3,
    GPIO_ID_LED_4,
    GPIO_ID_OLED_RST_N,
    GPIO_ID_OLED_VCC_EN,
    GPIO_ID_OLED_PWR_EN,
    GPIO_ID_BUTTON_KEY,
    GPIO_ID_BUTTON_TAMPER,
    GPIO_ID_BUTTON_WAKEUP,

    GPIO_ID_UART_1_TX,
    GPIO_ID_UART_1_RX,
    GPIO_ID_UART_2_TX,
    GPIO_ID_UART_2_RX,
    GPIO_ID_UART_3_TX,
    GPIO_ID_UART_3_RX,
    GPIO_ID_UART_4_TX,
    GPIO_ID_UART_4_RX,
    GPIO_ID_UART_5_TX,
    GPIO_ID_UART_5_RX,
    GPIO_ID_UART_6_TX,
    GPIO_ID_UART_6_RX,

    // Do not place values below!
    GPIO_ID_COUNT
} gpio_id_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the GPIO application layer.
 */
extern void gpioApplicationInit(void);

#endif // _GPIO_APPLICATION_H_
