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
/// @file GpioPinSTM32F30x.cpp
/// @author Ben Minerd
/// @date 2/4/2016
/// @brief GpioPinSTM32F30x class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/STM32F30x/GpioPinSTM32F30x.h>

using Plat4m::GpioPinSTM32F30x;
using Plat4m::GpioPortSTM32F30x;
using Plat4m::GpioPin;
using Plat4m::Module;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const GPIOMode_TypeDef GpioPinSTM32F30x::myModeMap[] =
{
    GPIO_Mode_OUT, /// GpioPin::MODE_DIGITAL_OUTPUT_PUSH_PULL
    GPIO_Mode_OUT, /// GpioPin::MODE_DIGITAL_OUTPUT_OPEN_DRAIN
    GPIO_Mode_IN,  /// GpioPin::MODE_DIGITAL_INPUT
    GPIO_Mode_AN,  /// GpioPin::MODE_ANALOG
    GPIO_Mode_AF   /// GpioPin::MODE_ALTERNATE_FUNCTION
};

const GPIOPuPd_TypeDef GpioPinSTM32F30x::myResistorMap[] =
{
    GPIO_PuPd_NOPULL, /// GpioPin::RESISTOR_NONE
    GPIO_PuPd_UP,     /// GpioPin::RESISTOR_PULL_UP
    GPIO_PuPd_DOWN    /// GpioPin::RESISTOR_PULL_DOWN
};

const GpioPinSTM32F30x::OutputSpeed GpioPinSTM32F30x::myDefaultOutputSpeed =
                                           GpioPinSTM32F30x::OUTPUT_SPEED_50MHZ;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GpioPinSTM32F30x::GpioPinSTM32F30x(GpioPortSTM32F30x& gpioPort,
                                   const Id id) :
    GpioPin(),
    myGpioPort(gpioPort),
    myId(id),
    myPinBitMask(1 << myId),
    mySTM32F30xConfig()
{
}

//------------------------------------------------------------------------------
// Public methods implemented from GpioPin
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void GpioPinSTM32F30x::setLevelFast(const Level level)
{
    if (level == LEVEL_LOW)
    {
        myGpioPort.getPort()->BRR = myPinBitMask;
    }
    else
    {
        myGpioPort.getPort()->BSRR = myPinBitMask;
    }
}

//------------------------------------------------------------------------------
GpioPin::Level GpioPinSTM32F30x::getLevelFast()
{
    return ((Level) areBitsSet(myGpioPort.getPort()->ODR, myPinBitMask));
}

//------------------------------------------------------------------------------
GpioPin::Level GpioPinSTM32F30x::readLevelFast()
{
    return ((Level) areBitsSet(myGpioPort.getPort()->IDR, myPinBitMask));
}

//------------------------------------------------------------------------------
void GpioPinSTM32F30x::toggleLevelFast()
{
    toggleBits(myGpioPort.getPort()->ODR, myPinBitMask);
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GpioPortSTM32F30x& GpioPinSTM32F30x::getGpioPort()
{
    return myGpioPort;
}

//------------------------------------------------------------------------------
GpioPinSTM32F30x::Id GpioPinSTM32F30x::getId() const
{
    return myId;
}

//------------------------------------------------------------------------------
void GpioPinSTM32F30x::setSTM32F30xConfig(STM32F30xConfig& config)
{
    setAlternateFunction(config.alternateFunction);
    setOutputType(config.outputType);
    setOutputSpeed(config.outputSpeed);

    mySTM32F30xConfig = config;
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error GpioPinSTM32F30x::driverSetEnabled(const bool enabled)
{
    myGpioPort.setEnabled(enabled);
    
    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods implemented from GpioPin
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GpioPin::Error GpioPinSTM32F30x::driverConfigure(const Config& config)
{
    // Set mode
    
    // Clear mode bits for this pin
    myGpioPort.getPort()->MODER &= ~(GPIO_MODER_MODER0 << (myId * 2));
    // Set mode bits for this pin
    myGpioPort.getPort()->MODER |= (((uint32_t) myModeMap[config.mode]) <<
                      (myId * 2));
    
    // Set resistor
    
    // Clear resistor bits for this pin
    myGpioPort.getPort()->PUPDR &= ~(GPIO_PUPDR_PUPDR0 << (myId * 2));
    // Set resistor bits for this pin
    myGpioPort.getPort()->PUPDR |= (((uint32_t)
                       myResistorMap[config.resistor]) <<
                      (myId * 2));
    
    if (config.mode == MODE_DIGITAL_OUTPUT_PUSH_PULL)
    {
        setOutputSpeed(myDefaultOutputSpeed);
        setOutputType(OUTPUT_TYPE_PUSH_PULL);
    }
    
    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
GpioPin::Error GpioPinSTM32F30x::driverSetLevel(const Level level)
{
    if (level == LEVEL_LOW)
    {
        myGpioPort.getPort()->BRR = myPinBitMask;
    }
    else
    {
        myGpioPort.getPort()->BSRR = myPinBitMask;
    }
    
    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
GpioPin::Error GpioPinSTM32F30x::driverGetLevel(Level& level)
{
    level = (Level) areBitsSet(myGpioPort.getPort()->ODR, myPinBitMask);
    
    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
GpioPin::Error GpioPinSTM32F30x::driverReadLevel(Level& level)
{
    level = (Level) areBitsSet(myGpioPort.getPort()->IDR, myPinBitMask);
    
    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
GpioPin::Error GpioPinSTM32F30x::driverToggleLevel()
{
    toggleBits(myGpioPort.getPort()->ODR, myPinBitMask);
    
    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private inline methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void GpioPinSTM32F30x::setOutputType(const OutputType outputType)
{
    // Set output bits for this pin
    clearAndSetBits(myGpioPort.getPort()->OTYPER,
                    GPIO_OTYPER_OT_0 << myId,
                    ((uint32_t) outputType) << myId);
}

//------------------------------------------------------------------------------
void GpioPinSTM32F30x::setOutputSpeed(const OutputSpeed outputSpeed)
{
    // Set speed bits for this pin
    clearAndSetBits(myGpioPort.getPort()->OSPEEDR,
                    GPIO_OSPEEDER_OSPEEDR0 << (myId * 2),
                    ((uint32_t) outputSpeed) << (myId * 2));
}

//------------------------------------------------------------------------------
void GpioPinSTM32F30x::setAlternateFunction(
                                      const AlternateFunction alternateFunction)
{
    if (myId < ID_8)
    {
        clearAndSetBits(myGpioPort.getPort()->AFR[0],
                        GPIO_AFRL_AFRL0 << (myId * 4),
                        ((uint32_t) alternateFunction) << (myId * 4));
    }
    else
    {
        clearAndSetBits(myGpioPort.getPort()->AFR[1],
                        GPIO_AFRH_AFRH0 << ((myId - 8) * 4),
                        ((uint32_t) alternateFunction) << ((myId - 8) * 4));
    }
}
