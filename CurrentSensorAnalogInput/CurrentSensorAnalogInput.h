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
/// @file CurrentSensorAnalogInput.h
/// @author Ben Minerd
/// @date 1/12/2017
/// @brief CurrentSensorAnalogInput class header file.
///

#ifndef PLAT4M_CURRENT_SENSOR_ANALOG_INPUT_H
#define PLAT4M_CURRENT_SENSOR_ANALOG_INPUT_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

// Plat4m includes
#include <CurrentSensor.h>
#include <AnalogInput.h>
#include <Plat4m.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class CurrentSensorAnalogInput : public CurrentSensor
{
public:

    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    CurrentSensorAnalogInput(const RealNumber scale,
                             const RealNumber offset,
                             AnalogInput& analogInput);

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~CurrentSensorAnalogInput();

    //--------------------------------------------------------------------------
    // Public methods implemented from CurrentSensore
    //--------------------------------------------------------------------------

    CurrentA getCurrentAFast();

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    const RealNumber myScale;
    const RealNumber myOffset;

    AnalogInput& myAnalogInput;

    //--------------------------------------------------------------------------
    // Private methods implemented from Module
    //--------------------------------------------------------------------------

    Module::Error driverSetEnabled(const bool enabled);

    //--------------------------------------------------------------------------
    // Private methods implemented from CurrentSensor
    //--------------------------------------------------------------------------

    Error driverSetConfig(const Config& config);

    Error driverGetCurrentA(CurrentA& currentA);

    Error driverStartOffsetCalibration(const uint32_t nSamples);

    bool driverIsOffsetCalibrationComplete();
};

}; // namespace Plat4m

#endif // PLAT4M_CURRENT_SENSOR_ANALOG_INPUT_H
