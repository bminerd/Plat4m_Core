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
 * @file GpioPinIoExpanderMcp23008.cpp
 * @author Ben Minerd
 * @date 3/26/2013
 * @brief GpioPinIoExpanderMcp23008 class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <GpioPinIoExpanderMcp23008.h>

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

static const char* pinStringMap[] =
{
    "0",    /// PIN_ID_0
    "1",    /// PIN_ID_1
    "2",    /// PIN_ID_2
    "3",    /// PIN_ID_3
    "4",    /// PIN_ID_4
    "5",    /// PIN_ID_5
    "6",    /// PIN_ID_6
    "7"     /// PIN_ID_7
};

/*------------------------------------------------------------------------------
 * Public constructors and destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
GpioPinIoExpanderMcp23008::GpioPinIoExpanderMcp23008(
                                               const PinId pinId,
                                               IoExpanderMcp23008& ioExpander) :
    GpioPin("Only", pinStringMap[pinId]),
    myPinId(pinId),
    myLevel(LEVEL_LOW),
    myIoExpander(ioExpander)
{
}

/*------------------------------------------------------------------------------
 * Private implemented methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
GpioPin::Error GpioPinIoExpanderMcp23008::driverEnable(const bool enable)
{
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
GpioPin::Error GpioPinIoExpanderMcp23008::driverConfigure(const Config& config)
{
    if (myIoExpander.pinConfigure(myPinId, config) != IoExpander::ERROR_NONE)
    {
        return ERROR_COMMUNICATION;
    }

    return ERROR_NONE;
}

//------------------------------------------------------------------------------
GpioPin::Error GpioPinIoExpanderMcp23008::driverSetLevel(const Level level)
{
    if (myIoExpander.pinSetLevel(myPinId, level) != IoExpander::ERROR_NONE)
    {
        return ERROR_COMMUNICATION;
    }
    
    myLevel = level;

    return ERROR_NONE;
}

//------------------------------------------------------------------------------
GpioPin::Error GpioPinIoExpanderMcp23008::driverGetLevel(Level& level)
{
    if (myIoExpander.pinGetLevel(myPinId, level) != IoExpander::ERROR_NONE)
    {
        return ERROR_COMMUNICATION;
    }

    return ERROR_NONE;
}

//------------------------------------------------------------------------------
GpioPin::Error GpioPinIoExpanderMcp23008::driverReadLevel(Level& level)
{
    if (myIoExpander.pinGetLevel(myPinId, level) != IoExpander::ERROR_NONE)
    {
        return ERROR_COMMUNICATION;
    }

    return ERROR_NONE;
}

//------------------------------------------------------------------------------
GpioPin::Error GpioPinIoExpanderMcp23008::driverToggleLevel()
{
    return driverSetLevel((Level) !myLevel);
}