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
// Copyright (c) 2021-2023 Benjamin Minerd
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
/// @file Stopwatch.cpp
/// @author Ben Minerd
/// @date 9/13/2021
/// @brief Generic class for all stopwatchs to be subclasses of.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Stopwatch.h>
#include <Plat4m_Core/StopwatchManager.h>
#include <Plat4m_Core/System.h>

using namespace Plat4m;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

Stopwatch* Stopwatch::myCurrentStopwatch = 0;

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Stopwatch::~Stopwatch()
{
    StopwatchManager::removeStopwatch(*this);
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void Stopwatch::start()
{
    TimeStamp previousStartTimeStamp = myStartTimeStamp;

    System::enterCriticalSection();

    myStartTimeStamp = StopwatchManager::getCurrentTimeStamp();

    if (isValidPointer(myCurrentStopwatch) && !(myCurrentStopwatch->myIsPaused))
    {
        myPreemptedStopwatch = myCurrentStopwatch;
    }

    myCurrentStopwatch = this;

    System::exitCriticalSection();

    // Reset the preemption time
    myPreemptedTimeStamp = TimeStamp();

    if (!myIsFirstMeasurement)
    {
        myPeriodTimeStamp = myStartTimeStamp - previousStartTimeStamp;
        myFrequencyHz = 1 / myPeriodTimeStamp.toTimeSFloat();
    }
}

//------------------------------------------------------------------------------
void Stopwatch::stop()
{
    System::enterCriticalSection();

    myStopTimeStamp = StopwatchManager::getCurrentTimeStamp();

    myElapsedTimeStamp = myStopTimeStamp - myStartTimeStamp;

    myElapsedTimeStamp -= myPausedTimeStamp;

    if (isValidPointer(myPreemptedStopwatch))
    {
        myPreemptedStopwatch->myPreemptedTimeStamp += myElapsedTimeStamp;
    }

    myCurrentStopwatch = myPreemptedStopwatch;

    myPreemptedStopwatch = 0;

    System::exitCriticalSection();

    // The calculations below rely on non-static data members and can be
    // interrupted

    myEventCount++;
    myTotalEventCount++;

    myCpuTimeStamp = myElapsedTimeStamp - myPreemptedTimeStamp;

    myCummulativeCpuTimeStamp += myCpuTimeStamp;

    findMinMax(myCpuTimeStamp, myMinCpuTimeStamp, myMaxCpuTimeStamp);

    findMinMax(myElapsedTimeStamp,
               myMinElapsedTimeStamp,
               myMaxElapsedTimeStamp);

    if (myIsFirstMeasurement)
    {
        myMinCpuTimeStamp = myCpuTimeStamp;
        myMinElapsedTimeStamp = myElapsedTimeStamp;

        myIsFirstMeasurement = false;
    }

    myPausedTimeStamp.reset();
}

//------------------------------------------------------------------------------
void Stopwatch::pause()
{
    System::enterCriticalSection();

    myPauseStartTimeStamp = StopwatchManager::getCurrentTimeStamp();

    myIsPaused = true;

    System::exitCriticalSection();
}

//------------------------------------------------------------------------------
void Stopwatch::resume()
{
    System::enterCriticalSection();

    myPauseStopTimeStamp = StopwatchManager::getCurrentTimeStamp();

    myIsPaused = false;

    System::exitCriticalSection();

    myPausedTimeStamp += myPauseStopTimeStamp - myPauseStartTimeStamp;
}

//------------------------------------------------------------------------------
void Stopwatch::setName(const char* name)
{
    myName = name;
}

//------------------------------------------------------------------------------
const char* Stopwatch::getName() const
{
    return myName;
}

//------------------------------------------------------------------------------
TimeStamp Stopwatch::getStartTimeStamp() const
{
    return myStartTimeStamp;
}

//------------------------------------------------------------------------------
TimeStamp Stopwatch::getStopTimeStamp() const
{
    return myStopTimeStamp;
}

//------------------------------------------------------------------------------
TimeStamp Stopwatch::getCpuTimeStamp() const
{
    return myCpuTimeStamp;
}

//------------------------------------------------------------------------------
TimeStamp Stopwatch::getMinCpuTimeStamp() const
{
    return myMinCpuTimeStamp;
}

//------------------------------------------------------------------------------
TimeStamp Stopwatch::getMaxCpuTimeStamp() const
{
    return myMaxCpuTimeStamp;
}

//------------------------------------------------------------------------------
TimeStamp Stopwatch::getElapsedTimeStamp() const
{
    return myElapsedTimeStamp;
}

//------------------------------------------------------------------------------
TimeStamp Stopwatch::getMinElapsedTimeStamp() const
{
    return myMinElapsedTimeStamp;
}

//------------------------------------------------------------------------------
TimeStamp Stopwatch::getMaxElapsedTimeStamp() const
{
    return myMaxElapsedTimeStamp;
}

//------------------------------------------------------------------------------
TimeStamp Stopwatch::getPeriodTimeStamp() const
{
    return myPeriodTimeStamp;
}

//------------------------------------------------------------------------------
TimeStamp Stopwatch::getPreemptedTimeStamp() const
{
    return myPreemptedTimeStamp;
}

//------------------------------------------------------------------------------
TimeStamp Stopwatch::getCummulativeCpuTimeStamp() const
{
    return myCummulativeCpuTimeStamp;
}

//------------------------------------------------------------------------------
void Stopwatch::resetCummulativeCpuTimeStamp()
{
    myCummulativeCpuTimeStamp.reset();
}

//------------------------------------------------------------------------------
std::uint32_t Stopwatch::getEventCount() const
{
    return myEventCount;
}

//------------------------------------------------------------------------------
void Stopwatch::resetEventCount()
{
    myEventCount = 0;
}

//------------------------------------------------------------------------------
std::uint64_t Stopwatch::getTotalEventCount() const
{
    return myTotalEventCount;
}

//------------------------------------------------------------------------------
float Stopwatch::getCpuLoadPercentage() const
{
    return myCpuLoadPercentage;
}

//------------------------------------------------------------------------------
void Stopwatch::setCpuLoadPercentage(const float percentage)
{
    myCpuLoadPercentage = percentage;
}

//------------------------------------------------------------------------------
float Stopwatch::getFrequencyHz() const
{
    return myFrequencyHz;
}

//------------------------------------------------------------------------------
// Protected constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Stopwatch::Stopwatch(const char* name) :
    myName(name),
    myPreemptedStopwatch(0),
    myIsFirstMeasurement(true),
    myIsPaused(false),
    myStartTimeStamp(),
    myStopTimeStamp(),
    myPauseStartTimeStamp(),
    myPauseStopTimeStamp(),
    myPausedTimeStamp(),
    myCpuTimeStamp(),
    myMinCpuTimeStamp(),
    myMaxCpuTimeStamp(),
    myElapsedTimeStamp(),
    myMinElapsedTimeStamp(),
    myMaxElapsedTimeStamp(),
    myPreemptedTimeStamp(),
    myCummulativeCpuTimeStamp(),
    myEventCount(0),
    myTotalEventCount(0),
    myCpuLoadPercentage(0),
    myPeriodTimeStamp(),
    myFrequencyHz(0)
{
    StopwatchManager::addStopwatch(*this);
}

//------------------------------------------------------------------------------
// Protected methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool Stopwatch::isFirstMeasurement() const
{
    return myIsFirstMeasurement;
}
