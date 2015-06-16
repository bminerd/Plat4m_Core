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
 * @file Interrupt.cpp
 * @author Ben Minerd
 * @date 4/17/13
 * @brief Interrupt class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Interrupt.h>
#include <System.h>

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 * @brief Trigger mode map.
 */
static const Interrupt::Trigger triggerMap[] =
{
    Interrupt::TRIGGER_FALLING, /// GpioPin::LEVEL_LOW
    Interrupt::TRIGGER_RISING   /// GpioPin::LEVEL_HIGH
};

static const GpioPin::Resistor resistorMap[] =
{
    GpioPin::RESISTOR_PULL_DOWN,    /// Interrupt::TRIGGER_RISING
    GpioPin::RESISTOR_PULL_UP,      /// Interrupt::TRIGGER_FALLING
    GpioPin::RESISTOR_NONE          /// Interrupt::TRIGGER_RISING_FALLING
};

static const bool activeLevelMap[2][2] =
{
    // Interrupt::ACTIVE_LEVEL_HIGH
    {
        false,  // GpioPin::LEVEL_LOW
        true    // GpioPin::LEVEL_HIGH
    },
    // Interrupt::ACTIVE_LEVEL_LOW
    {
        true,   // GpioPin::LEVEL_LOW
        false   // GpioPin::LEVEL_HIGH
    }
};

/*------------------------------------------------------------------------------
 * Public virtual destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Interrupt::~Interrupt()
{
}

/*------------------------------------------------------------------------------
 * Public methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Interrupt::Error Interrupt::enable(const bool enable)
{
    if (enable == myIsEnabled)
    {
        return ERROR_NONE;
    }

    Error error = driverEnable(enable);
    
    GpioPin::Error gpioError = myGpioPin.enable(enable);
    
    if (error == ERROR_NONE)
    {
        myIsEnabled = enable;
    }

    return error;
}

//------------------------------------------------------------------------------
Interrupt::Error Interrupt::isEnabled(bool& isEnabled)
{
    isEnabled = myIsEnabled;

    return ERROR_NONE;
}

//------------------------------------------------------------------------------
Interrupt::Error Interrupt::configure(const Config& config)
{
    GpioPin::Config gpioConfig;
    gpioConfig.mode     = GpioPin::MODE_DIGITAL_INPUT;
    gpioConfig.resistor = resistorMap[config.trigger];
    
    myGpioPin.configure(gpioConfig);
    
    Error error = driverConfigure(config);
    
    if (error == ERROR_NONE)
    {
        myConfig = config;
    }

    return ERROR_NONE;
}

//------------------------------------------------------------------------------
Interrupt::Error Interrupt::setHandlerCallback(HandlerCallback& handlerCallback)
{
    if (IS_NULL_POINTER(myHandlerCallback))
    {
        myHandlerCallback = &handlerCallback;
    }
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
Interrupt::Error Interrupt::isActive(bool& isActive)
{
    GpioPin::Level level;
    myGpioPin.readLevel(level);
    
    isActive = activeLevelMap[myConfig.activeLevel][level];
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
void Interrupt::handler()
{
    if (IS_VALID_POINTER(myHandlerCallback))
    {
        bool isActive = true;
            
        if (myConfig.trigger == TRIGGER_RISING_FALLING)
        {
            GpioPin::Level level;
            myGpioPin.readLevel(level);
            
            isActive = activeLevelMap[myConfig.activeLevel][level];
        }
    
        myHandlerCallback->call(isActive);
    }
}

/*------------------------------------------------------------------------------
 * Protected constructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Interrupt::Interrupt(GpioPin& gpioPin) :
    myIsEnabled(false),
    myGpioPin(gpioPin),
    myHandlerCallback(NULL_POINTER)
{
}