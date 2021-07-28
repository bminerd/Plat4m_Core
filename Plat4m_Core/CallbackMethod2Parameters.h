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
// Copyright (c) 2021 Benjamin Minerd
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
/// @file CallbackMethod2Parameters.h
/// @author Ben Minerd
/// @date 7/8/2013
/// @brief CallbackMethod2Parameters class header file.
///

#ifndef PLAT4M_CALLBACK_METHOD_2_PARAMETERS_H
#define PLAT4M_CALLBACK_METHOD_2_PARAMETERS_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Callback.h>
#include <Plat4m_Core/MemoryAllocator.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

template <class TClass,
          typename TReturn,
          typename TParameter1,
          typename TParameter2>
class CallbackMethod2Parameters : public Callback<TReturn,
                                                  TParameter1,
                                                  TParameter2>
{
public:
    
    //--------------------------------------------------------------------------
    // Public typedefs
    //--------------------------------------------------------------------------
    
    typedef TReturn (TClass::*CallbackMethodType)(TParameter1, TParameter2);
    
    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------
    
    //--------------------------------------------------------------------------
    CallbackMethod2Parameters(TClass* object,
                              CallbackMethodType callbackMethod) :
        Callback<TReturn, TParameter1, TParameter2>(),
        myObject(object),
        myCallbackMethod(callbackMethod)
    {
    }
    
    //--------------------------------------------------------------------------
    // Public methods implemented from Callback
    //--------------------------------------------------------------------------
    
    //--------------------------------------------------------------------------
    inline TReturn call(TParameter1 parameter1, TParameter2 parameter2)
    {
        return (*myObject.*myCallbackMethod)(parameter1, parameter2);
    }
    
private:
    
    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------
    
    TClass* myObject;
    CallbackMethodType myCallbackMethod;
};

//------------------------------------------------------------------------------
// Namespace functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template <class TClass,
          typename TReturn,
          typename TParameter1,
          typename TParameter2>
Callback<TReturn, TParameter1, TParameter2>& createCallback(
                          TClass* object,
                          TReturn (TClass::*callback)(TParameter1, TParameter2))
{
    return *(MemoryAllocator::allocate<
                   CallbackMethod2Parameters<TClass,
                                               TReturn,
                                               TParameter1,
                                               TParameter2>>(object, callback));
}

}; // namespace Plat4m

#endif // PLAT4M_CALLBACK_METHOD_2_PARAMETERS_H
