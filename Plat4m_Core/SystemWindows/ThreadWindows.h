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
// Copyright (c) 2018-2023 Benjamin Minerd
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
/// @file ThreadWindows.h
/// @author Ben Minerd
/// @date 2/19/2018
/// @brief ThreadWindows class header file.
///

#ifndef PLAT4M_THREAD_WINDOWS_H
#define PLAT4M_THREAD_WINDOWS_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <windows.h>

#include <Plat4m_Core/Thread.h>
#include <Plat4m_Core/Plat4m.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class ThreadWindows : public Thread
{
public:

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    ThreadWindows(RunCallback& callback,
                  const TimeMs periodMs = 0,
                  const char* name = 0);

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~ThreadWindows();

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    DWORD getThreadId() const;

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    HANDLE myThreadHandle;

    DWORD myThreadId;

    //--------------------------------------------------------------------------
    // Private static methods
    //--------------------------------------------------------------------------

    static DWORD WINAPI threadCallback(LPVOID lpParameter);

    //--------------------------------------------------------------------------
    // Private virtual methods overridden for Module
    //--------------------------------------------------------------------------

    virtual Module::Error driverSetEnabled(const bool enabled) override;

    //--------------------------------------------------------------------------
    // Private virtual methods overridden for Thread
    //--------------------------------------------------------------------------

    virtual void driverSetPeriodMs(const TimeMs periodMs) override;

    virtual uint32_t driverSetPriority(const uint32_t priority) override;
};

}; // namespace Plat4m

#endif // PLAT4M_THREAD_WINDOWS_H
