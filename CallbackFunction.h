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
 * @file CallbackFunction.h
 * @author Ben Minerd
 * @date 8/13/2013
 * @brief CallbackFunction class.
 */

#ifndef _CALLBACK_FUNCTION_H_
#define _CALLBACK_FUNCTION_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <Callback.h>

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

template <typename TReturn, typename... TParameters>
class CallbackFunction : public Callback<TReturn, TParameters...>
{
public:
    
    /*--------------------------------------------------------------------------
     * Public typedefs
     *------------------------------------------------------------------------*/
    
    typedef TReturn (*CallbackFunctionType)(TParameters...);
    
    /*--------------------------------------------------------------------------
     * Public constructors and destructors
     *------------------------------------------------------------------------*/
    
    CallbackFunction(CallbackFunctionType callbackFunction) :
        Callback<TReturn, TParameters...>(),
        myCallbackFunction(callbackFunction)
    {
    }
    
    /*--------------------------------------------------------------------------
     * Public implemented methods
     *------------------------------------------------------------------------*/
    
    TReturn call(TParameters... parameters)
    {
        return (*myCallbackFunction)(parameters...);
    }
    
    TReturn operator()(TParameters... parameters)
    {
        return (*myCallbackFunction)(parameters...);
    }

private:
    
    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/
    
    CallbackFunctionType myCallbackFunction;
};

#endif // _CALLBACK_FUNCTION_H_
