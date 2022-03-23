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
/// @file StopwatchStatisticsPrinter.h
/// @author Ben Minerd
/// @date 3/1/2022
/// @brief StopwatchStatisticsPrinter class header file.
///

#ifndef PLAT4M_STATISTICS_PRINTER_H
#define PLAT4M_STATISTICS_PRINTER_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/Module.h>
#include <Plat4m_Core/Stopwatch.h>
#include <Plat4m_Core/List.h>
#include <Plat4m_Core/Thread.h>
#include <Plat4m_Core/ByteArray.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class StopwatchStatisticsPrinter : public Module
{
public:

    //--------------------------------------------------------------------------
    // Public data types
    //--------------------------------------------------------------------------

    struct Config
    {
        FrequencyHz outputFrequencyHz;
    };

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    StopwatchStatisticsPrinter(const bool createOutputThread = false,
                               const Config config = myDefaultConfig);

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~StopwatchStatisticsPrinter();

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    void setConfig(const Config& config);

    Config getConfig();

    void setStopwatchNameList(List<const char*>& stopwatchNameList);

    List<const char*>* getStopwatchNameList();

    void printStopwatchStatistics();
    
private:
    
    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    static const Config myDefaultConfig;

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    Config myConfig;

    List<const char*>* myStopwatchNameList;

    Thread* myOutputThread;

    //--------------------------------------------------------------------------
    // Private virtual methods overridden for Module
    //--------------------------------------------------------------------------

    virtual Module::Error driverSetEnabled(const bool enabled) override;

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------

    void outputThreadCallback();

    void addStopwatchName(ByteArray& byteArray, const char* name);

    void addStopwatchStatistics(ByteArray& byteArray,
                                Stopwatch* stopwatch);
};

}; // namespace Plat4m

#endif // PLAT4M_STATISTICS_PRINTER_H
