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
 * @file ExternalInterruptTest.h
 * @author Ben Minerd
 * @date 9/25/15
 * @brief ExternalInterruptTest class.
 */

#ifndef EXTERNAL_INTERRUPT_TEST_H
#define EXTERNAL_INTERRUPT_TEST_H

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <ExternalInterrupt.h>
#include <ErrorTemplate.h>
#include <Callback.h>
#include <GpioPin.h>

namespace Plat4m
{

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

class ExternalInterruptTest : public ExternalInterrupt
{
public:
    
    /*--------------------------------------------------------------------------
     * Public enumerations
     *------------------------------------------------------------------------*/
    
    /**
     * @brief Enumeration of external interrupt errors.
     */
    enum ErrorCode
    {
        ERROR_CODE_NONE,
        ERROR_CODE_PARAMETER_INVALID,
        ERROR_CODE_NOT_ENABLED
    };

    /*--------------------------------------------------------------------------
     * Public typedefs
     *------------------------------------------------------------------------*/

    typedef ErrorTemplate<ErrorCode> Error;

    /*--------------------------------------------------------------------------
     * Public structures
     *------------------------------------------------------------------------*/
    
    struct Config
    {
        int a;
    };
    
    /*--------------------------------------------------------------------------
     * Public constructors
     *------------------------------------------------------------------------*/
    
    ExternalInterruptTest(GpioPin& gpioPin);
    
private:

    /*--------------------------------------------------------------------------
     * Private methods implemented from Module
     *------------------------------------------------------------------------*/

    Module::Error driverEnable(const bool enable) override;

    /*--------------------------------------------------------------------------
     * Private methods implemented from ExternalInterrupt
     *------------------------------------------------------------------------*/
    
    ExternalInterrupt::Error driverConfigure(
                              const ExternalInterrupt::Config& config) override;
};

}; // namespace Plat4m

#endif // EXTERNAL_INTERRUPT_TEST_H
