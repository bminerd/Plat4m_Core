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
 * @file Ssd1307.cpp
 * @author Ben Minerd
 * @date 4/23/2014
 * @brief Ssd1307 class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Ssd1307.h>

#include <System.h>

/*------------------------------------------------------------------------------
 * Public constructors and destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Ssd1307::Ssd1307(const unsigned int displayStartLine,
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
                 ByteArray& byteArray) :
    myDisplayStartLine(displayStartLine),
    myDisplayOffset(displayOffset),
    myColumnStartAddress(columnStartAddress),
    myColumnEndAddress(columnEndAddress),
    myPageStartAddress(pageStartAddress),
    myPageEndAddress(pageEndAddress),
    mySegmentRemap(segmentRemap),
    myMemoryAddressingMode(memoryAddressingMode),
    myRstNEnableLine(EnableLine::MODE_OUTPUT,
                     EnableLine::ACTIVE_LEVEL_LOW,
                     rstNGpioPin,
                     false),
    myVccEnEnableLine(EnableLine::MODE_OUTPUT,
                      EnableLine::ACTIVE_LEVEL_HIGH,
                      vccEnGpioPin,
                      false),
    myPwrEnEnableLine(EnableLine::MODE_OUTPUT,
                      EnableLine::ACTIVE_LEVEL_HIGH,
                      pwrEnGpioPin,
                      false),
    myByteArray(byteArray),
    myIsEnabled(false)
{
}

/*------------------------------------------------------------------------------
 * Public methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Ssd1307::Error Ssd1307::enable(const bool enable)
{
    if (enable == myIsEnabled)
    {
        return ERROR_NONE;
    }
    
    Error error;
    
    error = driverEnable(enable);
    
    if (error == ERROR_NONE)
    {
        myIsEnabled = enable;
    }
    
    myByteArray.clear();
    
    if (enable)
    {
        myRstNEnableLine.enable(true);
        myVccEnEnableLine.enable(true);
        myPwrEnEnableLine.enable(true);

        // Enable V_DD
        myPwrEnEnableLine.setActive(true);
        // Wait for V_DD to stabilize
        System::timeDelayMs(10);
        // Pull RST low
        myRstNEnableLine.setActive(true);
        System::timeDelayMs(1);
        // Push RST high
        myRstNEnableLine.setActive(false);
        System::timeDelayMs(1);
        
        myVccEnEnableLine.setActive(true);
        System::timeDelayMs(100);

        // Set display clock divide ratio / oscillator frequency to ???
        myByteArray.append(0xD5);
        myByteArray.append(0xF0);
        // Set multiplex ratio to ???
        myByteArray.append(0xA8);
        myByteArray.append(31);
        // Set display offset to ???
        myByteArray.append(0xD3);
        myByteArray.append(myDisplayOffset);
        // Set display start line to ???
        myByteArray.append(0x40 | myDisplayStartLine);
        // Set segment re-map to???
        myByteArray.append(mySegmentRemap);
        // Set COM scan direction to ???
        myByteArray.append(0xC0);
        // Set COM pins hardware configuration to ???
        myByteArray.append(0xDA);
        myByteArray.append(0x12);
        // Set contrast control to ???
        myByteArray.append(0x81);
        myByteArray.append(0xFF);
        // Set pre-charge period to ???
        myByteArray.append(0xD9);
        myByteArray.append(0x22);
        // Set VCOMH level to ???
        myByteArray.append(0xDB);
        myByteArray.append(0x30);
        // Set memory addressing mode to ???
        myByteArray.append(0x20);
        myByteArray.append(myMemoryAddressingMode);
        // Set column start and end address
        myByteArray.append(0x21);
        myByteArray.append(myColumnStartAddress);
        myByteArray.append(myColumnEndAddress);
        // Set page start and end address
        myByteArray.append(0x22);
        myByteArray.append(myPageStartAddress);
        myByteArray.append(myPageEndAddress);
        // Set entire display on/off
        myByteArray.append(0xA4);
        // Set display normal
        myByteArray.append(0xA6);
        
        error = driverWriteCommand(myByteArray);
        
        if (error != ERROR_NONE)
        {
            return error;
        }

        error = clear();

        if (error != ERROR_NONE)
        {
            return error;
        }

        myByteArray.clear();
        myByteArray.append(0xAF);

        error = driverWriteCommand(myByteArray);
        
        if (error != ERROR_NONE)
        {
            return error;
        }

        System::timeDelayMs(100);
    }
    else
    {
        myByteArray.append(0xAE);

        error = driverWriteCommand(myByteArray);
        
        if (error != ERROR_NONE)
        {
            return error;
        }

        // Disable V_CC
        myVccEnEnableLine.setActive(false);
        System::timeDelayMs(100);
        // Disable V_DD
        myPwrEnEnableLine.setActive(false);
        
        myRstNEnableLine.enable(false);
        myVccEnEnableLine.enable(false);
        myPwrEnEnableLine.enable(false);
    }
    
    return error;
}

//------------------------------------------------------------------------------
Ssd1307::Error Ssd1307::setColumnAddress(const unsigned int columnStartAddress,
                                         const unsigned int columnEndAddress)
{
    if (!myIsEnabled)
    {
        return ERROR_NOT_ENABLED;
    }
    
    Error error;
    
    // Reset memory pointer to top corner of display
    myByteArray.clear();
    myByteArray.append(0x21);
    myByteArray.append(columnStartAddress);
    myByteArray.append(columnEndAddress);
    
    error = driverWriteCommand(myByteArray);
    
    return error;
}

//------------------------------------------------------------------------------
Ssd1307::Error Ssd1307::writeData(ByteArray& byteArray,
                                  const bool waitUntilDone)
{
    if (!myIsEnabled)
    {
        return ERROR_NOT_ENABLED;
    }
    
    Error error;
    
    error = driverWriteData(byteArray, waitUntilDone);
    
    return error;
}

//------------------------------------------------------------------------------
Ssd1307::Error Ssd1307::clear()
{
    if (!myIsEnabled)
    {
        return ERROR_NOT_ENABLED;
    }
    
    Error error;
    
    error = setColumnAddress(myColumnStartAddress, myColumnEndAddress);
        
    if (error != ERROR_NONE)
    {
        return error;
    }

    myByteArray.clear();
    
    myByteArray.setValue(0);
    myByteArray.setSize(myByteArray.getMaxSize());

    error = driverWriteData(myByteArray, true);

    if (error != ERROR_NONE)
    {
        return error;
    }
    
    return error;
}

//------------------------------------------------------------------------------
Ssd1307::Error Ssd1307::setBrightnessPercent(const float brightnessPercent)
{
    if (!myIsEnabled)
    {
        return ERROR_NOT_ENABLED;
    }
    
    Error error;
    
    myByteArray.clear();
    myByteArray.append(0x81);
    myByteArray.append((uint8_t) round(255.0f *(brightnessPercent / 100.0f)));

    error = driverWriteCommand(myByteArray);
        
    if (error != ERROR_NONE)
    {
        return error;
    }

    return error;
}