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
/// @file GpioPinSTM32F4xx.cpp
/// @author Ben Minerd
/// @date 3/22/2013
/// @brief GpioPinSTM32F4xx class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <GpioPinSTM32F4xx.h>

using Plat4m::GpioPinSTM32F4xx;
using Plat4m::GpioPin;
using Plat4m::GpioPortSTM32F4xx;
using Plat4m::ProcessorSTM32F4xx;
using Plat4m::Module;

//------------------------------------------------------------------------------
// Private static members
//------------------------------------------------------------------------------

const GPIOMode_TypeDef GpioPinSTM32F4xx::myModeMap[] =
{
    GPIO_Mode_OUT,  /// GpioPin::MODE_DIGITAL_OUTPUT_PUSH_PULL
    GPIO_Mode_OUT,  /// GpioPin::MODE_DIGITAL_OUTPUT_OPEN_DRAIN
    GPIO_Mode_IN,   /// GpioPin::MODE_DIGITAL_INPUT
    GPIO_Mode_AN,   /// GpioPin::MODE_ANALOG_INPUT
    GPIO_Mode_AF    /// GpioPin::MODE_ALTERNATE_FUNCTION
};

const GPIOPuPd_TypeDef GpioPinSTM32F4xx::myResistorMap[] =
{
    GPIO_PuPd_NOPULL,   /// GpioPin::RESISTOR_NONE
    GPIO_PuPd_UP,       /// GpioPin::RESISTOR_PULL_UP
    GPIO_PuPd_DOWN      /// GpioPin::RESISTOR_PULL_DOWN
};

const GpioPinSTM32F4xx::OutputSpeed GpioPinSTM32F4xx::myDefaultOutputSpeed =
															 OUTPUT_SPEED_50MHZ;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GpioPinSTM32F4xx::GpioPinSTM32F4xx(GpioPortSTM32F4xx& gpioPort, const Id id) :
    GpioPin(),
    myGpioPort(gpioPort),
    myId(id),
    myPinBitMask(1 << myId),
	mySTM32F4xxConfig()
{
}

//------------------------------------------------------------------------------
// Public methods implemented from GpioPin
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void GpioPinSTM32F4xx::setLevelFast(const Level level)
{
    if (level == LEVEL_LOW)
    {
        myGpioPort.getPort()->BSRRH = myPinBitMask;
    }
    else // (level == LEVEL_HIGH)
    {
        myGpioPort.getPort()->BSRRL = myPinBitMask;
    }
}

//------------------------------------------------------------------------------
GpioPin::Level GpioPinSTM32F4xx::getLevelFast()
{
    return ((Level) areBitsSet(myGpioPort.getPort()->ODR, myPinBitMask));
}

//------------------------------------------------------------------------------
GpioPin::Level GpioPinSTM32F4xx::readLevelFast()
{
    return ((Level) areBitsSet(myGpioPort.getPort()->IDR, myPinBitMask));
}

//------------------------------------------------------------------------------
void GpioPinSTM32F4xx::toggleLevelFast()
{
    toggleBits(myGpioPort.getPort()->ODR, myPinBitMask);
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GpioPortSTM32F4xx& GpioPinSTM32F4xx::getGpioPort()
{
    return myGpioPort;
}

//------------------------------------------------------------------------------
GpioPinSTM32F4xx::Id GpioPinSTM32F4xx::getId() const
{
    return myId;
}

//------------------------------------------------------------------------------
void GpioPinSTM32F4xx::setSTM32F4xxConfig(STM32F4xxConfig& config)
{
    switch (config.alternateFunction)
    {
        // I2C
        case GpioPinSTM32F4xx::ALTERNATE_FUNCTION_4:
        {
            setOutputType(GpioPinSTM32F4xx::OUTPUT_TYPE_OPEN_DRAIN);
            
            break;
        }
        default:
        {
            setOutputType(GpioPinSTM32F4xx::OUTPUT_TYPE_PUSH_PULL);
            
            break;
        }
    }
    
    setOutputSpeed(config.outputSpeed);
    GPIO_PinAFConfig(myGpioPort.getPort(), myId, config.alternateFunction);
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error GpioPinSTM32F4xx::driverSetEnabled(const bool enabled)
{
    myGpioPort.setEnabled(enabled);
    
    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods implemented from GpioPin
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GpioPin::Error GpioPinSTM32F4xx::driverConfigure(const Config& config)
{
    // Set mode
    clearAndSetBits(myGpioPort.getPort()->MODER,
    				(GPIO_MODER_MODER0 << (myId * 2)),
					(((uint32_t) myModeMap[config.mode]) << (myId * 2)));
    
    // Set resistor
    clearAndSetBits(
    			   myGpioPort.getPort()->PUPDR,
    			   (GPIO_PUPDR_PUPDR0 << (myId * 2)),
				   (((uint32_t) myResistorMap[config.resistor]) << (myId * 2)));
    
    if (config.mode == MODE_DIGITAL_OUTPUT_PUSH_PULL)
    {
        setOutputSpeed(myDefaultOutputSpeed);
        setOutputType(GpioPinSTM32F4xx::OUTPUT_TYPE_PUSH_PULL);
    }
    
    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
GpioPin::Error GpioPinSTM32F4xx::driverSetLevel(const Level level)
{
    if (level == LEVEL_LOW)
    {
        myGpioPort.getPort()->BSRRH = myPinBitMask;
    }
    else
    {
        myGpioPort.getPort()->BSRRL = myPinBitMask;
    }
    
    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
GpioPin::Error GpioPinSTM32F4xx::driverGetLevel(Level& level)
{
    level = (Level) areBitsSet(myGpioPort.getPort()->ODR, myPinBitMask);
    
    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
GpioPin::Error GpioPinSTM32F4xx::driverReadLevel(Level& level)
{
    level = (Level) areBitsSet(myGpioPort.getPort()->IDR, myPinBitMask);
    
    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
GpioPin::Error GpioPinSTM32F4xx::driverToggleLevel()
{
    toggleBits(myGpioPort.getPort()->ODR, myPinBitMask);
    
    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void GpioPinSTM32F4xx::setOutputType(const OutputType outputType)
{
    // Set output bits for this pin
    clearAndSetBits(myGpioPort.getPort()->OTYPER,
                    GPIO_OTYPER_OT_0 << myId,
                    ((uint32_t) outputType) << myId);
}

//------------------------------------------------------------------------------
void GpioPinSTM32F4xx::setOutputSpeed(const OutputSpeed outputSpeed)
{
    // Set speed bits for this pin
    clearAndSetBits(myGpioPort.getPort()->OSPEEDR,
                    GPIO_OSPEEDER_OSPEEDR0 << (myId * 2),
                    ((uint32_t) outputSpeed) << (myId * 2));
}
