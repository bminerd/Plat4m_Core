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
 * Copyright (c) 2015 Benjamin Minerd
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
 * @file ExternalInterruptTest.cpp
 * @author Ben Minerd
 * @date 4/17/13
 * @brief ExternalInterruptTest class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <ExternalInterruptTest.h>
#include <System.h>

using Plat4m::ExternalInterruptTest;
using Plat4m::ExternalInterrupt;
using Plat4m::Module;

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Public constructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
ExternalInterruptTest::ExternalInterruptTest(GpioPin& gpioPin) :
    ExternalInterrupt(gpioPin)
{
}

/*------------------------------------------------------------------------------
 * Private methods implemented from Module
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Module::Error ExternalInterruptTest::driverEnable(const bool enable)
{
    return Module::Error(Module::ERROR_CODE_NONE);
}

/*------------------------------------------------------------------------------
 * Private methods implemented from ExternalInterrupt
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
ExternalInterrupt::Error ExternalInterruptTest::driverConfigure(
                                        const ExternalInterrupt::Config& config)
{
    return ExternalInterrupt::Error(ExternalInterrupt::ERROR_CODE_NONE);
}
