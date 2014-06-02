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
* @file GpioPortStm32f4xx.cpp
 * @author Ben Minerd
 * @date 3/25/13
 * @brief GpioPortStm32f4xx class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <GpioPortStm32f4xx.h>

#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/**
 * @brief Define for GPIO clock speed.
 */
#define GPIO_CLOCK_SPEED (GPIO_Speed_50MHz)
     
/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Structures
 *----------------------------------------------------------------------------*/

static const uint32_t modeMap[] =
{
    0x55555555, /// GPIO_MODE_DIGITAL_OUTPUT
    0x00000000, /// GPIO_MODE_DIGITAL_INPUT
    0xFFFFFFFF, /// GPIO_MODE_ANALOG
    0xAAAAAAAA  /// GPIO_MODE_ALTERNATE_FUNCTION
};

static const uint32_t resistorMap[] =
{
    0x00000000, /// GPIO_RESISTOR_NONE
    0x55555555, /// GPIO_RESISTOR_PULL_UP
    0xAAAAAAAA  /// GPIO_RESISTOR_PULL_DOWN
};

/*------------------------------------------------------------------------------
 * Public constructors and destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
GpioPortStm32f4xx::GpioPortStm32f4xx(const GpioStm32f4xx::PortId portId) :
    GpioPort(),
    myPortId(portId),
    myPort(GpioStm32f4xx::portMap[portId])
{
}

/*------------------------------------------------------------------------------
 * Private methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
GpioPort::Error GpioPortStm32f4xx::driverEnable(const bool enable)
{
    RCC_AHB1PeriphClockCmd(GpioStm32f4xx::clockMap[myPortId],
                           (FunctionalState) enable);
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
GpioPort::Error GpioPortStm32f4xx::driverConfigure(const Config& config)
{
    // Set mode
    
    // Set mode bits for this port
    myPort->MODER = modeMap[config.mode];
    
    // Set resistor
    
    // Set resistor bits for this pin
    myPort->PUPDR = resistorMap[config.resistor];
    
    if ((config.mode) == MODE_DIGITAL_OUTPUT)
    {
        // Set speed bits for this pin
        myPort->OSPEEDR = 0xAAAAAAAA;
        
        // Set output bits for this pin
        myPort->OTYPER = 0x00000000;
    }
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
GpioPort::Error GpioPortStm32f4xx::driverSetValue(const unsigned int value)
{
    myPort->ODR = (uint16_t) value;
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
GpioPort::Error GpioPortStm32f4xx::driverGetValue(unsigned int& value)
{
    value = (myPort->ODR & 0xFFFF);
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
GpioPort::Error GpioPortStm32f4xx::driverReadValue(unsigned int& value)
{
    value = (myPort->IDR & 0xFFFF);
    
    return ERROR_NONE;
}