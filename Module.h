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
 * Copyright (c) 2015 Benjamin Minerd
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
 * @file Module.h
 * @author Ben Minerd
 * @date 4/18/13
 * @brief Generic class for all modules to be subclasses of.
 */

#ifndef MODULE_H
#define MODULE_H

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <ErrorTemplate.h>
#include <Callback.h>

namespace Plat4m
{

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

class Module
{
public:
    
	/*--------------------------------------------------------------------------
	 * Public enumerations
	 *------------------------------------------------------------------------*/

	enum ErrorCode
	{
		ERROR_CODE_NONE = 0,
		ERROR_CODE_ENABLE
	};

	/*--------------------------------------------------------------------------
	 * Public typedefs
	 *------------------------------------------------------------------------*/

	typedef ErrorTemplate<ErrorCode> Error;

	typedef Callback<Error, bool> EnableCallback;
    
    /*--------------------------------------------------------------------------
	 * Public methods
	 *------------------------------------------------------------------------*/

    void setEnableCallback(EnableCallback& enableCallback);

    Error enable(const bool enable);

    bool isEnabled();

protected:
    
    /*--------------------------------------------------------------------------
	 * Protected constructors
	 *------------------------------------------------------------------------*/

	Module();

	/*--------------------------------------------------------------------------
     * Protected virtual destructors
     *------------------------------------------------------------------------*/

    virtual ~Module();
    
private:
    
    /*--------------------------------------------------------------------------
	 * Private members
	 *------------------------------------------------------------------------*/

	bool myIsEnabled;

	EnableCallback* myEnableCallback;

	/*--------------------------------------------------------------------------
	 * Private virtual methods
	 *------------------------------------------------------------------------*/

	virtual Error driverEnable(const bool enable);
};

}; // namespace Plat4m

#endif // MODULE_H
