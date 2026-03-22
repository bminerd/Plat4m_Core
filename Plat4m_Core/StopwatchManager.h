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
// Copyright (c) 2022-2024 Benjamin Minerd
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
/// @file StopwatchManager.h
/// @author Ben Minerd
/// @date 2/28/2022
/// @brief StopwatchManager class header file.
///

#ifndef PLAT4M_STOPWATCH_MANAGER_H
#define PLAT4M_STOPWATCH_MANAGER_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Stopwatch.h>
#include <Plat4m_Core/ErrorTemplate.h>
#include <Plat4m_Core/List.h>
#include <Plat4m_Core/TimeStamp.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class StopwatchManager
{
public:

    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------

    enum ErrorCode
    {
        ERROR_CODE_NONE = 0,
        ERROR_CODE_INSTANCE_NOT_CREATED,
        ERROR_CODE_INSTANCE_ALREADY_CREATED
    };

    typedef ErrorTemplate<ErrorCode> Error;

    //--------------------------------------------------------------------------
    // Public static methods
    //--------------------------------------------------------------------------

    static Stopwatch& createStopwatch(const char* name = 0);

    static void addStopwatch(Stopwatch& stopwatch);

    static void removeStopwatch(Stopwatch& stopwatch);

    static List<Stopwatch*>& getStopwatchList();

    static void computeBackgroundCalculations();

    static TimeStamp getCurrentTimeStamp();

protected:

    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------

    StopwatchManager(const TimeStamp& cpuLoadTimeWindow);

    //--------------------------------------------------------------------------
    // Protected virtual destructors
    //--------------------------------------------------------------------------

    virtual ~StopwatchManager();

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    static StopwatchManager* myDriver;

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    List<Stopwatch*> myStopwatchList;

    Stopwatch* myBackgroundStopwatch;

    TimeStamp myCpuLoadTimeWindow;

    TimeStamp myCpuLoadTimeWindowStartTimeStamp;

    TimeStamp myCpuLoadTimeWindowEndTimeStamp;

    bool myIsMeasuringCpuLoadTimeWindow;

    bool myIsFirstBackgroundMeasurement;

    //--------------------------------------------------------------------------
    // Private pure virtual methods
    //--------------------------------------------------------------------------

    virtual Stopwatch& subclassCreateStopwatch(const char* name) = 0;

    virtual TimeStamp subclassGetCurrentTimeStamp() = 0;

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------

    void addStopwatchPrivate(Stopwatch& stopwatch);

    void removeStopwatchPrivate(Stopwatch& stopwatch);

    List<Stopwatch*>& getStopwatchListPrivate();

    void computeBackgroundCalculationsPrivate();
};

}; // namespace Plat4m

#endif // PLAT4M_STOPWATCH_MANAGER_H
