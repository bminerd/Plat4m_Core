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
 * @file EnableLine.h
 * @author Ben Minerd
 * @date 4/18/13
 * @brief EnableLine class.
 */

#ifndef ENABLE_LINE_H
#define ENABLE_LINE_H

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <Module.h>
#include <ErrorTemplate.h>
#include <GpioPin.h>

namespace Plat4m
{

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

class EnableLine : public Module
{
public:
    
    /*--------------------------------------------------------------------------
     * Public enumerations
     *------------------------------------------------------------------------*/
    
    /**
     * @brief Enumeration of enable line errors.
     */
    enum ErrorCode
    {
        ERROR_CODE_NONE,
        ERROR_CODE_ENABLED,
        ERROR_CODE_MODE_NOT_INPUT,
        ERROR_CODE_MODE_NOT_OUTPUT,
        ERROR_CODE_COMMUNICATION
    };
    
    /**
     * @brief Enumeration of enable line modes.
     */
    enum Mode
    {
        MODE_INPUT,
        MODE_OUTPUT
    };
    
    /**
     * @brief Enumeration of enable line levels.
     */
    enum ActiveLevel
    {
        ACTIVE_LEVEL_HIGH = 0,
        ACTIVE_LEVEL_LOW
    };

    /*--------------------------------------------------------------------------
     * Public typedefs
     *------------------------------------------------------------------------*/

    typedef ErrorTemplate<ErrorCode> Error;
    
    /*--------------------------------------------------------------------------
     * Public constructors
     *------------------------------------------------------------------------*/
    
    EnableLine(const Mode mode,
               const ActiveLevel activeLevel,
               GpioPin& gpioPin,
               const bool usePullResistor);
    
    /*--------------------------------------------------------------------------
     * Public destructors
     *------------------------------------------------------------------------*/
    
    ~EnableLine();
    
    /*--------------------------------------------------------------------------
     * Public methods
     *------------------------------------------------------------------------*/
    
    Error setActive(const bool active);
    
    Error isActive(bool& isActive);
    
    Error pulseActive(const bool active, const uint32_t timeMs = 0);
    
    Error toggleActive();
    
private:
    
    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/
    
    const Mode myMode;
    
    const ActiveLevel myActiveLevel;
    
    const bool myUsePullResistor;
    
    bool myIsActive;
    
    GpioPin& myGpioPin;

    /*--------------------------------------------------------------------------
     * Private methods implemented from Module
     *------------------------------------------------------------------------*/

    Module::Error driverEnable(const bool enable) override;
};

}; // namespace Plat4m

#endif // ENABLE_LINE_H
