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
// Copyright (c) 2017 Benjamin Minerd
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
/// @file GpioPort.h
/// @author Ben Minerd
/// @date 3/25/13
/// @brief GpioPort class.
///

#ifndef PLAT4M_GPIO_PORT_H
#define PLAT4M_GPIO_PORT_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Module.h>
#include <ErrorTemplate.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

template <typename TValue>
class GpioPort : public Module
{
public:
    
    //--------------------------------------------------------------------------
    // Public enumerations
    //--------------------------------------------------------------------------

    enum ErrorCode
    {
        ERROR_CODE_NONE,
        ERROR_CODE_NOT_ENABLED
    };

    enum Mode
    {
        MODE_DIGITAL_OUTPUT_PUSH_PULL = 0,
        MODE_DIGITAL_OUTPUT_OPEN_DRAIN,
        MODE_DIGITAL_INPUT
    };

    enum Resistor
    {
        RESISTOR_NONE = 0,
        RESISTOR_PULL_UP,
        RESISTOR_PULL_DOWN
    };
    
    typedef ErrorTemplate<ErrorCode> Error;

    //--------------------------------------------------------------------------
    // Public structures
    //--------------------------------------------------------------------------

    struct Config
    {
        Mode mode;
        Resistor resistor;
    };

    //--------------------------------------------------------------------------
    // Public pure virtual methods
    //--------------------------------------------------------------------------

    virtual void setValueFast(const TValue value) = 0;
    
    virtual TValue getValueFast() = 0;

    virtual TValue readValueFast() = 0;

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------
    
    //--------------------------------------------------------------------------
    Error configure(const Config& config)
    {
        Error error = driverConfigure(config);

        if (error.getCode() == ERROR_CODE_NONE)
        {
            myConfig = config;
        }

        return error;
    }
    
    //--------------------------------------------------------------------------
    Error setValue(const TValue value)
    {
        if (!isEnabled())
        {
            return Error(ERROR_CODE_NOT_ENABLED);
        }

        Error error = driverSetValue(value);

        return error;
    }
    
    //--------------------------------------------------------------------------
    Error getValue(TValue& value)
    {
        if (!isEnabled())
        {
            return Error(ERROR_CODE_NOT_ENABLED);
        }

        Error error = driverGetValue(value);

        return error;
    }
    
    //--------------------------------------------------------------------------
    Error readValue(TValue& value)
    {
        if (!isEnabled())
        {
            return Error(ERROR_CODE_NOT_ENABLED);
        }

        Error error = driverReadValue(value);

        return error;
    }
    
protected:
    
    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------
    
    //--------------------------------------------------------------------------
    GpioPort() :
        myConfig()
    {
    }
    
    //--------------------------------------------------------------------------
    // Protected virtual destructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual ~GpioPort()
    {
    }

private:
    
    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------
    
    Config myConfig;
    
    //--------------------------------------------------------------------------
    // Private pure virtual methods
    //--------------------------------------------------------------------------
    
    virtual Error driverConfigure(const Config& config) = 0;
    
    virtual Error driverSetValue(const TValue value) = 0;
    
    virtual Error driverGetValue(TValue& value) = 0;
    
    virtual Error driverReadValue(TValue& value) = 0;
};

}; // namespace Plat4m

#endif // PLAT4M_GPIO_PORT_H
