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
 * @file TimerStm32f4xx.cpp
 * @author Ben Minerd
 * @date 5/15/2013
 * @brief TimerStm32f4xx class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <TimerStm32f4xx.h>
#include <ProcessorStm32f4xx.h>

#include <stm32f4xx_tim.h>

using Plat4m::TimerStm32f4xx;
using Plat4m::Module;
using Plat4m::GpioStm32f4xx;

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

static TIM_TypeDef* timerMap[TIMER_STM32F4XX_ID_COUNT] =
{
    TIM1,   /// TimerStm32f4xx::ID_1
    TIM2,   /// TimerStm32f4xx::ID_2
    TIM3,   /// TimerStm32f4xx::ID_3
    TIM4,   /// TimerStm32f4xx::ID_4
    TIM5,   /// TimerStm32f4xx::ID_5
    TIM6,   /// TimerStm32f4xx::ID_6
    TIM7,   /// TimerStm32f4xx::ID_7
    TIM8,   /// TimerStm32f4xx::ID_8
    TIM9,   /// TimerStm32f4xx::ID_9
    TIM10,  /// TimerStm32f4xx::ID_10
    TIM11,  /// TimerStm32f4xx::ID_11
    TIM12,  /// TimerStm32f4xx::ID_12
    TIM13,  /// TimerStm32f4xx::ID_13
    TIM14   /// TimerStm32f4xx::ID_14
};

static const uint32_t clockMap[TIMER_STM32F4XX_ID_COUNT] =
{
    RCC_APB2Periph_TIM1,    /// TimerStm32f4xx::ID_1
    RCC_APB1Periph_TIM2,    /// TimerStm32f4xx::ID_2
    RCC_APB1Periph_TIM3,    /// TimerStm32f4xx::ID_3
    RCC_APB1Periph_TIM4,    /// TimerStm32f4xx::ID_4
    RCC_APB1Periph_TIM5,    /// TimerStm32f4xx::ID_5
    RCC_APB1Periph_TIM6,    /// TimerStm32f4xx::ID_6
    RCC_APB1Periph_TIM7,    /// TimerStm32f4xx::ID_7
    RCC_APB2Periph_TIM8,    /// TimerStm32f4xx::ID_8
    RCC_APB2Periph_TIM9,    /// TimerStm32f4xx::ID_9
    RCC_APB2Periph_TIM10,   /// TimerStm32f4xx::ID_10
    RCC_APB2Periph_TIM11,   /// TimerStm32f4xx::ID_11
    RCC_APB1Periph_TIM12,   /// TimerStm32f4xx::ID_12
    RCC_APB1Periph_TIM13,   /// TimerStm32f4xx::ID_13
    RCC_APB1Periph_TIM14    /// TimerStm32f4xx::ID_14
};

static const GpioStm32f4xx::AlternateFunction
                                alternateFunctionMap[TIMER_STM32F4XX_ID_COUNT] =
{
    GpioStm32f4xx::ALTERNATE_FUNCTION_TIM1,     /// TimerStm32f4xx::ID_1
    GpioStm32f4xx::ALTERNATE_FUNCTION_TIM2,     /// TimerStm32f4xx::ID_2
    GpioStm32f4xx::ALTERNATE_FUNCTION_TIM3,     /// TimerStm32f4xx::ID_3
    GpioStm32f4xx::ALTERNATE_FUNCTION_TIM4,     /// TimerStm32f4xx::ID_4
    GpioStm32f4xx::ALTERNATE_FUNCTION_TIM5,     /// TimerStm32f4xx::ID_5
    (GpioStm32f4xx::AlternateFunction) 0,       /// TimerStm32f4xx::ID_6
    (GpioStm32f4xx::AlternateFunction) 0,       /// TimerStm32f4xx::ID_7
    GpioStm32f4xx::ALTERNATE_FUNCTION_TIM8,     /// TimerStm32f4xx::ID_8
    GpioStm32f4xx::ALTERNATE_FUNCTION_TIM9,     /// TimerStm32f4xx::ID_9
    GpioStm32f4xx::ALTERNATE_FUNCTION_TIM10,    /// TimerStm32f4xx::ID_10
    GpioStm32f4xx::ALTERNATE_FUNCTION_TIM11,    /// TimerStm32f4xx::ID_11
    GpioStm32f4xx::ALTERNATE_FUNCTION_TIM12,    /// TimerStm32f4xx::ID_12
    GpioStm32f4xx::ALTERNATE_FUNCTION_TIM13,    /// TimerStm32f4xx::ID_13
    GpioStm32f4xx::ALTERNATE_FUNCTION_TIM14     /// TimerStm32f4xx::ID_14
};

static const uint16_t counterModeMap[] =
{
    TIM_CounterMode_Up,     /// Timer::MODE_COUNTING_UP
    TIM_CounterMode_Down,   /// Timer::MODE_COUNTING_DOWN
    TIM_CounterMode_Up,     /// Timer::MODE_COUNTING_UP_DOWN
    TIM_CounterMode_Up,     /// Timer::MODE_INPUT_CAPTURE
    TIM_CounterMode_Up,     /// Timer::MODE_OUTPUT_COMPARE
    TIM_CounterMode_Up,     /// Timer::MODE_PWM_INPUT
    TIM_CounterMode_Up,     /// Timer::MODE_PWM_OUTPUT
    TIM_CounterMode_Up,     /// Timer::MODE_PULSE
};

static const uint16_t polarityMap[] =
{
    TIM_OCPolarity_High,    /// Timer::POLARITY_HIGH
    TIM_OCPolarity_Low      /// Timer::POLARITY_LOW
};

static const GpioStm32f4xx::OutputSpeed gpioSpeed =
                                             GpioStm32f4xx::OUTPUT_SPEED_100MHZ;

static TimerStm32f4xx* objectMap[TIMER_STM32F4XX_ID_COUNT]
                       [TIMER_STM32F4XX_CHANNEL_ID_COUNT];

static const float maxFrequencyErrorPercent = 0.5f;

/*------------------------------------------------------------------------------
 * Public static methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
TimerStm32f4xx& TimerStm32f4xx::get(const Id id,
                                    const ChannelId channelId,
                                    GpioPinStm32f4xx* gpioPin)
{
    if (IS_NULL_POINTER(objectMap[id][channelId]))
    {
        objectMap[id][channelId] = new TimerStm32f4xx(id, channelId, gpioPin);
    }
    else
    {
        // Error?
    }
    
    return *(objectMap[id][channelId]);
}

/*------------------------------------------------------------------------------
 * Private constructors and destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
TimerStm32f4xx::TimerStm32f4xx(const Id id,
                               const ChannelId channelId,
                               GpioPinStm32f4xx* gpioPin) :
    Timer(),
    myId(id),
    myChannelId(channelId),
    myTimer(timerMap[id]),
    myGpioPin(NULL_POINTER)
{
    // Timers 6 and 7 do not have a related GPIO pin
    if ((myId != ID_6) && (myId != ID_7))
    {
        myGpioPin = gpioPin;
    }
    
    switch (myId)
    {
        case ID_1:
        // Fall through
        case ID_8:
        // Fall through
        case ID_9:
        // Fall through
        case ID_10:
        // Fall through
        case ID_11:
        {
            unsigned int multiplier;
            
            // APB2
            if (ProcessorStm32f4xx::getApb2ClockPrescaler() ==
                ProcessorStm32f4xx::APB2_CLOCK_PRESCALER_1)
            {
                multiplier = 1;
            }
            else
            {
                multiplier = 2;
            }
            
            myInputClockFrequencyHz =
                         ProcessorStm32f4xx::getApb2ClockFrequencyHz() * multiplier;
            
            break;
        }
        case ID_2:
        // Fall through
        case ID_3:
        // Fall through
        case ID_4:
        // Fall through
        case ID_5:
        // Fall through
        case ID_6:
        // Fall through
        case ID_7:
        // Fall through
        case ID_12:
        // Fall through
        case ID_13:
        // Fall through
        case ID_14:
        {
            unsigned int multiplier;
            
            // APB1
            if (ProcessorStm32f4xx::getApb1ClockPrescaler() ==
                ProcessorStm32f4xx::APB1_CLOCK_PRESCALER_1)
            {
                multiplier = 1;
            }
            else
            {
                multiplier = 2;
            }
            
            myInputClockFrequencyHz =
                     ProcessorStm32f4xx::getApb1ClockFrequencyHz() * multiplier;
            
            break;
        }
    }
}

/*------------------------------------------------------------------------------
 * Private implemented methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Timer::Error TimerStm32f4xx::driverEnable(const bool enable)
{
    if (IS_VALID_POINTER(myGpioPin))
    {
        myGpioPin->enable(enable);
        
        if (enable)
        {
            // Generic GpioPin configuration
            GpioPin::Config gpioConfig;
            gpioConfig.mode     = GpioPin::MODE_ALTERNATE_FUNCTION;
//            gpioConfig.resistor = GpioPin::RESISTOR_NONE;
            gpioConfig.resistor = GpioPin::RESISTOR_PULL_DOWN;
            
            myGpioPin->configure(gpioConfig);
            
            // Specific GpioPinStm32f4xx configuration
            GpioStm32f4xx::Config gpioDriverConfig;
            gpioDriverConfig.alternateFunction  = alternateFunctionMap[myId];
            gpioDriverConfig.outputSpeed        = gpioSpeed;
            
            myGpioPin->configureDriver(gpioDriverConfig);
        }
    }
    
    switch (myId)
    {
        case ID_1:
        // Fall through
        case ID_8:
        // Fall through
        case ID_9:
        // Fall through
        case ID_10:
        // Fall through
        case ID_11:
        {
            // APB2
            RCC_APB2PeriphClockCmd(clockMap[myId], (FunctionalState) enable);
            
            break;
        }
        case ID_2:
        // Fall through
        case ID_3:
        // Fall through
        case ID_4:
        // Fall through
        case ID_5:
        // Fall through
        case ID_6:
        // Fall through
        case ID_7:
        // Fall through
        case ID_12:
        // Fall through
        case ID_13:
        // Fall through
        case ID_14:
        {
            // APB1
            RCC_APB1PeriphClockCmd(clockMap[myId], (FunctionalState) enable);
            
            break;
        }
    }
    
    TIM_Cmd(myTimer, (FunctionalState) enable);
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
Timer::Error TimerStm32f4xx::driverConfigure(const Timer::Config& config)
{
    // TODO: Incorporate clock division (for reeeeally slow timers)
    
    uint32_t prescaler = 1;
    uint32_t period = 1;
    float outputClockFrequencyHz;
    
    switch (config.resolution)
    {
        case RESOLUTION_MIN:
        {
            prescaler = (uint32_t) round(myInputClockFrequencyHz /
                                         config.frequencyHz);
            
            // Prescaler is 16-bit register
            if (prescaler > 65536)
            {
                prescaler = 65536;
            }
            
            outputClockFrequencyHz = (myInputClockFrequencyHz /
                                      (float) (prescaler * period));
            
            float errorPercent = ((outputClockFrequencyHz -
                                   config.frequencyHz)  /
                                  config.frequencyHz)           *
                                 100.0f;
            
            while (fabs(errorPercent) > maxFrequencyErrorPercent)
            {
                if (errorPercent > 0.0f)
                {
                    float ratio = (outputClockFrequencyHz / config.frequencyHz);
                    period = (uint32_t) round((float) period * ratio);
                }
                
                prescaler = (uint32_t) round(myInputClockFrequencyHz /
                                             (config.frequencyHz *
                                              (float) period));
                
                // All timers have a 16-bit counter register
                if (prescaler > 65536)
                {
                    prescaler = 65536;
                }
                
                outputClockFrequencyHz = (myInputClockFrequencyHz /
                                          (float) (prescaler * period));
                
                errorPercent = ((outputClockFrequencyHz - config.frequencyHz) /
                                config.frequencyHz) *
                                100.0f;
            }
            
            break;
        }
        case RESOLUTION_MAX:
        {
            period = (uint32_t) round(myInputClockFrequencyHz /
                                      config.frequencyHz);
            
            // All timers except TIM2 and TIM5 have a 16-bit counter register
            if ((myId != ID_2) && (myId != ID_5) && (period > 65536))
            {
                period = 65536;
            }
            
            outputClockFrequencyHz = (myInputClockFrequencyHz /
                                      (prescaler * period));
            
            float errorPercent = ((outputClockFrequencyHz -
                                   config.frequencyHz) /
                                  config.frequencyHz) *
                                 100.0f;
            
            while (fabs(errorPercent) > maxFrequencyErrorPercent)
            {
                if (errorPercent > 0.0f)
                {
                    float ratio = (outputClockFrequencyHz / config.frequencyHz);
                    prescaler = (uint32_t) round((float) prescaler * ratio);
                }
                
                period = (uint32_t) round(myInputClockFrequencyHz /
                                          (prescaler * config.frequencyHz));
                
                // All timers except TIM2 and TIM5 have a 16-bit counter
                // register
                if ((myId != ID_2) && (myId != ID_5) && (period > 65536))
                {
                    period = 65536;
                }
                
                outputClockFrequencyHz = (myInputClockFrequencyHz /
                                          (float) (prescaler * period));
                
                errorPercent = ((outputClockFrequencyHz - config.frequencyHz) /
                                config.frequencyHz) *
                               100.0f;
            }
            
            break;
        }
        case RESOLUTION_LEVELS:
        {
            prescaler = (uint32_t) round(myInputClockFrequencyHz /
                                         (config.frequencyHz *
                                          (float) config.resolutionLevels));
            
            // Prescaler is 16-bit register
            if (prescaler > 65536)
            {
                prescaler = 65536;
            }
            
            outputClockFrequencyHz = (myInputClockFrequencyHz /
                                      (float) (prescaler * period));
            
            float errorPercent = ((outputClockFrequencyHz -
                                   config.frequencyHz)  /
                                  config.frequencyHz)           *
                                 100.0f;
            
            while (fabs(errorPercent) > maxFrequencyErrorPercent)
            {
                if (errorPercent > 0.0f)
                {
                    float ratio = (outputClockFrequencyHz / config.frequencyHz);
                    period = (uint32_t) round((float) period * ratio);
                }
                
                prescaler = (uint32_t) round(myInputClockFrequencyHz /
                                             (config.frequencyHz *
                                              (float) period));
                
                // All timers have a 16-bit counter register
                if (prescaler > 65536)
                {
                    prescaler = 65536;
                }
                
                outputClockFrequencyHz = (myInputClockFrequencyHz /
                                          (float) (prescaler * period));
                
                errorPercent = ((outputClockFrequencyHz - config.frequencyHz) /
                                config.frequencyHz) *
                                100.0f;
            }
            
            break;
        }
    }
    
    myOutputClockFrequencyHz = outputClockFrequencyHz;
    myPeriod = period;
    
    TIM_TimeBaseInitTypeDef baseInit;
    baseInit.TIM_Prescaler      = prescaler - 1;
    baseInit.TIM_Period         = period - 1;
    baseInit.TIM_ClockDivision  = 0;
    baseInit.TIM_CounterMode    = counterModeMap[config.mode];
    
    TIM_TimeBaseInit(myTimer, &baseInit);
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
Timer::Error TimerStm32f4xx::driverPwmConfigure(const PwmConfig& pwmConfig)
{
    TIM_OCInitTypeDef outputCompareInit;
    outputCompareInit.TIM_OCMode        = TIM_OCMode_PWM1;
    outputCompareInit.TIM_OutputState   = TIM_OutputState_Enable;
    outputCompareInit.TIM_Pulse         = (uint32_t)
                                          round((pwmConfig.dutyCyclePercent /
                                                 100.0f) *
                                                (float) myPeriod);
    outputCompareInit.TIM_OCPolarity    = polarityMap[pwmConfig.polarity];
    
    switch (myChannelId)
    {
        case CHANNEL_ID_1:
        {
            TIM_OC1Init(myTimer, &outputCompareInit);
            TIM_OC1PreloadConfig(myTimer, TIM_OCPreload_Enable);
            
            break;
        }
        case CHANNEL_ID_2:
        {
            TIM_OC2Init(myTimer, &outputCompareInit);
            TIM_OC2PreloadConfig(myTimer, TIM_OCPreload_Enable);
            
            break;
        }
        case CHANNEL_ID_3:
        {
            TIM_OC3Init(myTimer, &outputCompareInit);
            TIM_OC3PreloadConfig(myTimer, TIM_OCPreload_Enable);
            
            break;
        }
        case CHANNEL_ID_4:
        {
            TIM_OC4Init(myTimer, &outputCompareInit);
            TIM_OC4PreloadConfig(myTimer, TIM_OCPreload_Enable);
            
            break;
        }
    }
    
    TIM_ARRPreloadConfig(myTimer, ENABLE);
    
    return ERROR_NONE;
}
