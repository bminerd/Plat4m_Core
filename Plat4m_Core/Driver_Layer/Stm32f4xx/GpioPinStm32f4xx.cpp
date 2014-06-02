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
 * @file GpioPinStm32f4xx.cpp
 * @author Ben Minerd
 * @date 3/22/13
 * @brief GpioPinStm32f4xx class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <GpioPinStm32f4xx.h>

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

static const GpioStm32f4xx::OutputSpeed defaultOutputSpeed =
                                              GpioStm32f4xx::OUTPUT_SPEED_50MHZ;

/*------------------------------------------------------------------------------
 * Public static methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
GpioPinStm32f4xx& GpioPinStm32f4xx::get(const GpioStm32f4xx::PortId portId,
                                        const GpioStm32f4xx::PinId pinId)
{
    if (IS_VALID_POINTER(GpioStm32f4xx::portObjectMap[portId]))
    {
        // Not allowed to have overlapping port and pin
        // Error?
        // System::lockup()
    }
    
    if (IS_NULL_POINTER(GpioStm32f4xx::pinObjectMap[portId][pinId]))
    {
        GpioStm32f4xx::pinObjectMap[portId][pinId] =
                                            new GpioPinStm32f4xx(portId, pinId);
    }
    
    return *(GpioStm32f4xx::pinObjectMap[portId][pinId]);
}

//------------------------------------------------------------------------------
GpioPinStm32f4xx& GpioPinStm32f4xx::get(const GpioPin::Id& id)
{
    return get((GpioStm32f4xx::PortId) id.portId,
               (GpioStm32f4xx::PinId) id.pinId);
}

/*------------------------------------------------------------------------------
 * Public methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
GpioStm32f4xx::PortId GpioPinStm32f4xx::getPortId() const
{
    return myPortId;
}

//------------------------------------------------------------------------------
GpioStm32f4xx::PinId GpioPinStm32f4xx::getPinId() const
{
    return myPinId;
}

//------------------------------------------------------------------------------
void GpioPinStm32f4xx::configureDriver(GpioStm32f4xx::Config& config)
{
    switch (config.alternateFunction)
    {
        // I2C
        case GpioStm32f4xx::ALTERNATE_FUNCTION_4:
        {
            setOutputType(GpioStm32f4xx::OUTPUT_TYPE_OPEN_DRAIN);
            
            break;
        }
        default:
        {
            setOutputType(GpioStm32f4xx::OUTPUT_TYPE_PUSH_PULL);
            
            break;
        }
    }
    
    setOutputSpeed(config.outputSpeed);
    GPIO_PinAFConfig(myPort, myPinId, config.alternateFunction);
}

/*------------------------------------------------------------------------------
 * Private constructors and destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
GpioPinStm32f4xx::GpioPinStm32f4xx(const GpioStm32f4xx::PortId portId,
                                   const GpioStm32f4xx::PinId pinId) :
    GpioPin(GpioStm32f4xx::portIdStringMap[portId],
            GpioStm32f4xx::pinIdStringMap[pinId]),
    myPortId(portId),
    myPinId(pinId),
    myPin(1 << pinId),
    myPort(GpioStm32f4xx::portMap[portId])
{
}

/*------------------------------------------------------------------------------
 * Private implemented methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
GpioPin::Error GpioPinStm32f4xx::driverEnable(const bool enable)
{
    RCC_AHB1PeriphClockCmd(GpioStm32f4xx::clockMap[myPortId],
                           (FunctionalState) enable);
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
GpioPin::Error GpioPinStm32f4xx::driverConfigure(const Config& config)
{
    // Set mode
    
    // Clear mode bits for this pin
    myPort->MODER &= ~(GPIO_MODER_MODER0 << (myPinId * 2));
    // Set mode bits for this pin
    myPort->MODER |= (((uint32_t) GpioStm32f4xx::modeMap[config.mode]) <<
                      (myPinId * 2));
    
    // Set resistor
    
    // Clear resistor bits for this pin
    myPort->PUPDR &= ~(GPIO_PUPDR_PUPDR0 << (myPinId * 2));
    // Set resistor bits for this pin
    myPort->PUPDR |= (((uint32_t)
                       GpioStm32f4xx::resistorMap[config.resistor]) <<
                      (myPinId * 2));
    
    if (config.mode == MODE_DIGITAL_OUTPUT)
    {
        setOutputSpeed(defaultOutputSpeed);
        setOutputType(GpioStm32f4xx::OUTPUT_TYPE_PUSH_PULL);
    }
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
GpioPin::Error GpioPinStm32f4xx::driverSetLevel(const Level level)
{
    switch (level)
    {
        case LEVEL_LOW:
        {
            myPort->BSRRH = myPin;
            
            break;
        }
        case LEVEL_HIGH:
        {
            myPort->BSRRL = myPin;
            
            break;
        }
    }
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
GpioPin::Error GpioPinStm32f4xx::driverGetLevel(Level& level)
{
    level = (Level) (((myPort->ODR) & myPin) == (myPin));
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
GpioPin::Error GpioPinStm32f4xx::driverReadLevel(Level& level)
{
    level = (Level) (((myPort->IDR) & myPin) == myPin);
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
GpioPin::Error GpioPinStm32f4xx::driverToggleLevel()
{
    myPort->ODR ^= myPin;
    
    return ERROR_NONE;
}

/*------------------------------------------------------------------------------
 * Private methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void GpioPinStm32f4xx::setOutputType(const GpioStm32f4xx::OutputType outputType)
{
    // Clear output bits for this pin
    myPort->OTYPER &= ~(GPIO_OTYPER_OT_0 << myPinId);
    
    // Set output bits for this pin
    myPort->OTYPER |= ((uint32_t) outputType << myPinId);
}

//------------------------------------------------------------------------------
void GpioPinStm32f4xx::setOutputSpeed(
                                   const GpioStm32f4xx::OutputSpeed outputSpeed)
{
    // Clear speed bits for this pin
    myPort->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR0 << (myPinId * 2));
    // Set speed bits for this pin
    myPort->OSPEEDR |= ((uint32_t) outputSpeed << (myPinId * 2));
}