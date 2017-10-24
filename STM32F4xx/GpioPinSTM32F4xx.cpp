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
/// @file GpioPinSTM32F4xx.cpp
/// @author Ben Minerd
/// @date 3/22/13
/// @brief GpioPinSTM32F4xx class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <GpioPinSTM32F4xx.h>

using Plat4m::GpioPinSTM32F4xx;
using Plat4m::GpioSTM32F4xx;
using Plat4m::GpioPin;
using Plat4m::Module;

//------------------------------------------------------------------------------
// Local variables
//------------------------------------------------------------------------------

static const GpioSTM32F4xx::OutputSpeed defaultOutputSpeed =
                                              GpioSTM32F4xx::OUTPUT_SPEED_50MHZ;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GpioPinSTM32F4xx::GpioPinSTM32F4xx(const GpioSTM32F4xx::PortId portId,
                                   const GpioSTM32F4xx::PinId pinId) :
    GpioPin(),
    myPortId(portId),
    myPinId(pinId),
    myPinBitMask(1 << pinId),
    myPort(GpioSTM32F4xx::portMap[portId])
{
}

//------------------------------------------------------------------------------
GpioPinSTM32F4xx::GpioPinSTM32F4xx(const GpioPin::Id id) :
    GpioPin(),
    myPortId((GpioSTM32F4xx::PortId) id.portId),
    myPinId((GpioSTM32F4xx::PinId) id.pinId),
    myPinBitMask(1 << (id.pinId)),
    myPort(GpioSTM32F4xx::portMap[(id.portId)])
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
        myPort->BSRRH = myPinBitMask;
    }
    else // (level == LEVEL_HIGH)
    {
        myPort->BSRRL = myPinBitMask;
    }
}

//------------------------------------------------------------------------------
GpioPin::Level GpioPinSTM32F4xx::getLevelFast()
{
    return ((Level) areBitsSet(myPort->ODR, myPinBitMask));
}

//------------------------------------------------------------------------------
GpioPin::Level GpioPinSTM32F4xx::readLevelFast()
{
    return ((Level) areBitsSet(myPort->IDR, myPinBitMask));
}

//------------------------------------------------------------------------------
void GpioPinSTM32F4xx::toggleLevelFast()
{
    toggleBits(myPort->ODR, myPinBitMask);
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GpioSTM32F4xx::PortId GpioPinSTM32F4xx::getPortId() const
{
    return myPortId;
}

//------------------------------------------------------------------------------
GpioSTM32F4xx::PinId GpioPinSTM32F4xx::getPinId() const
{
    return myPinId;
}

//------------------------------------------------------------------------------
void GpioPinSTM32F4xx::configureDriver(GpioSTM32F4xx::Config& config)
{
    switch (config.alternateFunction)
    {
        // I2C
        case GpioSTM32F4xx::ALTERNATE_FUNCTION_4:
        {
            setOutputType(GpioSTM32F4xx::OUTPUT_TYPE_OPEN_DRAIN);
            
            break;
        }
        default:
        {
            setOutputType(GpioSTM32F4xx::OUTPUT_TYPE_PUSH_PULL);
            
            break;
        }
    }
    
    setOutputSpeed(config.outputSpeed);
    GPIO_PinAFConfig(myPort, myPinId, config.alternateFunction);
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error GpioPinSTM32F4xx::driverEnable(const bool enable)
{
    RCC_AHB1PeriphClockCmd(GpioSTM32F4xx::clockMap[myPortId],
    					   (FunctionalState) enable);
    
    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods implemented from GpioPin
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GpioPin::Error GpioPinSTM32F4xx::driverConfigure(const Config& config)
{
    // Set mode
    
    // Clear mode bits for this pin
    myPort->MODER &= ~(GPIO_MODER_MODER0 << (myPinId * 2));
    // Set mode bits for this pin
    myPort->MODER |= (((uint32_t) GpioSTM32F4xx::modeMap[config.mode]) <<
                      (myPinId * 2));
    
    // Set resistor
    
    // Clear resistor bits for this pin
    myPort->PUPDR &= ~(GPIO_PUPDR_PUPDR0 << (myPinId * 2));
    // Set resistor bits for this pin
    myPort->PUPDR |= (((uint32_t)
                       GpioSTM32F4xx::resistorMap[config.resistor]) <<
                      (myPinId * 2));
    
    if (config.mode == MODE_DIGITAL_OUTPUT_PUSH_PULL)
    {
        setOutputSpeed(defaultOutputSpeed);
        setOutputType(GpioSTM32F4xx::OUTPUT_TYPE_PUSH_PULL);
    }
    
    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
GpioPin::Error GpioPinSTM32F4xx::driverSetLevel(const Level level)
{
    if (level == LEVEL_LOW)
    {
        myPort->BSRRH = myPinBitMask;
    }
    else
    {
        myPort->BSRRL = myPinBitMask;
    }
    
    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
GpioPin::Error GpioPinSTM32F4xx::driverGetLevel(Level& level)
{
    level = (Level) areBitsSet(myPort->ODR, myPinBitMask);
    
    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
GpioPin::Error GpioPinSTM32F4xx::driverReadLevel(Level& level)
{
    level = (Level) areBitsSet(myPort->IDR, myPinBitMask);
    
    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
GpioPin::Error GpioPinSTM32F4xx::driverToggleLevel()
{
    toggleBits(myPort->ODR, myPinBitMask);
    
    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void GpioPinSTM32F4xx::setOutputType(const GpioSTM32F4xx::OutputType outputType)
{
    // Clear output bits for this pin
    clearBits(myPort->OTYPER, GPIO_OTYPER_OT_0 << myPinId);
    
    // Set output bits for this pin
    setBits(myPort->OTYPER, ((uint32_t) outputType) << myPinId);
}

//------------------------------------------------------------------------------
void GpioPinSTM32F4xx::setOutputSpeed(
                                   const GpioSTM32F4xx::OutputSpeed outputSpeed)
{
    // Clear speed bits for this pin
    clearBits(myPort->OSPEEDR, GPIO_OSPEEDER_OSPEEDR0 << (myPinId * 2));

    // Set speed bits for this pin
    setBits(myPort->OSPEEDR, ((uint32_t) outputSpeed) << (myPinId * 2));
}
