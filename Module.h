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
/// @file Module.h
/// @author Ben Minerd
/// @date 4/18/2013
/// @brief Module class header file.
///

#ifndef PLAT4M_MODULE_H
#define PLAT4M_MODULE_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/ErrorTemplate.h>
#include <Plat4m_Core/Callback.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class Module
{
public:
    
    //--------------------------------------------------------------------------
    // Public enumerations
    //--------------------------------------------------------------------------

	enum ErrorCode
	{
		ERROR_CODE_NONE = 0,
		ERROR_CODE_ENABLE_FAILED
	};

    //--------------------------------------------------------------------------
    // Public typedefs
    //--------------------------------------------------------------------------

	typedef ErrorTemplate<ErrorCode> Error;

	typedef Callback<Error, bool> SetEnabledCallback;

    //--------------------------------------------------------------------------
    // Public typedefs (deprecated)
    //--------------------------------------------------------------------------

	typedef SetEnabledCallback EnableCallback;

    //--------------------------------------------------------------------------
    // Public virtual methods
    //--------------------------------------------------------------------------

	virtual void setSetEnabledCallback(SetEnabledCallback& setEnabledCallback);

    virtual Error setEnabled(const bool enabled);

    virtual Error enable();

    virtual Error disable();

    virtual bool isEnabled();

    //--------------------------------------------------------------------------
    // Public virtual methods (deprecated)
    //--------------------------------------------------------------------------

    virtual void setEnableCallback(EnableCallback& enableCallback);

    virtual Error enable(const bool enable);

protected:

    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------

	Module();

    //--------------------------------------------------------------------------
    // Protected virtual destructors
    //--------------------------------------------------------------------------

    virtual ~Module();
    
private:
    
    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

	bool myIsEnabled;

	SetEnabledCallback* mySetEnabledCallback;

    //--------------------------------------------------------------------------
    // Private virtual methods
    //--------------------------------------------------------------------------

	virtual Error interfaceSetEnabled(const bool enabled);

	virtual Error driverSetEnabled(const bool enabled);

    //--------------------------------------------------------------------------
    // Private virtual methods (deprecated)
    //--------------------------------------------------------------------------

	virtual Error interfaceEnable(const bool enable);

	virtual Error driverEnable(const bool enable);
};

}; // namespace Plat4m

#endif // PLAT4M_MODULE_H
