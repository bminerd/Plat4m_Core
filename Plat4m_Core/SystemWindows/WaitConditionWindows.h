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
// Copyright (c) 2018-2024 Benjamin Minerd
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
/// @file WaitConditionWindows.h
/// @author Ben Minerd
/// @date 2/19/2018
/// @brief WaitConditionWindows class header file.
///

#ifndef PLAT4M_WAIT_CONDITION_WINDOWS_H
#define PLAT4M_WAIT_CONDITION_WINDOWS_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Windows.h>

#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/WaitCondition.h>
#include <Plat4m_Core/Thread.h>
#include <Plat4m_Core/ErrorTemplate.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class WaitConditionWindows : public WaitCondition
{
public:

    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------

    enum ErrorCode
    {
        ERROR_CODE_NONE = 0,
        ERROR_CODE_CREATION_FAILED
    };

    using Error = ErrorTemplate<ErrorCode>;

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    WaitConditionWindows(Thread& thread);

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~WaitConditionWindows();

    //--------------------------------------------------------------------------
    // Public virtual methods overridden for WaitCondition
    //--------------------------------------------------------------------------

    virtual void notifyFast() override;

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    DWORD myThreadId;

    //--------------------------------------------------------------------------
    // Private virtual methods overridden for WaitCondition
    //--------------------------------------------------------------------------

    virtual WaitCondition::Error driverWait(const TimeMs waitTimeMs) override;

    virtual WaitCondition::Error driverNotify() override;
};

}; // namespace Plat4m

#endif // PLAT4M_WAIT_CONDITION_WINDOWS_H
