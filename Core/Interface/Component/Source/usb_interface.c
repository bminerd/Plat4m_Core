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
 * @file usb_interface.c
 * @author Ben Minerd
 * @date 7/30/12
 * @brief USB interface layer.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <usb_interface.h>
#include <mutex_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 * @brief USB driver.
 */
static usb_driver_t driver;

/**
 * @brief USB mutex.
 */
static mutex_t mutex;

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void usbInit(void)
{
    mutexInitMutex(&mutex);

    usbApplicationInit();
}

//------------------------------------------------------------------------------
usb_error_e usbSetDriver(const usb_driver_t* usbDriver)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(usbDriver) ||
        IS_NULL_POINTER(usbDriver->setEnabled))
    {
        return USB_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    driver = *usbDriver;

    return USB_ERROR_NONE;
}

//------------------------------------------------------------------------------
usb_error_e usbSetEnabled(const bool enabled)
{
#ifdef PLAT4M_DEBUG



#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&mutex, true);

    driver.setEnabled(enabled);

    // Unlock mutex
    mutexSetLocked(&mutex, false);

    return USB_ERROR_NONE;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

