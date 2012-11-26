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
 * @file usb_interface.h
 * @author Ben Minerd
 * @date 7/30/12
 * @brief USB interface layer.
 */

#ifndef _USB_INTERFACE_H_
#define _USB_INTERFACE_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>
#include <usb_application.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * @brief Enumeration of USB errors.
 */
typedef enum _usb_error_e_
{
    USB_ERROR_NONE = 0,
    USB_ERROR_PARAMETER_INVALID,
    USB_ERROR_NOT_ENABLED
} usb_error_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * @brief Function type that sets the USB enabled or disabled.
 * @param enabled Flag that indicates if the USB should be enabled or disabled.
 */
typedef void usb_driver_set_enabled_f(const bool enabled);

/**
 * @brief USB driver type.
 */
typedef struct _usb_driver_t_
{
    usb_driver_set_enabled_f* setEnabled;
} usb_driver_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the USB module.
 */
void usbInit(void);

/**
 * @brief Sets the driver for the USB interface.
 * @param usbDriver USB driver.
 * @return USB error.
 */
usb_error_e usbSetDriver(const usb_driver_t* usbDriver);

/**
 * @brief Sets the USB enabled or disabled.
 * @param enabled Flag that indicates if the USB should be enabled or disabled.
 * @return USB error.
 */
usb_error_e usbSetEnabled(const bool enabled);

#endif // _USB_INTERFACE_H_
