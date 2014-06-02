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
 * Copyright (c) 2014 Benjamin Minerd
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
 * @file DisplayUsmpP23302.h
 * @author Ben Minerd
 * @date 4/23/2014
 * @brief DisplayUsmpP23302 class.
 */

#ifndef _DISPLAY_USMP_P23302_H_
#define _DISPLAY_USMP_P23302_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <Display.h>
#include <GpioPin.h>
#include <I2c.h>
#include <Spi.h>
#include <Ssd1307.h>
#include <ByteArrayN.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

#define DISPLAY_USMP_P23302_WIDTH   96

#define DISPLAY_USMP_P23302_HEIGHT  32

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

class DisplayUsmpP23302 : public Display
{
public:
    
    /*--------------------------------------------------------------------------
     * Public constructors and destructors
     *------------------------------------------------------------------------*/
    
    DisplayUsmpP23302(GpioPin& rstNGpioPin,
                      GpioPin& vccEnGpioPin,
                      GpioPin& pwrEnGpioPin,
                      I2c& i2c);
    
    DisplayUsmpP23302(GpioPin& rstNGpioPin,
                      GpioPin& vccEnGpioPin,
                      GpioPin& pwrEnGpioPin,
                      GpioPin& chipSelectGpioPin,
                      Spi& spi,
                      GpioPin& dataCommandGpioPin);
    
private:
    
    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/
    
    Ssd1307& mySsd1307;
    
    ByteArrayN<(DISPLAY_USMP_P23302_WIDTH *
                (DISPLAY_USMP_P23302_HEIGHT / 8))> myData;
    
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

#endif // _DISPLAY_USMP_P23302_H_
