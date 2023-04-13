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
// Copyright (c) 2016-2023 Benjamin Minerd
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
/// @file ThreadLite.h
/// @author Ben Minerd
/// @date 7/1/2016
/// @brief ThreadLite class header file.
///

#ifndef PLAT4M_THREAD_LITE_H
#define PLAT4M_THREAD_LITE_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

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

class ThreadLite : public Thread
{
public:

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    ThreadLite(RunCallback& callback,
               const TimeMs periodMs = 0,
               const char* name = 0);

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~ThreadLite();

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    bool isActive() const;

    void setActive(const bool isActive);

    TimeMs getCallTimeMs() const;

    void setCallTimeMs(const TimeMs callTimeMs);

    TimeMs getWorstCaseRunTimeMs() const;

    void setWorstCaseRunTimeMs(const TimeMs worstCaseRunTimeMs);

    bool isCallTime() const;

    bool canRunInTimeWindow(const TimeMs timeWindowMs);

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    bool myIsActive;
    TimeMs myCallTimeMs;
    TimeMs myWorstCaseRunTimeMs;

    //--------------------------------------------------------------------------
    // Private virtual methods overridden for Thread
    //--------------------------------------------------------------------------

    virtual void driverSetPeriodMs(const TimeMs periodMs) override;

    virtual uint32_t driverSetPriority(const uint32_t priority) override;
};

}; // namespace Plat4m

#endif // PLAT4M_THREAD_LITE_H
