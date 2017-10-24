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
 * @file DacStm32f4xx.cpp
 * @author Ben Minerd
 * @date 4/11/2013
 * @brief DacStm32f4xx class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <DacStm32f4xx.h>
#include <Processor.h>

using Plat4m::DacStm32f4xx;

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 * @brief DAC channel map.
 */
static const uint32_t channelMap[] =
{
    DAC_Channel_1,  /// DAC_CHANNEL_1
    DAC_Channel_2   /// DAC_CHANNEL_2
};

/**
 * @brief DAC clock map.
 */
static const uint32_t clockMap[] =
{
    RCC_APB1Periph_DAC  /// DAC_1
};

static const uint32_t maxValue = 255;
static DacStm32f4xx* objectMap[2];

/*------------------------------------------------------------------------------
 * Public static methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
DacStm32f4xx& DacStm32f4xx::get(const ChannelId channelId,
                                GpioPinStm32f4xx& gpioPin)
{
    if (IS_NULL_POINTER(objectMap[channelId]))
    {
        objectMap[channelId] = new DacStm32f4xx(channelId, gpioPin);
    }
    else
    {
        // Error?
    }
    
    return *(objectMap[channelId]);
}

/*------------------------------------------------------------------------------
 * Private constructors and destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
DacStm32f4xx::DacStm32f4xx(const ChannelId channelId,
                           GpioPinStm32f4xx& gpioPin) :
    Dac(Processor::getCoreVoltage(), maxValue),
    myChannelId(channelId),
    myGpioPin(gpioPin)
{
}

/*------------------------------------------------------------------------------
 * Private methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Dac::Error DacStm32f4xx::driverEnable(const bool enable)
{
    myGpioPin.enable(enable);
    
    if (enable)
    {
        GpioPin::Config gpioConfig;
        gpioConfig.mode     = GpioPin::MODE_ANALOG;
        gpioConfig.resistor = GpioPin::RESISTOR_NONE;
        
        myGpioPin.configure(gpioConfig);
    }
    
    RCC_APB1PeriphClockCmd(clockMap[0], (FunctionalState) enable);
    DAC_Cmd(channelMap[myChannelId], (FunctionalState) enable);
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
Dac::Error DacStm32f4xx::driverConfigure(const Config& config)
{
    DAC_InitTypeDef dacInit;
    dacInit.DAC_Trigger         = DAC_Trigger_None;
    dacInit.DAC_WaveGeneration  = DAC_WaveGeneration_None;
//    dacInit.DAC_LFSRUnmask_TriangleAmplitude   =
    dacInit.DAC_OutputBuffer    = DAC_OutputBuffer_Enable;
    
    DAC_Init(channelMap[myChannelId], &dacInit);
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
Dac::Error DacStm32f4xx::driverSetVoltage(const float voltage)
{    
    uint16_t dacValue = computeDacValue(voltage);
    
    switch (myChannelId)
    {
        case CHANNEL_ID_1:
        {
            DAC_SetChannel1Data(DAC_Align_8b_R, dacValue);
            
            break;
        }
        case CHANNEL_ID_2:
        {
            DAC_SetChannel2Data(DAC_Align_8b_R, dacValue);
            
            break;
        }
    }
    
    return ERROR_NONE;
}
