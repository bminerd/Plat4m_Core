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
 * @file GpioPinStm32f4xx.h
 * @author Ben Minerd
 * @date 3/22/13
 * @brief GpioPinStm32f4xx class.
 */

#ifndef _GPIO_PIN_STM32F4XX_H_
#define _GPIO_PIN_STM32F4XX_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <GpioPin.h>
#include <GpioStm32f4xx.h>

#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

class GpioPinStm32f4xx : public GpioPin
{
public:
    
    /*--------------------------------------------------------------------------
     * Public static methods
     *------------------------------------------------------------------------*/
    
    static GpioPinStm32f4xx& get(const GpioStm32f4xx::PortId portId,
                                 const GpioStm32f4xx::PinId pinId);
     
    static GpioPinStm32f4xx& get(const GpioPin::Id& id);
    
    /*--------------------------------------------------------------------------
     * Public methods
     *------------------------------------------------------------------------*/
    
    GpioStm32f4xx::PortId getPortId() const;
    
    GpioStm32f4xx::PinId getPinId() const;
    
    void configureDriver(GpioStm32f4xx::Config& config);
    
private:
    
    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/
    
    const GpioStm32f4xx::PortId myPortId;
    
    const GpioStm32f4xx::PinId myPinId;
    
    const uint16_t myPin;
    
    GPIO_TypeDef* myPort;
    
    /*--------------------------------------------------------------------------
     * Private constructors and destructors
     *------------------------------------------------------------------------*/
    
    GpioPinStm32f4xx(const GpioStm32f4xx::PortId portId,
                     const GpioStm32f4xx::PinId pinId);
    
    /*--------------------------------------------------------------------------
     * Private implemented methods
     *------------------------------------------------------------------------*/
    
    Error driverEnable(const bool enable);
    
    Error driverConfigure(const Config& config);
    
    Error driverSetLevel(const Level level);
    
    Error driverGetLevel(Level& level);
    
    Error driverReadLevel(Level& level);
    
    Error driverToggleLevel();
    
    /*--------------------------------------------------------------------------
     * Private methods
     *------------------------------------------------------------------------*/
    
    void setOutputType(const GpioStm32f4xx::OutputType outputType);
    
    void setOutputSpeed(const GpioStm32f4xx::OutputSpeed outputSpeed);
};

#endif // _GPIO_PIN_STM32F4XX_H_