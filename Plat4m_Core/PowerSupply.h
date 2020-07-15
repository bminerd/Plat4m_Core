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
/// @file PowerSupply.h
/// @author Ben Minerd
/// @date 4/19/2013
/// @brief PowerSupply class header file.
///

#ifndef PLAT4M_POWER_SUPPLY_H
#define PLAT4M_POWER_SUPPLY_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/Module.h>
#include <Plat4m_Core/ErrorTemplate.h>
#include <Plat4m_Core/EnableLine.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class PowerSupply : public Module
{
public:
    
    //--------------------------------------------------------------------------
    // Public enumerations
    //--------------------------------------------------------------------------
    
    enum ErrorCode
    {
        ERROR_CODE_NONE,
        ERROR_CODE_NOT_ENABLED,
        ERROR_CODE_COMMUNICATION,
        ERROR_CODE_NOT_CONTROLLED
    };
    
    //--------------------------------------------------------------------------
    // Public typedefs
    //--------------------------------------------------------------------------

    typedef ErrorTemplate<ErrorCode> Error;

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------
    
    PowerSupply(const EnableLine::ActiveLevel activeLevel,
                GpioPin& gpioPin,
                PowerSupply* parentSupply = 0);
    
    //--------------------------------------------------------------------------
    // Public destructors
    //--------------------------------------------------------------------------
    
    ~PowerSupply();
    
    //--------------------------------------------------------------------------
    // Public virtual methods
    //--------------------------------------------------------------------------
    
    virtual Error setActive(const bool active);
    
    virtual Error isActive(bool& isActive);
    
    virtual Error use(const bool use);
    
    virtual Error control(const bool control);
    
    virtual Error isControlled(bool& isControlled);
    
    virtual Error getParent(PowerSupply*& parentSupply);
    
private:
    
    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------
    
    unsigned int myNUsers;
    
    bool myIsControlled;
    
    EnableLine myEnableLine;
    
    PowerSupply* myParent;

    //--------------------------------------------------------------------------
    // Private virtual methods implemented from Module
    //--------------------------------------------------------------------------

    virtual Module::Error driverEnable(const bool enable);
    
    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------
    
    Error update();
};

}; // namespace Plat4m

#endif // PLAT4M_POWER_SUPPLY_H
