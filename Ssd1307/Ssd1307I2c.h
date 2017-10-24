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
 * @file Ssd1307I2c.h
 * @author Ben Minerd
 * @date 4/23/2014
 * @brief Ssd1307I2c class.
 */

#ifndef _SSD1307_I2C_H_
#define _SSD1307_I2C_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <Ssd1307.h>
#include <GpioPin.h>
#include <I2c.h>
#include <I2cDevice.h>
#include <ByteArray.h>
#include <ByteArrayN.h>

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

class Ssd1307I2c : public Ssd1307
{
public:
    
    /*--------------------------------------------------------------------------
     * Public constructors and destructors
     *------------------------------------------------------------------------*/
    
    Ssd1307I2c(const unsigned int displayStartLine,
               const unsigned int displayOffset,
               const unsigned int columnStartAddress,
               const unsigned int columnEndAddress,
               const unsigned int pageStartAddress,
               const unsigned int pageEndAddress,
               const SegmentRemap segmentRemap,
               const MemoryAddressingMode memoryAddressingMode,
               GpioPin& rstNGpioPin,
               GpioPin& vccEnGpioPin,
               GpioPin& pwrEnGpioPin,
               I2c& i2c,
               ByteArray& byteArray);
    
private:
    
    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/
    
    I2cDevice myI2cDevice;
    
    ByteArrayN<(96 * 4) + 1> myByteArray;
    
    /*--------------------------------------------------------------------------
     * Private pure virtual methods
     *------------------------------------------------------------------------*/
    
    Error driverEnable(const bool enable);
    
    Error driverWriteCommand(const ByteArray& byteArray);
    
    Error driverWriteData(const ByteArray& byteArray, const bool waitUntilDone);
};

#endif // _SSD1307_I2C_H_
