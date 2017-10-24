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
// Copyright (c) 2017 Benjamin Minerd
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
/// @file CurrentSensor.h
/// @author Ben Minerd
/// @date 1/12/2017
/// @brief CurrentSensor class header file.
///

#ifndef PLAT4M_CURRENT_SENSOR_H
#define PLAT4M_CURRENT_SENSOR_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

// Plat4m includes
#include <Module.h>
#include <Callback.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class CurrentSensor : public Module
{
public:

    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------

    enum ErrorCode
    {
        ERROR_CODE_NONE = 0,
        ERROR_CODE_NOT_ENABLED,
        ERROR_CODE_DIRECTION_INVALID
    };

    typedef ErrorTemplate<ErrorCode> Error;

    struct Config
    {
        uint32_t nSamplesToAverage;
    };

    typedef Callback<> CurrentReadyCallback;

    //--------------------------------------------------------------------------
    // Public pure virtual methods
    //--------------------------------------------------------------------------

    virtual CurrentA getCurrentAFast() = 0;

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    Config getConfig() const;

    Error setConfig(const Config& config);

    void setCurrentReadyCallback(CurrentReadyCallback& currentReadyCallback);

	CurrentA getOffsetCurrentA();

    Error setOffsetCurrentA(const CurrentA offsetCurrentA);

    Error getCurrentA(CurrentA& currentA);

    Error startOffsetCalibration(const uint32_t nSamples);

    bool isOffsetCalibrationComplete();

protected:

    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------

    CurrentSensor();

    //--------------------------------------------------------------------------
    // Protected virtual destructors
    //--------------------------------------------------------------------------

    virtual ~CurrentSensor();

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    Config myConfig;

    CurrentReadyCallback* myCurrentReadyCallback;

    CurrentA myOffsetCurrentA;

    //--------------------------------------------------------------------------
    // Private virtual methods
    //--------------------------------------------------------------------------

    virtual Error driverStartOffsetCalibration(const uint32_t nSamples);

    virtual bool driverIsOffsetCalibrationComplete();

    //--------------------------------------------------------------------------
    // Private pure virtual methods
    //--------------------------------------------------------------------------

    virtual Error driverSetConfig(const Config& config) = 0;

    virtual Error driverGetCurrentA(CurrentA& currentA) = 0;
};

}; // namespace Plat4m

#endif // PLAT4M_CURRENT_SENSOR_H
