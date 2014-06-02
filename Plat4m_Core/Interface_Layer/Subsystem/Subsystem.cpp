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
 * @file GpioPin.cpp
 * @author Ben Minerd
 * @date 3/22/13
 * @brief GpioPin class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <GpioPin.h>

/*------------------------------------------------------------------------------
 * Public virtual destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
GpioPin::~GpioPin()
{
}

/*------------------------------------------------------------------------------
 * Public methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
GpioPin::Error GpioPin::enable(const bool enable)
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
GpioPin::Error GpioPin::isEnabled(bool& isEnabled)
{
    isEnabled = myIsEnabled;

    return ERROR_NONE;
}

//------------------------------------------------------------------------------
GpioPin::Error GpioPin::configure(const Config& config)
{
    Error error = driverConfigure(config);

    return error;
}

//------------------------------------------------------------------------------
GpioPin::Error GpioPin::setLevel(const Level level)
{
    if (!myIsEnabled)
    {
        return ERROR_NOT_ENABLED;
    }

    Error error = driverSetLevel(level);

    return error;
}

//------------------------------------------------------------------------------
GpioPin::Error GpioPin::getLevel(Level& level)
{
    if (!myIsEnabled)
    {
        return ERROR_NOT_ENABLED;
    }

    Error error = driverGetLevel(level);

    return error;
}

//------------------------------------------------------------------------------
GpioPin::Error GpioPin::readLevel(Level& level)
{
    if (!myIsEnabled)
    {
        return ERROR_NOT_ENABLED;
    }

    Error error = driverReadLevel(level);

    return error;
}

//------------------------------------------------------------------------------
GpioPin::Error GpioPin::toggleLevel()
{
    if (!myIsEnabled)
    {
        return ERROR_NOT_ENABLED;
    }

    Error error = driverToggleLevel();

    return error;
}

/*------------------------------------------------------------------------------
 * Protected constructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
GpioPin::GpioPin(const char* portIdString, const char* pinIdString) :
    myPortIdString(portIdString),
    myPinIdString(pinIdString),
    myIsEnabled(false)
{
}

/*------------------------------------------------------------------------------
 * Protected methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
GpioPin::Config& GpioPin::getConfig()
{
    return myConfig;
}