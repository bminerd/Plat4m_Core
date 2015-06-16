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
 * @file EnableLine.cpp
 * @author Ben Minerd
 * @date 4/18/13
 * @brief EnableLine class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <EnableLine.h>
#include <System.h>

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

static GpioPin::Resistor gpioResistorMap[] =
{
    GpioPin::RESISTOR_PULL_DOWN,    // ACTIVE_LEVEL_HIGH
    GpioPin::RESISTOR_PULL_UP       // ACTIVE_LEVEL_LOW
};

static GpioPin::Level gpioLevelMap[2][2] =
{
    // ACTIVE_HIGH
    {
        GpioPin::LEVEL_LOW,     // Active == false
        GpioPin::LEVEL_HIGH     // Active == true
    },
    // ACTIVE_LOW
    {
        GpioPin::LEVEL_HIGH,    // Active == false
        GpioPin::LEVEL_LOW      // Active == true
    }
};

static bool activeLevelMap[2][2] =
{
    // ACTIVE_HIGH
    {
        false,  // GpioPin::LEVEL_LOW
        true    // GpioPin::LEVEL_HIGH
    },
    // ACTIVE_LOW
    {
        true,   // GpioPin::LEVEL_LOW
        false   // GpioPin::LEVEL_HIGH
    }
};

/*------------------------------------------------------------------------------
 * Public constructors and destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
EnableLine::EnableLine(const Mode mode,
                       const ActiveLevel activeLevel,
                       GpioPin& gpioPin,
                       const bool usePullResistor) :
    myMode(mode),
    myActiveLevel(activeLevel),
    myUsePullResistor(usePullResistor),
    myIsEnabled(false),
    myIsActive(false),
    myGpioPin(gpioPin)
{
}

/*------------------------------------------------------------------------------
 * Public methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
EnableLine::Error EnableLine::enable(const bool enable)
{
    if (enable == myIsEnabled)
    {
        return ERROR_NONE;
    }
    
    GpioPin::Error error;
    
    error = myGpioPin.enable(enable);
    
    if (enable)
    {
        GpioPin::Config gpioConfig;
        
        if (myMode == MODE_INPUT)
        {
            gpioConfig.mode = GpioPin::MODE_DIGITAL_INPUT;
        }
        else // myMode == MODE_OUTPUT
        {
            gpioConfig.mode = GpioPin::MODE_DIGITAL_OUTPUT;
        }
        
        if (myUsePullResistor)
        {
            gpioConfig.resistor = gpioResistorMap[myActiveLevel];
        }
        else
        {
            gpioConfig.resistor = GpioPin::RESISTOR_NONE;
        }
        
        error = myGpioPin.configure(gpioConfig);
        
        if (myMode == MODE_OUTPUT)
        {
            error = myGpioPin.setLevel(gpioLevelMap[myActiveLevel][false]);
        }
    }
    
    if (error == GpioPin::ERROR_NONE)
    {
        myIsEnabled = enable;
    }
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
bool EnableLine::isEnabled()
{
    return myIsEnabled;
}

//------------------------------------------------------------------------------
EnableLine::Error EnableLine::setActive(const bool active)
{
    if (active == myIsActive)
    {
        return ERROR_NONE;
    }
    
    GpioPin::Error error = myGpioPin.
                           setLevel(gpioLevelMap[myActiveLevel][active]);
    
    if (error == GpioPin::ERROR_NONE)
    {
        myIsActive = active;
    }
    
    // Error?
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
EnableLine::Error EnableLine::isActive(bool& isActive)
{
    if (!myIsEnabled)
    {
        isActive = false;
    }
    
    if (myMode == MODE_OUTPUT)
    {
        isActive = myIsActive;
    }
    else
    {
        GpioPin::Level level;
        GpioPin::Error error = myGpioPin.readLevel(level);
        isActive = activeLevelMap[myActiveLevel][level];
    }
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
EnableLine::Error EnableLine::pulseActive(const bool active,
                                          const uint32_t timeMs)
{
    Error error = setActive(active);
    
    if (timeMs != 0)
    {
        System::timeDelayMs(timeMs);
    }
    
    return setActive(!active);
}

//------------------------------------------------------------------------------
EnableLine::Error EnableLine::toggleActive()
{
    return setActive(!myIsActive);
}