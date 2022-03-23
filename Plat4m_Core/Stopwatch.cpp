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
// Copyright (c) 2022 Benjamin Minerd
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
#include <Plat4m_Core/System.h>

using namespace Plat4m;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

List<Stopwatch*> Stopwatch::myStopwatchList;

List<Stopwatch*>::Iterator Stopwatch::myStopwatchListIterator =
                                                     myStopwatchList.iterator();

Stopwatch* Stopwatch::myCurrentStopwatch = 0;

//------------------------------------------------------------------------------
// Public static methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
List<Stopwatch*>& Stopwatch::getStopwatchList()
{
    return myStopwatchList;
}

//------------------------------------------------------------------------------
// Protected constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Stopwatch::Stopwatch() :
    myName(),
    myPreemptedStopwatch(0),
    myIsFirstMeasurement(true),
    myStartTimeStamp(),
    myEndTimeStamp(),
    myCpuTimeStamp(),
    myMinCpuTimeStamp(),
    myMaxCpuTimeStamp(),
    myElapsedTimeStamp(),
    myMinElapsedTimeStamp(),
    myMaxElapsedTimeStamp(),
    myPreemptedTimeStamp()
{
    Stopwatch* pointer = this;

    myStopwatchList.append(pointer);
}

//------------------------------------------------------------------------------
// Protected virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Stopwatch::~Stopwatch()
{
    Stopwatch* pointer = this;

    myStopwatchList.remove(pointer);
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void Stopwatch::start()
{
    System::enterCriticalSection();
    
    myStartTimeStamp = getCurrentTimeStamp();

    myPreemptedStopwatch = myCurrentStopwatch;

    myCurrentStopwatch = this;

    System::exitCriticalSection();

    // Reset the preemption time
    myPreemptedTimeStamp = TimeStamp();
}

//------------------------------------------------------------------------------
void Stopwatch::stop()
{
    System::enterCriticalSection();

    myEndTimeStamp = getCurrentTimeStamp();

    myElapsedTimeStamp = myEndTimeStamp - myStartTimeStamp;

    if (isValidPointer(myPreemptedStopwatch))
    {
        myPreemptedStopwatch->myPreemptedTimeStamp += myElapsedTimeStamp;
    }

    myCurrentStopwatch = myPreemptedStopwatch;

    myPreemptedStopwatch = 0;

    System::exitCriticalSection();

    // The calculations below rely on non-static data members and can be
    // interrupted

    myCpuTimeStamp = myElapsedTimeStamp - myPreemptedTimeStamp;

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
}

//------------------------------------------------------------------------------
TimeStamp Stopwatch::getCpuTimeStamp()
{
    return myCpuTimeStamp;
}

//------------------------------------------------------------------------------
TimeStamp Stopwatch::getMinCpuTimeStamp()
{
    return myMinCpuTimeStamp;
}

//------------------------------------------------------------------------------
TimeStamp Stopwatch::getMaxCpuTimeStamp()
{
    return myMaxCpuTimeStamp;
}

//------------------------------------------------------------------------------
TimeStamp Stopwatch::getElapsedTimeStamp()
{
    return myElapsedTimeStamp;
}

//------------------------------------------------------------------------------
TimeStamp Stopwatch::getMinElapsedTimeStamp()
{
    return myMinElapsedTimeStamp;
}

//------------------------------------------------------------------------------
TimeStamp Stopwatch::getMaxElapsedTimeStamp()
{
    return myMaxElapsedTimeStamp;
}
