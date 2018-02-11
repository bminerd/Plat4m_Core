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
// Copyright (c) 2015 Benjamin Minerd
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
/// @file ExternalInterrupt.cpp
/// @author Ben Minerd
/// @date 9/25/2015
/// @brief ExternalInterrupt class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/ExternalInterrupt.h>
#include <Plat4m_Core/System.h>

using Plat4m::ExternalInterrupt;
using Plat4m::GpioPin;
using Plat4m::Module;

//------------------------------------------------------------------------------
// Local variables
//------------------------------------------------------------------------------

static const ExternalInterrupt::Trigger triggerMap[] =
{
    ExternalInterrupt::TRIGGER_FALLING, /// GpioPin::LEVEL_LOW
    ExternalInterrupt::TRIGGER_RISING   /// GpioPin::LEVEL_HIGH
};

static const GpioPin::Resistor resistorMap[] =
{
    GpioPin::RESISTOR_PULL_DOWN,    /// ExternalInterrupt::TRIGGER_RISING
    GpioPin::RESISTOR_PULL_UP,      /// ExternalInterrupt::TRIGGER_FALLING
    GpioPin::RESISTOR_NONE          /// ExternalInterrupt::TRIGGER_RISING_FALLING
};

static const bool activeLevelMap[2][2] =
{
    // ExternalInterrupt::ACTIVE_LEVEL_HIGH
    {
        false,  // GpioPin::LEVEL_LOW
        true    // GpioPin::LEVEL_HIGH
    },
    // ExternalInterrupt::ACTIVE_LEVEL_LOW
    {
        true,   // GpioPin::LEVEL_LOW
        false   // GpioPin::LEVEL_HIGH
    }
};

//------------------------------------------------------------------------------
// Public virtual methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ExternalInterrupt::Error ExternalInterrupt::configure(const Config& config)
{
    GpioPin::Config gpioConfig;
    gpioConfig.mode     = GpioPin::MODE_DIGITAL_INPUT;
    gpioConfig.resistor = resistorMap[config.trigger];
    
    myGpioPin.configure(gpioConfig);
    
    Error error = driverConfigure(config);
    
    if (error.getCode() == ERROR_CODE_NONE)
    {
        myConfig = config;

        if (isValidPointer(myHandlerCallback))
        {
            GpioPin::Level level;
            myGpioPin.readLevel(level);

            bool isActive = activeLevelMap[myConfig.activeLevel][level];

            if (isActive)
            {
                myHandlerCallback->call(isActive);
            }
        }
    }

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
void ExternalInterrupt::setHandlerCallback(HandlerCallback& handlerCallback)
{
    if (isNullPointer(myHandlerCallback))
    {
        myHandlerCallback = &handlerCallback;
    }
}

//------------------------------------------------------------------------------
ExternalInterrupt::Error ExternalInterrupt::isActive(bool& isActive)
{
    GpioPin::Level level;
    myGpioPin.readLevel(level);
    
    isActive = activeLevelMap[myConfig.activeLevel][level];
    
    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
void ExternalInterrupt::interruptHandler()
{
    if (isValidPointer(myHandlerCallback))
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

//------------------------------------------------------------------------------
// Protected constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ExternalInterrupt::ExternalInterrupt(GpioPin& gpioPin) :
    myGpioPin(gpioPin),
    myHandlerCallback(0)
{
}

//------------------------------------------------------------------------------
// Protected virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ExternalInterrupt::~ExternalInterrupt()
{
}

//------------------------------------------------------------------------------
// Private virtual methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error ExternalInterrupt::interfaceEnable(const bool enable)
{
    myGpioPin.enable(enable);

    return Module::Error(Module::ERROR_CODE_NONE);
}
