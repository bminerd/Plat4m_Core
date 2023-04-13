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
// Copyright (c) 2022-2023 Benjamin Minerd
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
/// @file StopwatchStatisticsPrinter.cpp
/// @author Ben Minerd
/// @date 3/1/2022
/// @brief StopwatchStatisticsPrinter class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdlib>
#include <cstring>
#include <cstdio>

#include <Plat4m_Core/StopwatchStatisticsPrinter/StopwatchStatisticsPrinter.h>
#include <Plat4m_Core/System.h>
#include <Plat4m_Core/CallbackMethod.h>
#include <Plat4m_Core/Printer.h>
#include <Plat4m_Core/ByteArrayN.h>

using namespace Plat4m;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const StopwatchStatisticsPrinter::Config
                                   StopwatchStatisticsPrinter::myDefaultConfig =
{
    1,   /// .outputFrequencyHz
    true /// .outputHeartbeat
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
StopwatchStatisticsPrinter::StopwatchStatisticsPrinter(
                                                const bool createOutputThread,
                                                const Config config) :
    myConfig(config),
    myOutputThread(0)
{
    if (createOutputThread)
    {
        TimeMs periodTimeMs = std::round(1000 / myConfig.outputFrequencyHz);

        myOutputThread = 
            &System::createThread(
                createCallback(
                             this,
                             &StopwatchStatisticsPrinter::outputThreadCallback),
                periodTimeMs,
                4096,
                false,
                "Stopwatch Statistics Printer");
    }
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
StopwatchStatisticsPrinter::~StopwatchStatisticsPrinter()
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void StopwatchStatisticsPrinter::setConfig(const Config& config)
{
    myConfig = config;

    if (isValidPointer(myOutputThread))
    {
        TimeMs periodTimeMs =
                     integerDivideRound(1000, (int) myConfig.outputFrequencyHz);
        myOutputThread->setPeriodMs(periodTimeMs);
    }
}

//------------------------------------------------------------------------------
StopwatchStatisticsPrinter::Config StopwatchStatisticsPrinter::getConfig()
{
    return myConfig;
}

//------------------------------------------------------------------------------
void StopwatchStatisticsPrinter::printStopwatchStatistics()
{
    ByteArrayN<2048> bytes;

    bytes.append("\n--------------------\n");
    bytes.append("Stopwatch Statistics\n");
    bytes.append("--------------------\n\n");

    List<Stopwatch*>::Iterator stopwatchIterator =
                                       Stopwatch::getStopwatchList().iterator();

    while (stopwatchIterator.hasCurrent())
    {
        Stopwatch* stopwatch = stopwatchIterator.current();

        const char* stopwatchName = stopwatch->getName();

        if (stopwatchName == 0)
        {
            stopwatchName = "(Unnamed)";
        }

        addStopwatchName(bytes, stopwatchName);
        addStopwatchStatistics(bytes, stopwatch);

        stopwatchIterator.next();
    }

    bytes.append("\n---- End ----\n");

    Printer::print(bytes);
}

//------------------------------------------------------------------------------
// Private virtual methods overridden for Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error StopwatchStatisticsPrinter::driverSetEnabled(const bool enabled)
{
    if (isValidPointer(myOutputThread))
    {
        myOutputThread->setEnabled(enabled);
    }

    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void StopwatchStatisticsPrinter::outputThreadCallback()
{
    if (myConfig.outputHeartbeat)
    {
        Printer::print(".");
    }
    else
    {
        printStopwatchStatistics();
    }
}

//------------------------------------------------------------------------------
void StopwatchStatisticsPrinter::addStopwatchName(ByteArray& byteArray,
                                                  const char* name)
{
    byteArray.append(name);
    byteArray.append("\n");

    std::uint32_t nameSize = strlen(name);

    for (std::uint32_t i = 0; i < nameSize; i++)
    {
        byteArray.append("-");
    }

    byteArray.append("\n");
}

//------------------------------------------------------------------------------
void StopwatchStatisticsPrinter::addStopwatchStatistics(ByteArray& byteArray, 
                                                        Stopwatch* stopwatch)
{
    char numberString[10];
    TimeUs timeUs;

    timeUs = (stopwatch->getCpuTimeStamp().toTimeUs());
    std::snprintf(numberString, arraySize(numberString), "%u", timeUs);
    byteArray.append("- CPU Time (uS): ");
    byteArray.append(numberString);
    byteArray.append("\n");

    timeUs = (stopwatch->getMinCpuTimeStamp().toTimeUs());
    std::snprintf(numberString, arraySize(numberString), "%u", timeUs);
    byteArray.append("  - Min (uS): ");
    byteArray.append(numberString);
    byteArray.append("\n");

    timeUs = (stopwatch->getMaxCpuTimeStamp().toTimeUs());
    std::snprintf(numberString, arraySize(numberString), "%u", timeUs);
    byteArray.append("  - Max (uS): ");
    byteArray.append(numberString);
    byteArray.append("\n");

    timeUs = (stopwatch->getElapsedTimeStamp().toTimeUs());
    std::snprintf(numberString, arraySize(numberString), "%u", timeUs);
    byteArray.append("- Elapsed Time (uS): ");
    byteArray.append(numberString);
    byteArray.append("\n");

    timeUs = (stopwatch->getMinElapsedTimeStamp().toTimeUs());
    std::snprintf(numberString, arraySize(numberString), "%u", timeUs);
    byteArray.append("  - Min (uS): ");
    byteArray.append(numberString);
    byteArray.append("\n");

    timeUs = (stopwatch->getMaxElapsedTimeStamp().toTimeUs());
    std::snprintf(numberString, arraySize(numberString), "%u", timeUs);
    byteArray.append("  - Max (uS): ");
    byteArray.append(numberString);
    byteArray.append("\n");

    byteArray.append("\n");
}
