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
// Copyright (c) 2013-2023 Benjamin Minerd
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
/// @file CallbackMethod.h
/// @author Ben Minerd
/// @date 7/8/2013
/// @brief CallbackMethod class header file.
///

#ifndef PLAT4M_CALLBACK_METHOD_H
#define PLAT4M_CALLBACK_METHOD_H

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

template <class TClass, typename TReturn, typename... TParameters>
class CallbackMethod : public Callback<TReturn, TParameters...>
{
public:

    //--------------------------------------------------------------------------
    // Public typedefs
    //--------------------------------------------------------------------------

    typedef TReturn (TClass::*CallbackMethodType)(TParameters...);

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    CallbackMethod(TClass* object, CallbackMethodType callbackMethod) :
        Callback<TReturn, TParameters...>(),
        myObject(object),
        myCallbackMethod(callbackMethod)
    {
    }

    //--------------------------------------------------------------------------
    // Public virtual methods overridden for Callback
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual inline TReturn call(TParameters... parameters) override
    {
        return (*myObject.*myCallbackMethod)(parameters...);
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
template <class TClass, typename TReturn, typename... TParameters>
Callback<TReturn, TParameters...>& createCallback(TClass* object,
                                  TReturn (TClass::*callback)(TParameters...))
{
    return *(MemoryAllocator::allocate<
                    CallbackMethod<TClass, TReturn, TParameters...>>(object,
                                                                     callback));
}

}; // namespace Plat4m

#endif // PLAT4M_CALLBACK_METHOD_H
