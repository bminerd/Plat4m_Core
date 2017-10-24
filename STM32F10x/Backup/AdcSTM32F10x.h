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
 * @file AdcStm32f4xx.h
 * @author Ben Minerd
 * @date 4/10/13
 * @brief AdcStm32f4xx class.
 */

#ifndef _ADC_STM32F4XX_H_
#define _ADC_STM32F4XX_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <Adc.h>
#include <GpioPinStm32f4xx.h>

#include <stm32f4xx.h>
#include <stm32f4xx_adc.h>
#include <stm32f4xx_rcc.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

#define ADC_STM32F4XX_ID_COUNT          3
#define ADC_STM32F4XX_CHANNEL_ID_COUNT  16

namespace Plat4m
{

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

class AdcStm32f4xx : public Adc
{
public:
    
    /*--------------------------------------------------------------------------
     * Public enumerations
     *------------------------------------------------------------------------*/
    
    /**
     * @brief Enumeration of ADC IDs.
     */
    enum Id
    {
        ID_1 = 0,
        ID_2,
        ID_3
    };
    
    /**
     * @brief Enumeration of ADC channels.
     */
    enum ChannelId
    {
        CHANNEL_ID_0 = 0,
        CHANNEL_ID_1,
        CHANNEL_ID_2,
        CHANNEL_ID_3,
        CHANNEL_ID_4,
        CHANNEL_ID_5,
        CHANNEL_ID_6,
        CHANNEL_ID_7,
        CHANNEL_ID_8,
        CHANNEL_ID_9,
        CHANNEL_ID_10,
        CHANNEL_ID_11,
        CHANNEL_ID_12,
        CHANNEL_ID_13,
        CHANNEL_ID_14,
        CHANNEL_ID_15
    };
    
    /*--------------------------------------------------------------------------
     * Public static methods
     *------------------------------------------------------------------------*/
    
    static AdcStm32f4xx& get(const Id id,
                             const ChannelId channelId,
                             GpioPinStm32f4xx& gpioPin);
    
private:
    
    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/
    
    const Id myId;
    
    const ChannelId myChannelId;
    
    ADC_TypeDef* myAdc;
    
    GpioPinStm32f4xx& myGpioPin;
    
    /*--------------------------------------------------------------------------
     * Private constructors and destructors
     *------------------------------------------------------------------------*/
    
    AdcStm32f4xx(const Id id,
                 const ChannelId channelId,
                 GpioPinStm32f4xx& gpioPin);
    
    /*--------------------------------------------------------------------------
     * Private implemented methods
     *------------------------------------------------------------------------*/
    
    Error driverEnable(const bool enable);
    
    Error driverConfigure(const Config& config);
    
    Error driverReadVoltage(float& voltage);
};

}; // namespace Plat4m

#endif // _ADC_STM32F4XX_H_
