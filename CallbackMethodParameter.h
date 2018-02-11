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
/// @file CallbackMethodParameter.h
/// @author Ben Minerd
/// @date 7/8/2013
/// @brief CallbackMethodParameter class header file.
///

#ifndef PLAT4M_CALLBACK_METHOD_PARAMETER_H
#define PLAT4M_CALLBACK_METHOD_PARAMETER_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Callback.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

template <class TClass, typename TReturn, typename TParameter>
class CallbackMethodParameter : public Callback<TReturn, TParameter>
{
public:
    
    //--------------------------------------------------------------------------
    // Public typedefs
    //--------------------------------------------------------------------------
    
    typedef TReturn (TClass::*CallbackMethodType)(TParameter);
    
    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------
    
    //--------------------------------------------------------------------------
    CallbackMethodParameter(TClass* object,
                             CallbackMethodType callbackMethod) :
        Callback<TReturn, TParameter>(),
        myObject(object),
        myCallbackMethod(callbackMethod)
    {
    }
    
    //--------------------------------------------------------------------------
    // Public methods implemented from Callback
    //--------------------------------------------------------------------------
    
    //--------------------------------------------------------------------------
    inline TReturn call(TParameter parameter, void* dummyParameter2 = 0)
    {
        return (*myObject.*myCallbackMethod)(parameter);
    }
    
private:
    
    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/
    
    TClass* myObject;
    CallbackMethodType myCallbackMethod;
};

    //--------------------------------------------------------------------------
    template <class TClass, typename TReturn, typename TParameter>
    Callback<TReturn, TParameter>& createCallback(
                                        TClass* object,
                                        TReturn (TClass::*callback)(TParameter))
    {
        return *(new CallbackMethodParameter<TClass, TReturn, TParameter>(
                                                                     object,
                                                                     callback));
    }

}; // namespace Plat4m

#endif // PLAT4M_CALLBACK_METHOD_PARAMETER_H
