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
 * @file usb_driver_stm32f2xx.c
 * @author Ben Minerd
 * @date 10/24/2012
 * @brief USB driver layer for the STM32F2xx.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <usb_driver_stm32f2xx.h>
#include <usb_interface.h>

#include <usbd_cdc_core.h>
#include <usbd_usr.h>
#include <usb_conf.h>
#include <usbd_desc.h>

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

__ALIGN_BEGIN USB_OTG_CORE_HANDLE USB_OTG_dev __ALIGN_END;

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Sets the USB enabled or disabled.
 * @param enabled Flag that indicates if the USB should be enabled or disabled.
 */
static void setEnabled(const bool enabled);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void usbDriverStm32f2xxInit(void)
{
    usb_driver_t driver =
    {
        .setEnabled = setEnabled
    };

    usbSetDriver(&driver);

    USBD_Init(&USB_OTG_dev,
              USB_OTG_HS_CORE_ID,
              &USR_desc,
              &USBD_CDC_cb,
              &USR_cb);
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static void setEnabled(const bool enabled)
{

}

/*------------------------------------------------------------------------------
 * Interrupt service routines
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void OTG_HS_IRQHandler(void)
{
    USBD_OTG_ISR_Handler(&USB_OTG_dev);
}

//------------------------------------------------------------------------------
void OTG_HS_EP1_IN_IRQHandler(void)
{
    USBD_OTG_EP1IN_ISR_Handler(&USB_OTG_dev);
}

//------------------------------------------------------------------------------
void OTG_HS_EP1_OUT_IRQHandler(void)
{
    USBD_OTG_EP1OUT_ISR_Handler(&USB_OTG_dev);
}
