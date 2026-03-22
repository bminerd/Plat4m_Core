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
// Copyright (c) 2016-2024 Benjamin Minerd
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
/// @file WaitCondition.h
/// @author Ben Minerd
/// @date 11/28/2016
/// @brief WaitCondition class header file.
///

#ifndef PLAT4M_WAIT_CONDITION_H
#define PLAT4M_WAIT_CONDITION_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/ErrorTemplate.h>
#include <Plat4m_Core/Thread.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class WaitCondition
{
public:

    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------

    enum ErrorCode
    {
        ERROR_CODE_NONE = 0,
        ERROR_CODE_WAIT_FAILED,
        ERROR_CODE_NOTIFY_FAILED,
        ERROR_CODE_INSTANTIATION_FAILED
    };

    using Error = ErrorTemplate<ErrorCode>;

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~WaitCondition();

    //--------------------------------------------------------------------------
    // Public pure virtual methods
    //--------------------------------------------------------------------------

    virtual void notifyFast() = 0;

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    Error wait(const TimeMs waitTimeMs = 0xFFFFFFFF);

    Error notify();

protected:

    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------

    WaitCondition(Thread& thread);

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    Thread& myThread;

    //--------------------------------------------------------------------------
    // Private pure virtual methods
    //--------------------------------------------------------------------------

    virtual Error driverWait(const TimeMs waitTimeMs) = 0;

    virtual Error driverNotify() = 0;
};

}; // namespace Plat4m

#endif // PLAT4M_WAIT_CONDITION_H
