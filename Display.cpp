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
 * @file Display.cpp
 * @author Ben Minerd
 * @date 4/11/2013
 * @brief Display class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Display.h>

/*------------------------------------------------------------------------------
 * Public virtual destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Display::~Display()
{
}

/*------------------------------------------------------------------------------
 * Public methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
unsigned int Display::getWidth() const
{
    return (myFrame.width);
}

//------------------------------------------------------------------------------
unsigned int Display::getHeight() const
{
    return (myFrame.height);
}

//------------------------------------------------------------------------------
Display::Frame& Display::getFrame()
{
    return myFrame;
}

//------------------------------------------------------------------------------
Display::Error Display::enable(const bool enable)
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
Display::Error Display::isEnabled(bool& isEnabled)
{
    isEnabled = myIsEnabled;

    return ERROR_NONE;
}

//------------------------------------------------------------------------------
Display::Error Display::configure(const Config& config)
{
    if (!myIsEnabled)
    {
        return ERROR_NOT_ENABLED;
    }

    Error error = driverConfigure(config);

    return error;
}

//------------------------------------------------------------------------------
Display::Error Display::writeFrame()
{
    if (!myIsEnabled)
    {
        return ERROR_NOT_ENABLED;
    }
    
    Error error = driverWriteFrame();
    
    return error;
}

//------------------------------------------------------------------------------
Display::Error Display::clear()
{
    if (!myIsEnabled)
    {
        return ERROR_NOT_ENABLED;
    }

    Error error = driverClear();

    return error;
}

//------------------------------------------------------------------------------
Display::Error Display::setBrightnessPercent(const float brightnessPercent)
{
    if (!myIsEnabled)
    {
        return ERROR_NOT_ENABLED;
    }

    Error error = driverSetBrightnessPercent(brightnessPercent);

    return error;
}

/*------------------------------------------------------------------------------
 * Protected constructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Display::Display(Frame& frame) :
    myIsEnabled(false),
    myFrame(frame)
{
}