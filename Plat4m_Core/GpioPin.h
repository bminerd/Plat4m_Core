//------------------------------------------------------------------------------
//       _______    __                           ___
//      ||  ___ \  || |             __          //  |
//      || |  || | || |   _______  || |__      //   |    _____  ___
//      || |__|| | || |  // ___  | ||  __|    // _  |   ||  _ \/ _ \
//      ||  ____/  || | || |  || | || |      // /|| |   || |\\  /\\ \
//      || |       || | || |__|| | || |     // /_|| |_  || | || | || |
//      || |       || |  \\____  | || |__  //_____   _| || | || | || |
//      ||_|       ||_|       ||_|  \\___|       ||_|   ||_| ||_| ||_|
//
//
// The MIT License (MIT)
//
// Copyright (c) 2013 Benjamin Minerd
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------------

///
/// @file GpioPin.h
/// @author Ben Minerd
/// @date 3/22/2013
/// @brief GpioPin class header file.
///

#ifndef PLAT4M_GPIO_PIN_H
#define PLAT4M_GPIO_PIN_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/Module.h>
#include <Plat4m_Core/ErrorTemplate.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class GpioPin : public Module
{
public:
    
    //--------------------------------------------------------------------------
    // Public enumerations
    //--------------------------------------------------------------------------
    
    enum ErrorCode
    {
        ERROR_CODE_NONE,
        ERROR_CODE_NOT_ENABLED,
        ERROR_CODE_SET_LEVEL_FAILED,
        ERROR_CODE_COMMUNICATION
    };
    
    enum Level
    {
        LEVEL_LOW  = 0, /// Digital low / logic 0
        LEVEL_HIGH = 1  /// Digital high / logic 1
    };
    
    enum Mode
    {
        MODE_DIGITAL_OUTPUT_PUSH_PULL = 0,
		MODE_DIGITAL_OUTPUT_OPEN_DRAIN,
        MODE_DIGITAL_INPUT,
        MODE_ANALOG,
        MODE_ALTERNATE_FUNCTION
    };

    enum Resistor
    {
        RESISTOR_NONE = 0,
        RESISTOR_PULL_UP,
        RESISTOR_PULL_DOWN
    };
    
    //--------------------------------------------------------------------------
    // Public typedefs
    //--------------------------------------------------------------------------

    typedef ErrorTemplate<ErrorCode> Error;

    //--------------------------------------------------------------------------
    // Public structures
    //--------------------------------------------------------------------------
    
    struct Id
    {
        Plat4m::Id portId;
        Plat4m::Id pinId;
    };

    struct Config
    {
        Mode mode;
        Resistor resistor;
    };
    
    //--------------------------------------------------------------------------
    // Public pure virtual methods
    //--------------------------------------------------------------------------

    virtual void setLevelFast(const Level level) = 0;

    virtual Level getLevelFast() = 0;

    virtual Level readLevelFast() = 0;

    virtual void toggleLevelFast() = 0;

    //--------------------------------------------------------------------------
    // Public virtual methods
    //--------------------------------------------------------------------------
    
    virtual Error configure(const Config& config);

    virtual Error setLevel(const Level level);

    virtual Error getLevel(Level& level);

    virtual Error readLevel(Level& level);

    virtual Error toggleLevel();

protected:

    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------
    
    GpioPin();
    
    //--------------------------------------------------------------------------
    // Protected virtual destructors
    //--------------------------------------------------------------------------

    virtual ~GpioPin();

    //--------------------------------------------------------------------------
    // Protected methods
    //--------------------------------------------------------------------------/
         
    Config& getConfig();

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    Config myConfig;

    //--------------------------------------------------------------------------
    // Private pure virtual methods
    //--------------------------------------------------------------------------
    
    virtual Error driverConfigure(const Config& config) = 0;

    virtual Error driverSetLevel(const Level level) = 0;

    virtual Error driverGetLevel(Level& level) = 0;

    virtual Error driverReadLevel(Level& level) = 0;

    virtual Error driverToggleLevel() = 0;
};

}; // namespace Plat4m

#endif // PLAT4M_GPIO_PIN_H
