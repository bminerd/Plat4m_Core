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
 * @file GpioPinTest.cpp
 * @author Ben Minerd
 * @date 3/22/13
 * @brief GpioPinTest class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <GpioPinTest.h>

using Plat4m::GpioPinTest;
using Plat4m::GpioPin;
using Plat4m::Module;

/*------------------------------------------------------------------------------
 * Public constructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
GpioPinTest::GpioPinTest(const char* portIdString, const char* pinIdString) :
    GpioPin(portIdString, pinIdString)
{
}

/*------------------------------------------------------------------------------
 * Private methods implemented from Module
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Module::Error GpioPinTest::driverEnable(const bool enable)
{
    return Module::Error(Module::ERROR_CODE_NONE);
}

/*------------------------------------------------------------------------------
 * Private methods implemented from Module
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
GpioPin::Error GpioPinTest::driverConfigure(const GpioPin::Config& config)
{
    return GpioPin::Error(GpioPin::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
GpioPin::Error GpioPinTest::driverSetLevel(const Level level)
{
    return GpioPin::Error(GpioPin::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
GpioPin::Error GpioPinTest::driverGetLevel(Level& level)
{
    return GpioPin::Error(GpioPin::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
GpioPin::Error GpioPinTest::driverReadLevel(Level& level)
{
    return GpioPin::Error(GpioPin::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
GpioPin::Error GpioPinTest::driverToggleLevel()
{
    return GpioPin::Error(GpioPin::ERROR_CODE_NONE);
}
