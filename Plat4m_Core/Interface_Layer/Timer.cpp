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
 * @file Timer.cpp
 * @author Ben Minerd
 * @date 5/15/2013
 * @brief Timer class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Timer.h>

/*------------------------------------------------------------------------------
 * Public virtual destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Timer::~Timer()
{
}

/*------------------------------------------------------------------------------
 * Public methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Timer::Error Timer::enable(const bool enable)
{
    if (enable == myIsEnabled)
    {
        return ERROR_NONE;
    }
    
    Error error = driverEnable(enable);
    
    if (error == ERROR_NONE)
    {
        myIsEnabled = enable;
    }
    
    return error;
}

//------------------------------------------------------------------------------
Timer::Error Timer::isEnabled(bool& isEnabled)
{
    isEnabled = myIsEnabled;
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
Timer::Error Timer::configure(const Config& config)
{
    if (!myIsEnabled)
    {
        return ERROR_NOT_ENABLED;
    }
    
    Error error = driverConfigure(config);
    
    if (error == ERROR_NONE)
    {
        myConfig = config;
    }
    
    return error;
}

//------------------------------------------------------------------------------
const Timer::Config& Timer::getConfig() const
{
    return myConfig;
}
    
//------------------------------------------------------------------------------
Timer::Error Timer::pwmConfigure(const PwmConfig& config)
{
    if (!myIsEnabled)
    {
        return ERROR_NOT_ENABLED;
    }
    
    if ((myConfig.mode != MODE_PWM_INPUT) && (myConfig.mode != MODE_PWM_OUTPUT))
    {
        return ERROR_MODE_INVALID;
    }
    
    Error error = driverPwmConfigure(config);
    
    return error;
}

/*------------------------------------------------------------------------------
 * Protected constructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Timer::Timer() :
    myIsEnabled(false)
{
}