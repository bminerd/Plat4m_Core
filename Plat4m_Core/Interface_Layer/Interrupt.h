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
 * @file Interrupt.h
 * @author Ben Minerd
 * @date 4/17/13
 * @brief Interrupt class.
 */

#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <Callback.h>
#include <GpioPin.h>

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

class Interrupt
{
public:
    
    /*--------------------------------------------------------------------------
     * Public enumerations
     *------------------------------------------------------------------------*/
    
    /**
     * @brief Enumeration of interrupt errors.
     */
    enum Error
    {
        ERROR_NONE,
        ERROR_PARAMETER_INVALID,
        ERROR_NOT_ENABLED
    };

    /**
     * @brief Enumeration of interrupt triggers.
     */
    enum Trigger
    {
        TRIGGER_RISING = 0,
        TRIGGER_FALLING,
        TRIGGER_RISING_FALLING
    };
    
    enum ActiveLevel
    {
        ACTIVE_LEVEL_HIGH = 0,
        ACTIVE_LEVEL_LOW
    };
         
    /*--------------------------------------------------------------------------
     * Public structures
     *------------------------------------------------------------------------*/
    
    struct Config
    {
        Trigger trigger;
        ActiveLevel activeLevel;
    };
    
    /*--------------------------------------------------------------------------
     * Public typedefs
     *------------------------------------------------------------------------*/
    
    typedef Callback<void, bool> HandlerCallback;
    
    /*--------------------------------------------------------------------------
     * Public virtual destructors
     *------------------------------------------------------------------------*/
    
    virtual ~Interrupt();
    
    /*--------------------------------------------------------------------------
     * Public methods
     *------------------------------------------------------------------------*/

    /**
     * @brief Sets the given interrupt enabled or disabled.
     * @param handle Interrupt handle to access.
     * @param enable Flag that indicates if the interrupt should be enabled or
     * disabled.
     * @return Interrupt error.
     */
    Error enable(const bool enable);

    /**
     * @brief Checks to see if the given interrupt is enabled or disabled.
     * @param handle Interrupt handle to access.
     * @param isEnabled Flag that indicates if the interrupt is enabled or disabled.
     * @return Interrupt error.
     */
    Error isEnabled(bool& isEnabled);

    /**
     * @brief Configures the given interrupt.
     * @param handle Interrupt handle to access.
     * @param config Interrupt configuration.
     * @return Interrupt error.
     */
    Error configure(const Config& config);
    
    Error setHandlerCallback(HandlerCallback& handlerCallback);
    
    Error isActive(bool& isActive);
    
    void handler();
    
protected:
    
    /*--------------------------------------------------------------------------
     * Protected constructors
     *------------------------------------------------------------------------*/
    
    Interrupt(GpioPin& gpioPin);
    
private:
    
    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/
    
    bool myIsEnabled;
    
    Config myConfig;
    
    GpioPin& myGpioPin;
    
    HandlerCallback* myHandlerCallback;
    
    /*--------------------------------------------------------------------------
     * Private pure virtual methods
     *------------------------------------------------------------------------*/
    
    virtual Error driverEnable(const bool enable) = 0;
    
    virtual Error driverConfigure(const Config& config) = 0;
};

#endif // _INTERRUPT_H_
