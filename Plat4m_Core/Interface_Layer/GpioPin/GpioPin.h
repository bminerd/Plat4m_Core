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
 * @file GpioPin.h
 * @author Ben Minerd
 * @date 3/22/13
 * @brief GpioPin class.
 */

#ifndef _GPIO_PIN_H_
#define _GPIO_PIN_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

class GpioPin
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
        ERROR_NOT_ENABLED,
        ERROR_COMMUNICATION
    };
    
    /**
     * @brief Enumeration of GPIO levels.
     */
    enum Level
    {
        LEVEL_LOW  = 0,   /// Digital low / logic 0
        LEVEL_HIGH = 1    /// Digital high / logic 1
    };
    
    /**
     * @brief Enumeration of GPIO modes.
     */
    enum Mode
    {
        MODE_DIGITAL_OUTPUT = 0,
        MODE_DIGITAL_INPUT,
        MODE_ANALOG,
        MODE_ALTERNATE_FUNCTION
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
    
    struct Id
    {
        Plat4m::Id portId;
        Plat4m::Id pinId;
    };
    
    /**
     * @brief GPIO configuration type.
     */
    struct Config
    {
        Mode mode;
        Resistor resistor;
    };
    
    /*--------------------------------------------------------------------------
     * Public virtual destructors
     *------------------------------------------------------------------------*/
    
    virtual ~GpioPin();
    
    /*--------------------------------------------------------------------------
     * Public methods
     *------------------------------------------------------------------------*/
    
    Error enable(const bool enable);
    
    Error isEnabled(bool& isEnabled);
    
    Error configure(const Config& config);
    
    Error setLevel(const Level level);
    
    Error getLevel(Level& level);
    
    Error readLevel(Level& level);
    
    Error toggleLevel();
    
protected:

    /*--------------------------------------------------------------------------
     * Protected constructors
     *------------------------------------------------------------------------*/
    
    GpioPin(const char* portIdString, const char* pinIdString);
    
    /*--------------------------------------------------------------------------
     * Protected methods
     *------------------------------------------------------------------------*/
         
    Config& getConfig();
    
private:

    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/

    const char* myPortIdString;
    const char* myPinIdString;

    bool myIsEnabled;
    
    Config myConfig;
    
    /*--------------------------------------------------------------------------
     * Private pure virtual methods
     *------------------------------------------------------------------------*/
    
    virtual Error driverEnable(const bool enable) = 0;
    
    virtual Error driverConfigure(const Config& config) = 0;
    
    virtual Error driverSetLevel(const Level level) = 0;
    
    virtual Error driverGetLevel(Level& level) = 0;
    
    virtual Error driverReadLevel(Level& level) = 0;
    
    virtual Error driverToggleLevel() = 0;
};

#endif // _GPIO_PIN_H_