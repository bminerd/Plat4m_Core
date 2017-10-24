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
// Copyright (c) 2016 Benjamin Minerd
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
/// @date 3/22/13
/// @brief GpioPinSTM32F4xx class header file.
///

#ifndef GPIO_PIN_STM32F4XX_H
#define GPIO_PIN_STM32F4XX_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m.h>
#include <GpioPin.h>
#include <GpioSTM32F4xx.h>

#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>

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
    // Public constructors
    //--------------------------------------------------------------------------
    
    GpioPinSTM32F4xx(const GpioSTM32F4xx::PortId portId,
                     const GpioSTM32F4xx::PinId pinId);

    GpioPinSTM32F4xx(const GpioPin::Id id);

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
    
    GpioSTM32F4xx::PortId getPortId() const;
    
    GpioSTM32F4xx::PinId getPinId() const;
    
    void configureDriver(GpioSTM32F4xx::Config& config);
    
private:
    
    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------
    
    const GpioSTM32F4xx::PortId myPortId;
    
    const GpioSTM32F4xx::PinId myPinId;
    
    const uint16_t myPinBitMask;
    
    GPIO_TypeDef* myPort;
    
    //--------------------------------------------------------------------------
    // Private methods implemented from Module
    //--------------------------------------------------------------------------
    
    Module::Error driverEnable(const bool enable);

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
    
    void setOutputType(const GpioSTM32F4xx::OutputType outputType);
    
    void setOutputSpeed(const GpioSTM32F4xx::OutputSpeed outputSpeed);
};

}; // namespace Plat4m

#endif // GPIO_PIN_STM32F4XX_H
