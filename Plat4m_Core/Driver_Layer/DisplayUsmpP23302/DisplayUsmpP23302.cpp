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
 * @file DisplayUsmpP23302.cpp
 * @author Ben Minerd
 * @date 4/23/2014
 * @brief DisplayUsmpP23302 class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <DisplayUsmpP23302.h>

#include <Ssd1307I2c.h>
#include <Ssd1307Spi4Wire.h>
#include <System.h>

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

static const unsigned int width                 = 96;
static const unsigned int height                = 32;
static const unsigned int displayStartLine      = 0;
static const unsigned int displayOffset         = 0;
static const unsigned int columnStartAddress    = 0;
static const unsigned int columnEndAddress      = 95;
static const unsigned int pageStartAddress      = 0;
static const unsigned int pageEndAddress        = 3;
static const Ssd1307::SegmentRemap segmentRemap =
                                           Ssd1307::SEGMENT_REMAP_COL_127_SEG_0;
static const Ssd1307::MemoryAddressingMode memoryAddressingMode =
                                       Ssd1307::MEMORY_ADDRESSING_MODE_VERTICAL;

/*------------------------------------------------------------------------------
 * Public constructors and destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
DisplayUsmpP23302::DisplayUsmpP23302(GpioPin& rstNGpioPin,
                                     GpioPin& vccEnGpioPin,
                                     GpioPin& pwrEnGpioPin,
                                     I2c& i2c) :
    Display(myFrame),
    mySsd1307(*(new Ssd1307I2c(displayStartLine,
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
                               i2c,
                               myData)))
{
    myFrame.data = myData.getItems();
    myFrame.size = myData.getMaxSize();
    myFrame.width = width;
    myFrame.height = height;
}

//------------------------------------------------------------------------------
DisplayUsmpP23302::DisplayUsmpP23302(GpioPin& rstNGpioPin,
                                     GpioPin& vccEnGpioPin,
                                     GpioPin& pwrEnGpioPin,
                                     GpioPin& chipSelectGpioPin,
                                     Spi& spi,
                                     GpioPin& dataCommandGpioPin) :
    Display(myFrame),
    mySsd1307(*(new Ssd1307Spi4Wire(displayStartLine,
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
                                    chipSelectGpioPin,
                                    spi,
                                    dataCommandGpioPin,
                                    myData)))
{
    myFrame.data = myData.getItems();
    myFrame.size = myData.getMaxSize();
    myFrame.width = width;
    myFrame.height = height;
}

/*------------------------------------------------------------------------------
 * Private methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Display::Frame& DisplayUsmpP23302::driverGetFrame()
{
    return myFrame;
}

//------------------------------------------------------------------------------
Display::Error DisplayUsmpP23302::driverEnable(const bool enable)
{
    myData.clear();
    
    mySsd1307.enable(enable);

    return ERROR_NONE;
}

//------------------------------------------------------------------------------
Display::Error DisplayUsmpP23302::driverConfigure(const Config& config)
{
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
Display::Error DisplayUsmpP23302::driverWriteFrame()
{
    myData.clear();
    myData.setSize(myData.getMaxSize());
    
    if (mySsd1307.writeData(myData, false) != Ssd1307::ERROR_NONE)
    {
        return ERROR_COMMUNICATION;
    }
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
Display::Error DisplayUsmpP23302::driverClear()
{
    if (mySsd1307.clear() != Ssd1307::ERROR_NONE)
    {
        return ERROR_COMMUNICATION;
    }
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
Display::Error DisplayUsmpP23302::driverSetBrightnessPercent(
                                                  const float brightnessPercent)
{
    if (mySsd1307.setBrightnessPercent(brightnessPercent) !=
                                                            Ssd1307::ERROR_NONE)
    {
        return ERROR_COMMUNICATION;
    }

    return ERROR_NONE;
}
