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
 * @file TimerStm32f4xx.h
 * @author Ben Minerd
 * @date 5/15/2013
 * @brief TimerStm32f4xx class.
 */

#ifndef _TIMER_STM32F4XX_H_
#define _TIMER_STM32F4XX_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <Timer.h>
#include <GpioPinStm32f4xx.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

#define TIMER_STM32F4XX_ID_COUNT            14
#define TIMER_STM32F4XX_CHANNEL_ID_COUNT    4

/*------------------------------------------------------------------------------
 * Class
 *----------------------------------------------------------------------------*/

class TimerStm32f4xx : public Timer
{
public:
    
    /*--------------------------------------------------------------------------
     * Public enumerations
     *------------------------------------------------------------------------*/
    
    /**
     * @brief Enumeration of timers.
     */
    enum Id
    {
        ID_1 = 0,
        ID_2,
        ID_3,
        ID_4,
        ID_5,
        ID_6,
        ID_7,
        ID_8,
        ID_9,
        ID_10,
        ID_11,
        ID_12,
        ID_13,
        ID_14
    };

    /**
     * @brief Enumeration of timer channels.
     */
    enum ChannelId
    {
        CHANNEL_ID_1 = 0,
        CHANNEL_ID_2,
        CHANNEL_ID_3,
        CHANNEL_ID_4
    };
    
    /*--------------------------------------------------------------------------
     * Public structures
     *------------------------------------------------------------------------*/
    
    struct Config
    {
        
    };
    
    /*--------------------------------------------------------------------------
     * Public static methods
     *------------------------------------------------------------------------*/
    
    // Verify channelId default parameter!
    static TimerStm32f4xx& get(const Id id,
                               const ChannelId channelId = CHANNEL_ID_1,
                               GpioPinStm32f4xx* gpioPin = NULL_POINTER);
    
private:
    
    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/
    
    const Id myId;
    
    const ChannelId myChannelId;
    
    TIM_TypeDef* myTimer;
    
    GpioPinStm32f4xx* myGpioPin;
    
    float myInputClockFrequencyHz;
    
    float myOutputClockFrequencyHz;
    
    uint32_t myPeriod;
    
    /*--------------------------------------------------------------------------
     * Private constructors and destructors
     *------------------------------------------------------------------------*/
    
    // Verify channelId default parameter!
    TimerStm32f4xx(const Id id,
                   const ChannelId channelId,
                   GpioPinStm32f4xx* gpioPin);
    
    /*--------------------------------------------------------------------------
     * Private implemented methods
     *------------------------------------------------------------------------*/
    
    Error driverEnable(const bool enable);
    
    Error driverConfigure(const Timer::Config& config);
    
    Error driverPwmConfigure(const PwmConfig& pwmConfig);
};

#endif // _TIMER_STM32F4XX_H_
