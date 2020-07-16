//------------------------------------------------------------------------------
//       _______    __                           ___
//      ||  ___ \  || |             __          //  |
//      || |  || | || |   _______  || |__      //   |    _____  ___
//      || |__|| | || |  // ___  | ||  __|    // _  |   ||  _ \/ _ \
//      ||  ____/  || | || |  || | || |      // /|| |   || |\\  /\\ \
//      || |       || | || |__|| | || |     // /_|| |_  || | || | || |
//      || |       || |  \\____  | || |__  //_____   _| || | || | || |
//      ||_|       ||_|       ||_|  \\___|       ||_|   ||_| ||_| ||_|
//
//
// The MIT License (MIT)
//
// Copyright (c) 2017 Benjamin Minerd
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------------

///
/// @file GpioPinSTM32F4xx.h
/// @author Ben Minerd
/// @date 3/22/2013
/// @brief GpioPinSTM32F4xx class header file.
///

#ifndef PLAT4M_GPIO_PIN_STM32F4XX_H
#define PLAT4M_GPIO_PIN_STM32F4XX_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>

#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/GpioPin.h>
#include <Plat4m_Core/STM32F4xx/GpioPortSTM32F4xx.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class GpioPinSTM32F4xx : public GpioPin
{
public:

    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------

    enum Id
    {
        ID_0 = 0,
        ID_1,
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
        ID_14,
        ID_15
    };

    enum AlternateFunction
    {
        ALTERNATE_FUNCTION_0            = 0,
        ALTERNATE_FUNCTION_RTC_50Hz     = ALTERNATE_FUNCTION_0,
        ALTERNATE_FUNCTION_MCO          = ALTERNATE_FUNCTION_0,
        ALTERNATE_FUNCTION_TAMPER       = ALTERNATE_FUNCTION_0,
        ALTERNATE_FUNCTION_SWJ          = ALTERNATE_FUNCTION_0,
        ALTERNATE_FUNCTION_TRACE        = ALTERNATE_FUNCTION_0,

        ALTERNATE_FUNCTION_1            = 1,
        ALTERNATE_FUNCTION_TIM1         = ALTERNATE_FUNCTION_1,
        ALTERNATE_FUNCTION_TIM2         = ALTERNATE_FUNCTION_1,

        ALTERNATE_FUNCTION_2            = 2,
        ALTERNATE_FUNCTION_TIM3         = ALTERNATE_FUNCTION_2,
        ALTERNATE_FUNCTION_TIM4         = ALTERNATE_FUNCTION_2,
        ALTERNATE_FUNCTION_TIM5         = ALTERNATE_FUNCTION_2,

        ALTERNATE_FUNCTION_3            = 3,
        ALTERNATE_FUNCTION_TIM8         = ALTERNATE_FUNCTION_3,
        ALTERNATE_FUNCTION_TIM9         = ALTERNATE_FUNCTION_3,
        ALTERNATE_FUNCTION_TIM10        = ALTERNATE_FUNCTION_3,
        ALTERNATE_FUNCTION_TIM11        = ALTERNATE_FUNCTION_3,

        ALTERNATE_FUNCTION_4            = 4,
        ALTERNATE_FUNCTION_I2C1         = ALTERNATE_FUNCTION_4,
        ALTERNATE_FUNCTION_I2C2         = ALTERNATE_FUNCTION_4,
        ALTERNATE_FUNCTION_I2C3         = ALTERNATE_FUNCTION_4,

        ALTERNATE_FUNCTION_5            = 5,
        ALTERNATE_FUNCTION_SPI1         = ALTERNATE_FUNCTION_5,
        ALTERNATE_FUNCTION_SPI2         = ALTERNATE_FUNCTION_5,

        ALTERNATE_FUNCTION_6            = 6,
        ALTERNATE_FUNCTION_SPI3         = ALTERNATE_FUNCTION_6,

        ALTERNATE_FUNCTION_7            = 7,
        ALTERNATE_FUNCTION_USART1       = ALTERNATE_FUNCTION_7,
        ALTERNATE_FUNCTION_USART2       = ALTERNATE_FUNCTION_7,
        ALTERNATE_FUNCTION_USART3       = ALTERNATE_FUNCTION_7,

        ALTERNATE_FUNCTION_8            = 8,
        ALTERNATE_FUNCTION_UART4        = ALTERNATE_FUNCTION_8,
        ALTERNATE_FUNCTION_UART5        = ALTERNATE_FUNCTION_8,
        ALTERNATE_FUNCTION_USART6       = ALTERNATE_FUNCTION_8,

        ALTERNATE_FUNCTION_9            = 9,
        ALTERNATE_FUNCTION_CAN1         = ALTERNATE_FUNCTION_9,
        ALTERNATE_FUNCTION_CAN2         = ALTERNATE_FUNCTION_9,
        ALTERNATE_FUNCTION_TIM12        = ALTERNATE_FUNCTION_9,
        ALTERNATE_FUNCTION_TIM13        = ALTERNATE_FUNCTION_9,
        ALTERNATE_FUNCTION_TIM14        = ALTERNATE_FUNCTION_9,

        ALTERNATE_FUNCTION_10           = 10,
        ALTERNATE_FUNCTION_OTG_FS       = ALTERNATE_FUNCTION_10,
        ALTERNATE_FUNCTION_OTG_HS       = ALTERNATE_FUNCTION_10,

        ALTERNATE_FUNCTION_11           = 11,
        ALTERNATE_FUNCTION_ETH          = ALTERNATE_FUNCTION_11,

        ALTERNATE_FUNCTION_12           = 12,
        ALTERNATE_FUNCTION_FSMC         = ALTERNATE_FUNCTION_12,
        ALTERNATE_FUNCTION_OTG_HS_FS    = ALTERNATE_FUNCTION_12,
        ALTERNATE_FUNCTION_SDIO         = ALTERNATE_FUNCTION_12,

        ALTERNATE_FUNCTION_13           = 13,
        ALTERNATE_FUNCTION_DCMI         = ALTERNATE_FUNCTION_13,

        ALTERNATE_FUNCTION_15           = 15,
        ALTERNATE_FUNCTION_EVENTOUT     = ALTERNATE_FUNCTION_15
    };

    enum OutputType
    {
        OUTPUT_TYPE_PUSH_PULL   = GPIO_OType_PP,
        OUTPUT_TYPE_OPEN_DRAIN  = GPIO_OType_OD
    };

    enum OutputSpeed
    {
        OUTPUT_SPEED_2MHZ   = GPIO_Speed_2MHz,
        OUTPUT_SPEED_25MHZ  = GPIO_Speed_25MHz,
        OUTPUT_SPEED_50MHZ  = GPIO_Speed_50MHz,
        OUTPUT_SPEED_100MHZ = GPIO_Speed_100MHz
    };

    struct STM32F4xxConfig
    {
        AlternateFunction alternateFunction;
        OutputType outputType;
        OutputSpeed outputSpeed;
    };

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------
    
    GpioPinSTM32F4xx(GpioPortSTM32F4xx& gpioPort, const Id id);

    //--------------------------------------------------------------------------
    // Public virtual methods implemented from GpioPin
    //--------------------------------------------------------------------------

    void setLevelFast(const Level level);

    Level getLevelFast();

    Level readLevelFast();

    void toggleLevelFast();
    
    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------
    
    GpioPortSTM32F4xx& getGpioPort();
    
    Id getId() const;
    
    void setSTM32F4xxConfig(STM32F4xxConfig& config);
    
private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    static const GPIOMode_TypeDef myModeMap[];

    static const GPIOPuPd_TypeDef myResistorMap[];
    
    static const OutputSpeed myDefaultOutputSpeed;

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------
    
    GpioPortSTM32F4xx& myGpioPort;
    
    const Id myId;
    
    const uint16_t myPinBitMask;
    
    STM32F4xxConfig mySTM32F4xxConfig;

    //--------------------------------------------------------------------------
    // Private methods implemented from Module
    //--------------------------------------------------------------------------
    
    Module::Error driverSetEnabled(const bool enabled);

    //--------------------------------------------------------------------------
    // Private methods implemented from GpioPin
    //--------------------------------------------------------------------------
    
    GpioPin::Error driverConfigure(const Config& config);
    
    GpioPin::Error driverSetLevel(const Level level);
    
    GpioPin::Error driverGetLevel(Level& level);
    
    GpioPin::Error driverReadLevel(Level& level);
    
    GpioPin::Error driverToggleLevel();
    
    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------
    
    void setOutputType(const OutputType outputType);
    
    void setOutputSpeed(const OutputSpeed outputSpeed);
};

}; // namespace Plat4m

#endif // PLAT4M_GPIO_PIN_STM32F4XX_H
