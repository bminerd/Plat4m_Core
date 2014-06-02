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
 * @file Ssd1307I2c.cpp
 * @author Ben Minerd
 * @date 4/23/2014
 * @brief Ssd1307I2c class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Ssd1307I2c.h>

#include <System.h>

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

static const uint8_t i2cAddress     = 0x3C;
static const uint8_t commandFlag    = 0x80;
static const uint8_t dataFlag       = 0x40;

/*------------------------------------------------------------------------------
 * Public constructors and destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Ssd1307I2c::Ssd1307I2c(const unsigned int displayStartLine,
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
    myI2cDevice(i2cAddress, i2c)
{
}

/*------------------------------------------------------------------------------
 * Private methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Ssd1307::Error Ssd1307I2c::driverEnable(const bool enable)
{
    myI2cDevice.enable(enable);
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
Ssd1307::Error Ssd1307I2c::driverWriteCommand(const ByteArray& byteArray)
{
    myByteArray.clear();
    
    int size = byteArray.getSize();
    
    for (int i = 0; i < size; i++)
    {
        myByteArray.append(commandFlag);
        myByteArray.append(byteArray[i]);
    }
    
    if (myI2cDevice.tx(myByteArray) != SlaveDevice::ERROR_NONE)
    {
        return ERROR_COMMUNICATION;
    }
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
Ssd1307::Error Ssd1307I2c::driverWriteData(const ByteArray& byteArray,
                                           const bool waitUntilDone)
{
    myByteArray.clear();
    myByteArray.append(dataFlag);
    myByteArray.append(byteArray);
    
    if (myI2cDevice.tx(myByteArray, waitUntilDone) != SlaveDevice::ERROR_NONE)
    {
        return ERROR_COMMUNICATION;
    }
    
    return ERROR_NONE;
}