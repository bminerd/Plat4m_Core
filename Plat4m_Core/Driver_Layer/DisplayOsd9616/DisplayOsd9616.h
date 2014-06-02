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
 * @file DisplayOsd9616.h
 * @author Ben Minerd
 * @date 4/11/2013
 * @brief DisplayOsd9616 class.
 */

#ifndef _DISPLAY_OSD_9616_H_
#define _DISPLAY_OSD_9616_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <Display.h>
#include <I2cDevice.h>
#include <GpioPin.h>
#include <ByteArrayN.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

#define DISPLAY_OSD_9616_WIDTH  96

#define DISPLAY_OSD_9616_HEIGHT 16

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

class DisplayOsd9616 : public Display
{
public:
    
    /*--------------------------------------------------------------------------
     * Public constructors and destructors
     *------------------------------------------------------------------------*/
    
    DisplayOsd9616(I2c& i2c,
                   GpioPin& rstNGpioPin,
                   GpioPin& vccEnGpioPin,
                   GpioPin& pwrEnGpioPin);
    
private:
    
    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/
    
    I2cDevice myI2cDevice;
    
    GpioPin& myRstNGpioPin;
    
    GpioPin& myVccEnGpioPin;
    
    GpioPin& myPwrEnGpioPin;
    
    ByteArrayN<(DISPLAY_OSD_9616_WIDTH *
                (DISPLAY_OSD_9616_HEIGHT / 8)) + 1> myData;
    
    Frame myFrame;
    
    /*--------------------------------------------------------------------------
     * Private implemented methods
     *------------------------------------------------------------------------*/
    
    Frame& driverGetFrame();
    
    Error driverEnable(const bool enable);
    
    Error driverConfigure(const Config& config);
    
    Error driverWriteFrame();
    
    Error driverClear();
    
    Error driverSetBrightnessPercent(const float brightnessPercent);
};

#endif // _DISPLAY_OSD_9616_H_
