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
 * @file PowerSupply.cpp
 * @author Ben Minerd
 * @date 4/19/13
 * @brief PowerSupply class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <PowerSupply.h>

using Plat4m::PowerSupply;
using Plat4m::Module;

/*------------------------------------------------------------------------------
 * Public constructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
PowerSupply::PowerSupply(const EnableLine::ActiveLevel activeLevel,
                         GpioPin& gpioPin,
                         PowerSupply* parentSupply) :
    myNUsers(0),
    myIsControlled(false),
    myEnableLine(EnableLine::MODE_OUTPUT, activeLevel, gpioPin, true),
    myParent(parentSupply)
{
}

/*------------------------------------------------------------------------------
 * Public destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
PowerSupply::~PowerSupply()
{
}

/*------------------------------------------------------------------------------
 * Public methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
PowerSupply::Error PowerSupply::setActive(const bool active)
{
    if (!myIsControlled)
    {
        return Error(ERROR_CODE_NOT_CONTROLLED);
    }
    
    EnableLine::Error error = myEnableLine.setActive(active);

    if (error.getCode() != EnableLine::ERROR_CODE_NONE)
    {
        // Error
    }
    
    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
PowerSupply::Error PowerSupply::isActive(bool& isActive)
{
    EnableLine::Error error = myEnableLine.isActive(isActive);

    if (error.getCode() != EnableLine::ERROR_CODE_NONE)
    {
        // Error
    }
    
    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
PowerSupply::Error PowerSupply::use(const bool use)
{
    // Fix same user problem
    
    if (use)
    {
        myNUsers++;
    }
    else
    {
        if (myNUsers > 0)
        {
            myNUsers--;
        }
    }
    
    if (isValidPointer(myParent))
    {
        myParent->use(use);
    }
    
    return update();
}

//------------------------------------------------------------------------------
PowerSupply::Error PowerSupply::control(const bool control)
{
    if (control == myIsControlled)
    {
        return Error(ERROR_CODE_NONE);
    }
    
    myIsControlled = control;
    
    return update();
}

//------------------------------------------------------------------------------
PowerSupply::Error PowerSupply::isControlled(bool& isControlled)
{
    isControlled = myIsControlled;
    
    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
PowerSupply::Error PowerSupply::getParent(PowerSupply*& parentSupply)
{
    parentSupply = myParent;
    
    return Error(ERROR_CODE_NONE);
}

/*------------------------------------------------------------------------------
 * Private methods implemented from Module
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Module::Error PowerSupply::driverEnable(const bool enable)
{
    Module::Error error = myEnableLine.enable(enable);

    if (error.getCode() != Module::ERROR_CODE_NONE)
    {
        // Error
    }

    return Module::Error(Module::ERROR_CODE_NONE);
}

/*------------------------------------------------------------------------------
 * Private methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
PowerSupply::Error PowerSupply::update()
{
    if (!myIsControlled)
    {
        bool active = (myNUsers != 0);
        
        EnableLine::Error error = myEnableLine.setActive(active);

        if (error.getCode() != EnableLine::ERROR_CODE_NONE)
        {
            // Error
        }
    }
    
    return Error(ERROR_CODE_NONE);
}
