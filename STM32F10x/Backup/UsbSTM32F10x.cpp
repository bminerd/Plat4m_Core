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
 * @file UsbStm32f4xx.cpp
 * @author Ben Minerd
 * @date 4/22/2013
 * @brief UsbStm32f4xx class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <UsbStm32f4xx.h>

#include <usb_core.h>
#include <usbd_core.h>
#include <usbd_cdc_core.h>
#include <usbd_usr.h>
#include <usb_conf.h>
#include <usbd_desc.h>
#include <usbd_cdc_vcp.h>
#include <usb_dcd_int.h>

using Plat4m::UsbStm32f4xx;
using Plat4m::Usb;

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

__ALIGN_BEGIN USB_OTG_CORE_HANDLE USB_OTG_dev __ALIGN_END;

static Usb* objectMap[1];

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

static void rxCallback(uint8_t* data, uint32_t size);

#ifdef USB_OTG_HS_DEDICATED_EP1_ENABLED 
extern uint32_t USBD_OTG_EP1IN_ISR_Handler (USB_OTG_CORE_HANDLE *pdev);
extern uint32_t USBD_OTG_EP1OUT_ISR_Handler (USB_OTG_CORE_HANDLE *pdev);
#endif

/*------------------------------------------------------------------------------
 * Public constructors and destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
UsbStm32f4xx::UsbStm32f4xx()
{
    objectMap[0] = this;
}

/*------------------------------------------------------------------------------
 * Private methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Usb::Error UsbStm32f4xx::driverEnable(const bool enable)
{
    if (enable)
    {
        USBD_Init(&USB_OTG_dev,
                  USB_OTG_FS_CORE_ID,
                  &USR_desc,
                  &USBD_CDC_cb,
                  &USR_cb);
    
        VCP_SetDataRxCallback(&rxCallback);
    }
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
Usb::Error UsbStm32f4xx::driverTx(const ByteArray& byteArray)
{
    VCP_DataTx(byteArray.getItems(), byteArray.getSize());
    
    return ERROR_NONE;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static void rxCallback(uint8_t* data, uint32_t size)
{
    ByteArray byteArray(data, size);
    
    objectMap[0]->interruptHandler(byteArray);
}

/*------------------------------------------------------------------------------
 * Interrupt service routines
 *----------------------------------------------------------------------------*/

#ifdef USE_USB_OTG_FS

//------------------------------------------------------------------------------
extern "C" void OTG_FS_IRQHandler(void)
{
    USBD_OTG_ISR_Handler(&USB_OTG_dev);
}

#endif // USE_USB_OTG_FS

#ifdef USE_USB_OTG_HS

//------------------------------------------------------------------------------
extern "C" void OTG_HS_IRQHandler(void)
{
    USBD_OTG_ISR_Handler(&USB_OTG_dev);
}

//------------------------------------------------------------------------------
extern "C" void OTG_HS_EP1_IN_IRQHandler(void)
{
    USBD_OTG_EP1IN_ISR_Handler(&USB_OTG_dev);
}

//------------------------------------------------------------------------------
extern "C" void OTG_HS_EP1_OUT_IRQHandler(void)
{
    USBD_OTG_EP1OUT_ISR_Handler(&USB_OTG_dev);
}

#endif // USE_USB_OTG_HS
