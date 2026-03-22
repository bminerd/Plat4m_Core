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
// Copyright (c) 2021-2024 Benjamin Minerd
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
/// @file Stopwatch.h
/// @author Ben Minerd
/// @date 9/13/2021
/// @brief Stopwatch class header file.
///

#ifndef PLAT4M_STOPWATCH_H
#define PLAT4M_STOPWATCH_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>

#include <Plat4m_Core/TimeStamp.h>
#include <Plat4m_Core/List.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class Stopwatch
{
public:

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~Stopwatch();

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    void start();

    void stop();

    void pause();

    void resume();

    void setName(const char* name);

    const char* getName() const;

    TimeStamp getStartTimeStamp() const;

    TimeStamp getStopTimeStamp() const;

    TimeStamp getCpuTimeStamp() const;

    TimeStamp getMinCpuTimeStamp() const;

    TimeStamp getMaxCpuTimeStamp() const;

    TimeStamp getElapsedTimeStamp() const;

    TimeStamp getMinElapsedTimeStamp() const;

    TimeStamp getMaxElapsedTimeStamp() const;

    TimeStamp getPeriodTimeStamp() const;

    TimeStamp getPreemptedTimeStamp() const;

    TimeStamp getCummulativeCpuTimeStamp() const;

    void resetCummulativeCpuTimeStamp();

    std::uint32_t getEventCount() const;

    void resetEventCount();

    std::uint64_t getTotalEventCount() const;

    float getCpuLoadPercentage() const;

    void setCpuLoadPercentage(const float percentage);

    float getFrequencyHz() const;

protected:

    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------

    Stopwatch(const char* name = 0);

    //--------------------------------------------------------------------------
    // Protected methods
    //--------------------------------------------------------------------------

    bool isFirstMeasurement() const;

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    static Stopwatch* myCurrentStopwatch;

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    const char* myName;

    Stopwatch* myPreemptedStopwatch;

    bool myIsFirstMeasurement;

    bool myIsPaused;

    TimeStamp myStartTimeStamp;

    TimeStamp myStopTimeStamp;

    TimeStamp myPauseStartTimeStamp;

    TimeStamp myPauseStopTimeStamp;

    TimeStamp myPausedTimeStamp;

    TimeStamp myCpuTimeStamp;

    TimeStamp myMinCpuTimeStamp;

    TimeStamp myMaxCpuTimeStamp;

    TimeStamp myElapsedTimeStamp;

    TimeStamp myMinElapsedTimeStamp;

    TimeStamp myMaxElapsedTimeStamp;

    TimeStamp myPreemptedTimeStamp;

    TimeStamp myCummulativeCpuTimeStamp;

    std::uint32_t myEventCount;

    std::uint64_t myTotalEventCount;

    float myCpuLoadPercentage;

    TimeStamp myPeriodTimeStamp;

    float myFrequencyHz;
};

}; // namespace Plat4m

#endif // PLAT4M_STOPWATCH_H
