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
/// @file GpioPinSTM32F30x.h
/// @author Ben Minerd
/// @date 2/4/2016
/// @brief GpioPinSTM32F30x class header file.
///

#ifndef PLAT4M_GPIO_PIN_STM32F30X_H
#define PLAT4M_GPIO_PIN_STM32F30X_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m.h>
#include <GpioPin.h>
#include <GpioPortSTM32F30x.h>

#include <stm32f30x.h>
#include <stm32f30x_gpio.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class GpioPinSTM32F30x : public GpioPin
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
        ALTERNATE_FUNCTION_0 = 0,
        ALTERNATE_FUNCTION_1,
        ALTERNATE_FUNCTION_2,
        ALTERNATE_FUNCTION_3,
        ALTERNATE_FUNCTION_4,
        ALTERNATE_FUNCTION_5,
        ALTERNATE_FUNCTION_6,
        ALTERNATE_FUNCTION_7,
        ALTERNATE_FUNCTION_8,
        ALTERNATE_FUNCTION_9,
        ALTERNATE_FUNCTION_10,
        ALTERNATE_FUNCTION_11,
        ALTERNATE_FUNCTION_12,
        ALTERNATE_FUNCTION_13,
        ALTERNATE_FUNCTION_15,
    };

    enum OutputType
    {
        OUTPUT_TYPE_PUSH_PULL   = GPIO_OType_PP,
        OUTPUT_TYPE_OPEN_DRAIN  = GPIO_OType_OD
    };

    enum OutputSpeed
    {
        OUTPUT_SPEED_2MHZ   = GPIO_Speed_2MHz,
        OUTPUT_SPEED_10MHZ  = GPIO_Speed_10MHz,
        OUTPUT_SPEED_50MHZ  = GPIO_Speed_50MHz
    };

    struct STM32F30xConfig
    {
        AlternateFunction alternateFunction;
        OutputType outputType;
        OutputSpeed outputSpeed;
    };

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    GpioPinSTM32F30x(GpioPortSTM32F30x& gpioPort, const Id id);

    //--------------------------------------------------------------------------
    // Public methods implemented from GpioPin
    //--------------------------------------------------------------------------

    void setLevelFast(const Level level);

    Level getLevelFast();

    Level readLevelFast();

    void toggleLevelFast();
    
    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------
    
    GpioPortSTM32F30x& getGpioPort();
    
    Id getId() const;
    
    void setSTM32F30xConfig(STM32F30xConfig& config);
    
    void setOutputType(const OutputType outputType);

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
    
    GpioPortSTM32F30x& myGpioPort;
    
    const Id myId;
    
    const uint16_t myPinBitMask;

    STM32F30xConfig mySTM32F30xConfig;

    //--------------------------------------------------------------------------
    // Private methods implemented from Module
    //--------------------------------------------------------------------------
    
    Module::Error driverSetEnabled(const bool enable);

    //--------------------------------------------------------------------------
    // Private methods implemented from GpioPin
    //--------------------------------------------------------------------------
    
    GpioPin::Error driverConfigure(const Config& config);
    
    GpioPin::Error driverSetLevel(const Level level);
    
    GpioPin::Error driverGetLevel(Level& level);
    
    GpioPin::Error driverReadLevel(Level& level);
    
    GpioPin::Error driverToggleLevel();
    
    //--------------------------------------------------------------------------
    // Private inline methods
    //--------------------------------------------------------------------------
    
//    inline void setOutputType(const OutputType outputType);

    inline void setOutputSpeed(const OutputSpeed outputSpeed);
    
    inline void setAlternateFunction(const AlternateFunction alternateFunction);
};

}; // namespace Plat4m

#endif // PLAT4M_GPIO_PIN_STM32F30X_H
