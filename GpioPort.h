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
 * @file GpioPort.h
 * @author Ben Minerd
 * @date 3/25/13
 * @brief GpioPort class.
 */

#ifndef _GPIO_PORT_H_
#define _GPIO_PORT_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <GpioPin.h>

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

class GpioPort
{
public:
    
    /*--------------------------------------------------------------------------
     * Public enumerations
     *------------------------------------------------------------------------*/
    
    /**
     * @brief Enumeration of GPIO errors.
     */
    enum Error
    {
        ERROR_NONE,
        ERROR_NOT_ENABLED
    };

    /**
     * @brief Enumeration of GPIO modes.
     */
    enum Mode
    {
        MODE_DIGITAL_OUTPUT = 0,
        MODE_DIGITAL_INPUT
    };

    /**
     * @brief Enumeration of GPIO resistor modes.
     */
    enum Resistor
    {
        RESISTOR_NONE = 0,
        RESISTOR_PULL_UP,
        RESISTOR_PULL_DOWN
    };
    
    /*--------------------------------------------------------------------------
     * Public structures
     *------------------------------------------------------------------------*/
    
    typedef id_t Id;

    struct Config
    {
        Mode mode;
        Resistor resistor;
    };
    
    /*--------------------------------------------------------------------------
     * Public virtual destructors
     *------------------------------------------------------------------------*/
    
    virtual ~GpioPort();
    
    /*--------------------------------------------------------------------------
     * Public methods
     *------------------------------------------------------------------------*/
    
    Error enable(const bool enable);
    
    Error isEnabled(bool& isEnabled);
    
    Error configure(const Config& config);
    
    Error setValue(const unsigned int value);
    
    Error getValue(unsigned int& value);
    
    Error readValue(unsigned int& value);
    
protected:
    
    /*--------------------------------------------------------------------------
     * Protected constructors and destructors
     *------------------------------------------------------------------------*/
    
    GpioPort();
    
private:
    
    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/
    
    bool myIsEnabled;
    
    Config myConfig;
    
    /*--------------------------------------------------------------------------
     * Private virtual methods
     *------------------------------------------------------------------------*/
    
    virtual Error driverEnable(const bool enable) = 0;
    
    virtual Error driverConfigure(const Config& config) = 0;
    
    virtual Error driverSetValue(const unsigned int value) = 0;
    
    virtual Error driverGetValue(unsigned int& value) = 0;
    
    virtual Error driverReadValue(unsigned int& value) = 0;
};

#endif // _GPIO_PORT_H_