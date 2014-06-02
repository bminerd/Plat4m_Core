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
 * @file Ssd1307Spi4Wire.cpp
 * @author Ben Minerd
 * @date 4/23/2014
 * @brief Ssd1307Spi4Wire class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Ssd1307Spi4Wire.h>

#include <System.h>

/*------------------------------------------------------------------------------
 * Public constructors and destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Ssd1307Spi4Wire::Ssd1307Spi4Wire(const unsigned int displayStartLine,
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
                                 GpioPin& chipSelectGpioPin,
                                 Spi& spi,
                                 GpioPin& dataCommandGpioPin,
                                 ByteArray& byteArray) :
    Ssd1307(displayStartLine,
            displayOffset,
            columnStartAddress,
            columnEndAddress,
            pageStartAddress,
            pageEndAddress,
            segmentRemap,
            memoryAddressingMode,
            rstNGpioPin,
            vccEnGpioPin,
            pwrEnGpioPin,
            byteArray),
    mySpiDevice(&chipSelectGpioPin, spi),
    myDataCommandGpioPin(dataCommandGpioPin)
{
}

/*------------------------------------------------------------------------------
 * Private methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Ssd1307::Error Ssd1307Spi4Wire::driverEnable(const bool enable)
{
    mySpiDevice.enable(enable);
    myDataCommandGpioPin.enable(enable);
    
    if (enable)
    {
        GpioPin::Config gpioConfig;
        gpioConfig.mode     = GpioPin::MODE_DIGITAL_OUTPUT;
        gpioConfig.resistor = GpioPin::RESISTOR_NONE;
        
        myDataCommandGpioPin.configure(gpioConfig);
        myDataCommandGpioPin.setLevel(GpioPin::LEVEL_LOW);
    }
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
Ssd1307::Error Ssd1307Spi4Wire::driverWriteCommand(const ByteArray& byteArray)
{
    if (mySpiDevice.tx(byteArray) != SlaveDevice::ERROR_NONE)
    {
        return ERROR_COMMUNICATION;
    }
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
Ssd1307::Error Ssd1307Spi4Wire::driverWriteData(const ByteArray& byteArray,
                                                const bool waitUntilDone)
{
    Error error;
    
    myDataCommandGpioPin.setLevel(GpioPin::LEVEL_HIGH);
    
    // Might not need to wait if mailbox was implemented?
    if (mySpiDevice.tx(byteArray, true) != SlaveDevice::ERROR_NONE)
    {
        error = ERROR_COMMUNICATION;
    }
    
    myDataCommandGpioPin.setLevel(GpioPin::LEVEL_LOW);
    
    return error;
}