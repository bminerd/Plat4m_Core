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
 * @file GpioPinIoExpanderMcp23008.h
 * @author Ben Minerd
 * @date 3/26/2013
 * @brief GpioPinIoExpanderMcp23008 class.
 */

#ifndef _GPIO_PIN_IO_EXPANDER_MCP23008_H_
#define _GPIO_PIN_IO_EXPANDER_MCP23008_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <GpioPin.h>
#include <IoExpanderMcp23008.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Structures
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

class GpioPinIoExpanderMcp23008 : public GpioPin
{
public:
    
    /*--------------------------------------------------------------------------
     * Public enumerations
     *------------------------------------------------------------------------*/
    
    /**
     * @brief Enumeration of IO expander registers.
     */
    enum PinId
    {
        PIN_ID_0 = 0,
        PIN_ID_1,
        PIN_ID_2,
        PIN_ID_3,
        PIN_ID_4,
        PIN_ID_5,
        PIN_ID_6,
        PIN_ID_7
    };
    
    /*--------------------------------------------------------------------------
     * Public structures
     *------------------------------------------------------------------------*/
    
    /*--------------------------------------------------------------------------
     * Public constructors and destructors
     *------------------------------------------------------------------------*/
    
    GpioPinIoExpanderMcp23008(const PinId pinId,
                              IoExpanderMcp23008& ioExpander);
    
private:
    
    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/
    
    const PinId myPinId;
    
    Level myLevel;
    
    IoExpanderMcp23008& myIoExpander;
    
    /*--------------------------------------------------------------------------
     * Private implemented methods
     *------------------------------------------------------------------------*/
    
    Error driverEnable(const bool enable);
    
    Error driverConfigure(const Config& config);
    
    Error driverSetLevel(const Level level);
    
    Error driverGetLevel(Level& level);
    
    Error driverReadLevel(Level& level);
    
    Error driverToggleLevel();
};

#endif // _GPIO_PIN_IO_EXPANDER_MCP23008_H_
