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
// Copyright (c) 2016 Benjamin Minerd
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
/// @file ExternalInterrupt.h
/// @author Ben Minerd
/// @date 9/25/15
/// @brief ExternalInterrupt class.
///

#ifndef EXTERNAL_INTERRUPT_H
#define EXTERNAL_INTERRUPT_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m.h>
#include <Module.h>
#include <Callback.h>
#include <GpioPin.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class ExternalInterrupt : public Module
{
public:
    
    //--------------------------------------------------------------------------
    // Public enumerations
    //--------------------------------------------------------------------------
    
    /**
     * @brief Enumeration of external interrupt errors.
     */
    enum ErrorCode
    {
        ERROR_CODE_NONE,
        ERROR_CODE_PARAMETER_INVALID,
        ERROR_CODE_NOT_ENABLED
    };

    /**
     * @brief Enumeration of external interrupt triggers.
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
    
    //--------------------------------------------------------------------------
    // Public typedefs
    //--------------------------------------------------------------------------

    typedef ErrorTemplate<ErrorCode> Error;

    typedef Callback<void, bool> HandlerCallback;

    //--------------------------------------------------------------------------
    // Public structures
    //--------------------------------------------------------------------------

    struct Config
    {
        Trigger trigger;
        ActiveLevel activeLevel;
    };
    
    //--------------------------------------------------------------------------
    // Public virtual methods
    //--------------------------------------------------------------------------

    /**
     * @brief Configures the given external interrupt.
     * @param handle ExternalInterrupt handle to access.
     * @param config ExternalInterrupt configuration.
     * @return ExternalInterrupt error.
     */
    virtual Error configure(const Config& config);
    
    virtual void setHandlerCallback(HandlerCallback& handlerCallback);
    
    virtual Error isActive(bool& isActive);
    
protected:
    
    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------
    
    ExternalInterrupt(GpioPin& gpioPin);
    
    //--------------------------------------------------------------------------
    // Protected virtual destructors
    //--------------------------------------------------------------------------

    virtual ~ExternalInterrupt();

    //--------------------------------------------------------------------------
    // Protected methods
    //--------------------------------------------------------------------------

    void interruptHandler();

private:
    
    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------
    
    Config myConfig;
    
    GpioPin& myGpioPin;
    
    HandlerCallback* myHandlerCallback;

    //--------------------------------------------------------------------------
    // Private pure virtual methods
    //--------------------------------------------------------------------------
    
    virtual Error driverConfigure(const Config& config) = 0;

    //--------------------------------------------------------------------------
    // Private virtual methods implemented from Module
    //--------------------------------------------------------------------------

    virtual Module::Error interfaceEnable(const bool enable);
};

}; // namespace Plat4m

#endif // EXTERNAL_INTERRUPT_H
