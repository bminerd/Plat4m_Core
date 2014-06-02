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
 * @file InterruptStm32f4xx.h
 * @author Ben Minerd
 * @date 4/17/2013
 * @brief InterruptStm32f4xx class.
 */

#ifndef _INTERRUPT_STM32F4XX_H_
#define _INTERRUPT_STM32F4XX_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <Interrupt.h>
#include <GpioPinStm32f4xx.h>
     
#include <stm32f4xx.h>
#include <stm32f4xx_exti.h>
#include <stm32f4xx_rcc.h>

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

class InterruptStm32f4xx : public Interrupt
{
public:
    
    /*--------------------------------------------------------------------------
     * Public enumerations
     *------------------------------------------------------------------------*/
    
    /**
     * @brief Enumeration of interrupts.
     */
    enum Id
    {
        ID_EXTERNAL_0 = 0,
        ID_EXTERNAL_1,
        ID_EXTERNAL_2,
        ID_EXTERNAL_3,
        ID_EXTERNAL_4,
        ID_EXTERNAL_5,
        ID_EXTERNAL_6,
        ID_EXTERNAL_7,
        ID_EXTERNAL_8,
        ID_EXTERNAL_9,
        ID_EXTERNAL_10,
        ID_EXTERNAL_11,
        ID_EXTERNAL_12,
        ID_EXTERNAL_13,
        ID_EXTERNAL_14,
        ID_EXTERNAL_15
    };
    
    /*--------------------------------------------------------------------------
     * Public static methods
     *------------------------------------------------------------------------*/
    
    static InterruptStm32f4xx& get(GpioPinStm32f4xx& gpioPin);
    
private:
    
    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/
    
    const Id myId;
    
    const GpioStm32f4xx::PortId myGpioPortId;
    
    /*--------------------------------------------------------------------------
     * Private constructors and destructors
     *------------------------------------------------------------------------*/
    
    InterruptStm32f4xx(GpioPinStm32f4xx& gpioPin);
    
    /*--------------------------------------------------------------------------
     * Private implemented methods
     *------------------------------------------------------------------------*/
    
    Error driverEnable(const bool enable);
    
    Error driverConfigure(const Config& config);
};

#endif // _INTERRUPT_STM32F4XX_H_
