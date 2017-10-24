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
 * @file Ssd1307.h
 * @author Ben Minerd
 * @date 4/23/2014
 * @brief Ssd1307 class.
 */

#ifndef _SSD1307_H_
#define _SSD1307_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <GpioPin.h>
#include <SlaveDevice.h>
#include <ByteArray.h>
#include <EnableLine.h>

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

class Ssd1307
{
public:
    
    /*--------------------------------------------------------------------------
     * Public enumerations
     *------------------------------------------------------------------------*/
    
    enum Error
    {
        ERROR_NONE,
        ERROR_NOT_ENABLED,
        ERROR_COMMUNICATION
    };
    
    enum MemoryAddressingMode
    {
        MEMORY_ADDRESSING_MODE_HORIZONTAL   = 0,
        MEMORY_ADDRESSING_MODE_VERTICAL     = 1,
        MEMORY_ADDRESSING_MODE_PAGE         = 2
    };
    
    enum SegmentRemap
    {
        SEGMENT_REMAP_COL_0_SEG_0   = 0xA0,
        SEGMENT_REMAP_COL_127_SEG_0 = 0xA1
    };
    
    /*--------------------------------------------------------------------------
     * Public methods
     *------------------------------------------------------------------------*/
    
    Error enable(const bool enable);
    
    Error setColumnAddress(const unsigned int columnStartAddress,
                           const unsigned int columnEndAddress);
    
    Error writeData(ByteArray& byteArray,
                    const bool waitUntilDone = true);
    
    Error clear();
    
    Error setBrightnessPercent(const float brightnessPercent);
    
protected:
    
    /*--------------------------------------------------------------------------
     * Protected constructors and destructors
     *------------------------------------------------------------------------*/
    
    Ssd1307(const unsigned int displayStartLine,
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
            ByteArray& byteArray);
    
private:
    
    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/
    
    const unsigned int myDisplayStartLine;
    
    const unsigned int myDisplayOffset;
    
    const unsigned int myColumnStartAddress;
    
    const unsigned int myColumnEndAddress;
    
    const unsigned int myPageStartAddress;
    
    const unsigned int myPageEndAddress;
    
    const SegmentRemap mySegmentRemap;
    
    const MemoryAddressingMode myMemoryAddressingMode;
    
    EnableLine myRstNEnableLine;
    
    EnableLine myVccEnEnableLine;
    
    EnableLine myPwrEnEnableLine;
    
    ByteArray& myByteArray;
    
    bool myIsEnabled;
    
    /*--------------------------------------------------------------------------
     * Private pure virtual methods
     *------------------------------------------------------------------------*/
    
    virtual Error driverEnable(const bool enable) = 0;
    
    virtual Error driverWriteCommand(const ByteArray& byteArray) = 0;
    
    virtual Error driverWriteData(const ByteArray& byteArray,
                                  const bool waitUntilDone) = 0;
};

#endif // _SSD1307_H_
