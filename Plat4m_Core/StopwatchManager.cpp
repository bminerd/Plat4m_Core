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
/// @file StopwatchManager.cpp
/// @author Ben Minerd
/// @date 2/28/2022
/// @brief StopwatchManager class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/StopwatchManager.h>
#include <Plat4m_Core/MemoryAllocator.h>
#include <Plat4m_Core/System.h>
#include <Plat4m_Core/CallbackFunction.h>

using namespace Plat4m;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

StopwatchManager* StopwatchManager::myDriver = 0;

//------------------------------------------------------------------------------
// Public static methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Stopwatch& StopwatchManager::createStopwatch(const char* name)
{
    if (isNullPointer(myDriver))
    {
        PLAT4M_REPORT_ERROR_STATIC(
                              StopwatchManager::Error,
                              StopwatchManager::ERROR_CODE_INSTANCE_NOT_CREATED,
                              ErrorBase::SEVERITY_CRITICAL,
                              StopwatchManager);
    }

    if (isNullPointer(myDriver->myBackgroundStopwatch))
    {
        myDriver->myBackgroundStopwatch =
                             &(myDriver->subclassCreateStopwatch("Background"));
    }

    return (myDriver->subclassCreateStopwatch(name));
}

//------------------------------------------------------------------------------
void StopwatchManager::addStopwatch(Stopwatch& stopwatch)
{
    if (isNullPointer(myDriver))
    {
        PLAT4M_REPORT_ERROR_STATIC(
                              StopwatchManager::Error,
                              StopwatchManager::ERROR_CODE_INSTANCE_NOT_CREATED,
                              ErrorBase::SEVERITY_CRITICAL,
                              StopwatchManager);
    }

    myDriver->addStopwatchPrivate(stopwatch);
}

//------------------------------------------------------------------------------
void StopwatchManager::removeStopwatch(Stopwatch& stopwatch)
{
    if (isNullPointer(myDriver))
    {
        PLAT4M_REPORT_ERROR_STATIC(
                              StopwatchManager::Error,
                              StopwatchManager::ERROR_CODE_INSTANCE_NOT_CREATED,
                              ErrorBase::SEVERITY_CRITICAL,
                              StopwatchManager);
    }

    myDriver->removeStopwatchPrivate(stopwatch);
}

//------------------------------------------------------------------------------
List<Stopwatch*>& StopwatchManager::getStopwatchList()
{
    if (isNullPointer(myDriver))
    {
        PLAT4M_REPORT_ERROR_STATIC(
                              StopwatchManager::Error,
                              StopwatchManager::ERROR_CODE_INSTANCE_NOT_CREATED,
                              ErrorBase::SEVERITY_CRITICAL,
                              StopwatchManager);
    }

    return (myDriver->getStopwatchListPrivate());
}

//------------------------------------------------------------------------------
void StopwatchManager::computeBackgroundCalculations()
{
    if (isNullPointer(myDriver))
    {
        PLAT4M_REPORT_ERROR_STATIC(
                              StopwatchManager::Error,
                              StopwatchManager::ERROR_CODE_INSTANCE_NOT_CREATED,
                              ErrorBase::SEVERITY_CRITICAL,
                              StopwatchManager);
    }

    myDriver->computeBackgroundCalculationsPrivate();
}

//------------------------------------------------------------------------------
TimeStamp StopwatchManager::getCurrentTimeStamp()
{
    if (isNullPointer(myDriver))
    {
        PLAT4M_REPORT_ERROR_STATIC(
                              StopwatchManager::Error,
                              StopwatchManager::ERROR_CODE_INSTANCE_NOT_CREATED,
                              ErrorBase::SEVERITY_CRITICAL,
                              StopwatchManager);
    }

    return (myDriver->subclassGetCurrentTimeStamp());
}

//------------------------------------------------------------------------------
// Protected constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
StopwatchManager::StopwatchManager(const TimeStamp& cpuLoadTimeWindow) :
    myStopwatchList(),
    myBackgroundStopwatch(0),
    myCpuLoadTimeWindow(cpuLoadTimeWindow),
    myCpuLoadTimeWindowStartTimeStamp(),
    myCpuLoadTimeWindowEndTimeStamp(),
    myIsMeasuringCpuLoadTimeWindow(false),
    myIsFirstBackgroundMeasurement(true)
{
    if (isValidPointer(myDriver))
    {
        PLAT4M_REPORT_ERROR(
                          StopwatchManager::Error,
                          StopwatchManager::ERROR_CODE_INSTANCE_ALREADY_CREATED,
                          ErrorBase::SEVERITY_CRITICAL,
                          this);
    }

    myDriver = this;
}

//------------------------------------------------------------------------------
// Protected virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
StopwatchManager::~StopwatchManager()
{
    List<Stopwatch*>::Iterator iterator = myStopwatchList.iterator();

    while (iterator.hasCurrent())
    {
        Stopwatch* stopwatch = iterator.current();

        stopwatch->~Stopwatch();

        iterator.next();
    }

    myDriver = 0;
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void StopwatchManager::addStopwatchPrivate(Stopwatch& stopwatch)
{
    Stopwatch* pointer = &stopwatch;

    myStopwatchList.append(pointer);
}

//------------------------------------------------------------------------------
void StopwatchManager::removeStopwatchPrivate(Stopwatch& stopwatch)
{
    Stopwatch* pointer = &stopwatch;

    myStopwatchList.remove(pointer);
}

//------------------------------------------------------------------------------
List<Stopwatch*>& StopwatchManager::getStopwatchListPrivate()
{
    return myStopwatchList;
}

//------------------------------------------------------------------------------
void StopwatchManager::computeBackgroundCalculationsPrivate()
{
    if (myIsFirstBackgroundMeasurement)
    {
        myBackgroundStopwatch->start();

        myIsFirstBackgroundMeasurement = false;
    }

    myCpuLoadTimeWindowEndTimeStamp = subclassGetCurrentTimeStamp();

    if (myCpuLoadTimeWindowEndTimeStamp - myCpuLoadTimeWindowStartTimeStamp >=
                                                            myCpuLoadTimeWindow)
    {
        myBackgroundStopwatch->stop();

        TimeStamp cpuTimeStamp = myBackgroundStopwatch->getCpuTimeStamp();
        TimeStamp elapsedTimeStamp =
                                   myBackgroundStopwatch->getElapsedTimeStamp();

        volatile float cpuTimeS = cpuTimeStamp.toTimeSFloat();

        volatile float elapsedTimeS = elapsedTimeStamp.toTimeSFloat();

        volatile float cpuLoadPercentage =
                                          100 * (1 - (cpuTimeS / elapsedTimeS));

        myCpuLoadTimeWindowEndTimeStamp += myCpuLoadTimeWindow;

        List<Stopwatch*>::Iterator iterator = myStopwatchList.iterator();

        while (iterator.hasCurrent())
        {
            Stopwatch* stopwatch = iterator.current();

            volatile float cummulativeCpuTimeS =
                         stopwatch->getCummulativeCpuTimeStamp().toTimeSFloat();

            volatile float stopwatchCpuLoadPercentage =
                                     100 * (cummulativeCpuTimeS / elapsedTimeS);

            stopwatch->setCpuLoadPercentage(stopwatchCpuLoadPercentage);

            stopwatch->resetEventCount();
            stopwatch->resetCummulativeCpuTimeStamp();

            iterator.next();
        }

        myBackgroundStopwatch->start();

        myCpuLoadTimeWindowStartTimeStamp = subclassGetCurrentTimeStamp();
    }
}
