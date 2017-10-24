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
 * @file AdcStm32f4xx.cpp
 * @author Ben Minerd
 * @date 4/10/13
 * @brief AdcStm32f4xx class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <AdcStm32f4xx.h>
#include <Processor.h>

using Plat4m::AdcStm32f4xx;

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 * @brief ADC map.
 */
static ADC_TypeDef* adcMap[ADC_STM32F4XX_ID_COUNT] =
{
    ADC1,   /// ADC_1
    ADC2,   /// ADC_2
    ADC3    /// ADC_3
};

/**
 * @brief ADC channel map.
 */
static const uint8_t idToChannelMap[ADC_STM32F4XX_CHANNEL_ID_COUNT] =
{
    ADC_Channel_0,  /// ADC_CHANNEL_0
    ADC_Channel_1,  /// ADC_CHANNEL_1
    ADC_Channel_2,  /// ADC_CHANNEL_2
    ADC_Channel_3,  /// ADC_CHANNEL_3
    ADC_Channel_4,  /// ADC_CHANNEL_4
    ADC_Channel_5,  /// ADC_CHANNEL_5
    ADC_Channel_6,  /// ADC_CHANNEL_6
    ADC_Channel_7,  /// ADC_CHANNEL_7
    ADC_Channel_8,  /// ADC_CHANNEL_8
    ADC_Channel_9,  /// ADC_CHANNEL_9
    ADC_Channel_10, /// ADC_CHANNEL_10
    ADC_Channel_11, /// ADC_CHANNEL_11
    ADC_Channel_12, /// ADC_CHANNEL_12
    ADC_Channel_13, /// ADC_CHANNEL_13
    ADC_Channel_14, /// ADC_CHANNEL_14
    ADC_Channel_15  /// ADC_CHANNEL_15
};

/**
 * @brief ADC clock map.
 */
static const uint32_t clockMap[ADC_STM32F4XX_ID_COUNT] =
{
    RCC_APB2Periph_ADC1,    /// ADC_1
    RCC_APB2Periph_ADC2,    /// ADC_2
    RCC_APB2Periph_ADC3     /// ADC_3
};

static AdcStm32f4xx* objectMap[ADC_STM32F4XX_ID_COUNT]
                              [ADC_STM32F4XX_CHANNEL_ID_COUNT];

/*------------------------------------------------------------------------------
 * Public static methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
AdcStm32f4xx& AdcStm32f4xx::get(const Id id,
                                const ChannelId channelId,
                                GpioPinStm32f4xx& gpioPin)
{
    if (IS_NULL_POINTER(objectMap[id][channelId]))
    {
        objectMap[id][channelId] = new AdcStm32f4xx(id, channelId, gpioPin);
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
AdcStm32f4xx::AdcStm32f4xx(const Id id,
                           const ChannelId channelId,
                           GpioPinStm32f4xx& gpioPin) :
    myId(id),
    myChannelId(channelId),
    myAdc(adcMap[id]),
    myGpioPin(gpioPin)
{
}

/*------------------------------------------------------------------------------
 * Private methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Adc::Error AdcStm32f4xx::driverEnable(const bool enable)
{
    myGpioPin.enable(enable);
    
    if (enable)
    {
        GpioPin::Config gpioConfig;
        gpioConfig.mode     = GpioPin::MODE_ANALOG;
        gpioConfig.resistor = GpioPin::RESISTOR_NONE;

        myGpioPin.configure(gpioConfig);
    }
    
    RCC_APB2PeriphClockCmd(clockMap[myId], (FunctionalState) enable);
    ADC_Cmd(myAdc, (FunctionalState) enable);
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
Adc::Error AdcStm32f4xx::driverConfigure(const Config& config)
{
    // ADC common init
    ADC_CommonInitTypeDef commonAdcInit;
    commonAdcInit.ADC_Mode              = ADC_Mode_Independent;
    commonAdcInit.ADC_Prescaler         = ADC_Prescaler_Div2;
    commonAdcInit.ADC_DMAAccessMode     = ADC_DMAAccessMode_Disabled;
    commonAdcInit.ADC_TwoSamplingDelay  = ADC_TwoSamplingDelay_5Cycles;

    ADC_CommonInit(&commonAdcInit);

    // ADC init
    ADC_InitTypeDef adcInit;
    adcInit.ADC_Resolution              = ADC_Resolution_12b,
    adcInit.ADC_ScanConvMode            = DISABLE,
    adcInit.ADC_ContinuousConvMode      = (FunctionalState)
                                                          config.conversionMode;
    adcInit.ADC_ExternalTrigConvEdge    = ADC_ExternalTrigConvEdge_None;
    adcInit.ADC_ExternalTrigConv        = ADC_ExternalTrigConv_T1_CC1;
    adcInit.ADC_DataAlign               = ADC_DataAlign_Right;
    adcInit.ADC_NbrOfConversion         = 1;

    ADC_Init(myAdc, &adcInit);

    ADC_RegularChannelConfig(myAdc,
                             idToChannelMap[myChannelId],
                             1,
                             ADC_SampleTime_3Cycles);
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
Adc::Error AdcStm32f4xx::driverReadVoltage(float& voltage)
{
    static const unsigned int valueMax = 4095;
    static const float levelsToVolts = Processor::getCoreVoltage() /
                                       valueMax;
    
    uint16_t raw = 0;

    ADC_SoftwareStartConv(myAdc);

    while (ADC_GetFlagStatus(myAdc, ADC_FLAG_EOC) == RESET);

    raw = ADC_GetConversionValue(myAdc);
    voltage = raw * levelsToVolts;
    
    return ERROR_NONE;
}
