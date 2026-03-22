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
// Copyright (c) 2013-2024 Benjamin Minerd
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
/// @file Module.cpp
/// @author Ben Minerd
/// @date 4/18/2013
/// @brief Generic class for all modules to be subclasses of.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Module.h>
#include <Plat4m_Core/ErrorTemplate.h>

using namespace Plat4m;

//------------------------------------------------------------------------------
// Public virtual methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void Module::setSetEnabledCallback(SetEnabledCallback& setEnabledCallback)
{
    mySetEnabledCallback = &setEnabledCallback;
}

//------------------------------------------------------------------------------
Module::Error Module::setEnabled(const bool enabled)
{
    if (myIsEnabled == enabled)
    {
        return Error(ERROR_CODE_NONE);
    }

    bool lastIsEnabled = myIsEnabled;

    Error error;

    error = subclassSetEnabled(enabled);

    if (error.getCode() != ERROR_CODE_NONE)
    {
        return error;
    }

    myIsEnabled = enabled;

    if (isValidPointer(mySetEnabledCallback))
    {
        error = mySetEnabledCallback->call(enabled);

        if (error.getCode() != ERROR_CODE_NONE)
        {
            myIsEnabled = lastIsEnabled;
        }
    }
    else
    {
        error = Error(ERROR_CODE_NONE);
    }

    return error;
}

//------------------------------------------------------------------------------
Module::Error Module::enable()
{
    return setEnabled(true);
}

//------------------------------------------------------------------------------
Module::Error Module::disable()
{
    return setEnabled(false);
}

//------------------------------------------------------------------------------
bool Module::isEnabled()
{
    return myIsEnabled;
}

//------------------------------------------------------------------------------
// Public virtual methods (deprecated)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void Module::setEnableCallback(EnableCallback& enableCallback)
{
    setSetEnabledCallback(enableCallback);
}

//------------------------------------------------------------------------------
Module::Error Module::enable(const bool enable)
{
    return setEnabled(enable);
}

//------------------------------------------------------------------------------
// Protected constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Module() :
    myIsEnabled(false),
    mySetEnabledCallback(0)
{
}

//------------------------------------------------------------------------------
// Protected virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::~Module()
{
}

//------------------------------------------------------------------------------
// Protected virtual methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error Module::subclassSetEnabled(const bool enabled)
{
    // Call deprecated methods in case those were implemented by subclass

    Error error = driverSetEnabled(enabled);

    if (error.getCode() != ERROR_CODE_NONE)
    {
        return error;
    }

    return (interfaceSetEnabled(enabled));
}

//------------------------------------------------------------------------------
// Private virtual methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error Module::interfaceSetEnabled(const bool enabled)
{
    // Call deprecated method in case it was implemented by subclass
    return (interfaceEnable(enabled));
}

//------------------------------------------------------------------------------
Module::Error Module::driverSetEnabled(const bool enabled)
{
    // Call deprecated method in case it was implemented by subclass
    return (driverEnable(enabled));
}

//------------------------------------------------------------------------------
Module::Error Module::interfaceEnable(const bool enable)
{
    // Intentionally blank, not implemented by subclass
    return (Error(ERROR_CODE_NONE));
}

//------------------------------------------------------------------------------
Module::Error Module::driverEnable(const bool enable)
{
    // Intentionally blank, not implemented by subclass
    return (Error(ERROR_CODE_NONE));
}
