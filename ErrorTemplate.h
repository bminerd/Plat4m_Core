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
// Copyright (c) 2015 Benjamin Minerd
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
/// @file ErrorTemplate.h
/// @author Ben Minerd
/// @date 5/25/15
/// @brief ErrorTemplate header file.
///

#ifndef PLAT4M_ERROR_TEMPLATE_H
#define PLAT4M_ERROR_TEMPLATE_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Plat4m.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

template <typename TCode>
class ErrorTemplate
{
public:
    
    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	ErrorTemplate()
	{
	}

    //--------------------------------------------------------------------------
    explicit ErrorTemplate(const TCode code) :
        myCode()
    {
        setCode(code);
    }

	//--------------------------------------------------------------------------
	ErrorTemplate(const ErrorTemplate<TCode>& errorTemplate) :
	    myCode(errorTemplate.myCode)
	{
	    // Don't log any errors
	}

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------
    
	//--------------------------------------------------------------------------
    ErrorTemplate& operator=(const ErrorTemplate<TCode>& errorTemplate)
    {
        myCode = errorTemplate.myCode;

        return (*this);
    }

    //--------------------------------------------------------------------------
    bool operator==(const TCode code)
    {
        return (myCode == code);
    }

    //--------------------------------------------------------------------------
    TCode getCode()
    {
        return myCode;
    }

	//--------------------------------------------------------------------------
	void setCode(const TCode code)
	{
		myCode = code;

		// Psuedocode...
		//
		// if (myCode != 0)
		// {
		//     ErrorHandler::call(*this);
		// }
	}
    
private:
    
    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

	TCode myCode;
};

}; // namespace Plat4m

#endif // PLAT4M_ERROR_TEMPLATE_H
