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
// Copyright (c) 2017 Benjamin Minerd
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
/// @file Interrupt.cpp
/// @author Ben Minerd
/// @date 3/14/2016
/// @brief Interrupt class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Interrupt.h>
#include <System.h>

using Plat4m::Interrupt;
using Plat4m::Module;

//------------------------------------------------------------------------------
// Public virtual methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Interrupt::HandlerCallback* Interrupt::getHandlerCallback()
{
    return myHandlerCallback;
}

//------------------------------------------------------------------------------
void Interrupt::setHandlerCallback(HandlerCallback& handlerCallback)
{
    myHandlerCallback = &handlerCallback;
}

//------------------------------------------------------------------------------
Interrupt::Error Interrupt::configure(const Config& config)
{
    Error error = driverConfigure(config);

    return error;
}

//------------------------------------------------------------------------------
void Interrupt::setPolicy(InterruptPolicy* interruptPolicy)
{
    myInterruptPolicy = interruptPolicy;
}

//------------------------------------------------------------------------------
// Protected constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Interrupt::Interrupt(HandlerCallback& handlerCallback) :
    Module(),
    myHandlerCallback(&handlerCallback),
    myInterruptPolicy(InterruptPolicy::get())
{
}

//------------------------------------------------------------------------------
// Protected virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Interrupt::~Interrupt()
{
}

//------------------------------------------------------------------------------
// Private virtual methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error Interrupt::interfaceEnable(const bool enable)
{
    return Module::Error(Module::ERROR_CODE_NONE);
}
