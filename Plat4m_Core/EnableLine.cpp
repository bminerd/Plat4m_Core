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
// Copyright (c) 2013 Benjamin Minerd
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
/// @file EnableLine.cpp
/// @author Ben Minerd
/// @date 4/18/2013
/// @brief EnableLine class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/EnableLine.h>

using Plat4m::EnableLine;
using Plat4m::Module;
using Plat4m::GpioPin;

//------------------------------------------------------------------------------
// Local variables
//------------------------------------------------------------------------------

static const GpioPin::Resistor gpioResistorMap[] =
{
    GpioPin::RESISTOR_PULL_DOWN, /// EnableLine::LEVEL_HIGH
    GpioPin::RESISTOR_PULL_UP    /// EnableLine::LEVEL_LOW
};

static const GpioPin::Level gpioLevelMap[2][2] =
{
    // Active::HIGH
    {
        GpioPin::LEVEL_LOW, /// Active == false
        GpioPin::LEVEL_HIGH /// Active == true
    },
    // Active::LOW
    {
        GpioPin::LEVEL_HIGH, /// Active == false
        GpioPin::LEVEL_LOW   /// Active == true
    }
};

static const bool activeLevelMap[2][2] =
{
    // Active::HIGH
    {
        false, /// GpioPin::LEVEL_LOW
        true   /// GpioPin::LEVEL_HIGH
    },
    // Active::LOW
    {
        true, /// GpioPin::LEVEL_LOW
        false /// GpioPin::LEVEL_HIGH
    }
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
EnableLine::EnableLine(const Mode mode,
                       const ActiveLevel activeLevel,
                       GpioPin& gpioPin,
                       const bool usePullResistor) :
    myMode(mode),
    myActiveLevel(activeLevel),
    myUsePullResistor(usePullResistor),
    myIsActive(false),
    myGpioPin(gpioPin)
{
}

//------------------------------------------------------------------------------
// Public destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
EnableLine::~EnableLine()
{
}

//------------------------------------------------------------------------------
// Public virtual methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GpioPin& EnableLine::getGpioPin()
{
	return myGpioPin;
}

//------------------------------------------------------------------------------
EnableLine::Error EnableLine::setActive(const bool active)
{
    if (active == myIsActive)
    {
        return Error(ERROR_CODE_NONE);
    }
    
    GpioPin::Error error =
                        myGpioPin.setLevel(gpioLevelMap[myActiveLevel][active]);
    
    if (error.getCode() == GpioPin::ERROR_CODE_NONE)
    {
        myIsActive = active;
    }
    
    // Error?
    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
EnableLine::Error EnableLine::isActive(bool& isActive)
{
    if (!isEnabled())
    {
        isActive = false;
    }
    
    Error error(ERROR_CODE_NONE);

    if (myMode == MODE_OUTPUT)
    {
        isActive = myIsActive;
    }
    else
    {
        GpioPin::Level level;
        myGpioPin.readLevel(level);
        
        isActive = activeLevelMap[myActiveLevel][level];
    }
    
    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
EnableLine::Error EnableLine::toggleActive()
{
    return setActive(!myIsActive);
}

//------------------------------------------------------------------------------
// Private virtual methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error EnableLine::driverEnable(const bool enable)
{
    if (enable == isEnabled())
    {
        return Module::Error(Module::ERROR_CODE_NONE);
    }

    Module::Error moduleError = myGpioPin.enable(enable);
    GpioPin::Error gpioPinError;

    if (enable)
    {
        GpioPin::Config gpioConfig;

        if (myMode == MODE_INPUT)
        {
            gpioConfig.mode = GpioPin::MODE_DIGITAL_INPUT;
        }
        else // myMode == MODE_OUTPUT
        {
        	// TODO: Update GpioPin to improve output mode selection
            gpioConfig.mode = GpioPin::MODE_DIGITAL_OUTPUT_PUSH_PULL;
        }

        if (myUsePullResistor)
        {
            gpioConfig.resistor = gpioResistorMap[myActiveLevel];
        }
        else
        {
            gpioConfig.resistor = GpioPin::RESISTOR_NONE;
        }

        gpioPinError = myGpioPin.configure(gpioConfig);

        if (myMode == MODE_OUTPUT)
        {
            gpioPinError = myGpioPin.setLevel(gpioLevelMap[myActiveLevel][false]);
        }
    }

    if ((moduleError.getCode() != Module::ERROR_CODE_NONE) ||
        (gpioPinError.getCode() != GpioPin::ERROR_CODE_NONE))
    {
        return Module::Error(Module::ERROR_CODE_ENABLE_FAILED);
    }

    return Module::Error(Module::ERROR_CODE_NONE);
}
