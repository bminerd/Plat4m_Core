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
/// @file InputCaptureTimer.cpp
/// @author Ben Minerd
/// @date 12/22/2016
/// @brief InputCaptureTimer class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <InputCaptureTimer.h>

using Plat4m::InputCaptureTimer;
using Plat4m::Module;
using Plat4m::GpioPin;

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GpioPin& InputCaptureTimer::getGpioPin()
{
    return myGpioPin;
}

//------------------------------------------------------------------------------
void InputCaptureTimer::setCaptureReadyCallback(
                                     CaptureReadyCallback& captureReadyCallback)
{
    myCaptureReadyCallback = &captureReadyCallback;
}

//------------------------------------------------------------------------------
void InputCaptureTimer::setCaptureInterruptCallback(
                             CaptureInterruptCallback& captureInterruptCallback)
{
    myCaptureInterruptCallback = &captureInterruptCallback;
}

//------------------------------------------------------------------------------
InputCaptureTimer::Error InputCaptureTimer::setConfig(const Config& config)
{
    if (!isEnabled())
    {
        return Error(ERROR_CODE_NOT_ENABLED);
    }
    
    Error error = driverSetConfig(config);
    
    if (error.getCode() == ERROR_CODE_NONE)
    {
        myConfig = config;

        uint32_t periodValue = driverGetPeriodValue();
        mySecondsPerCount = (1.0/myConfig.frequencyHz) /
                            ((RealNumber) periodValue);
        mySecondsPerPeriod = ((RealNumber) periodValue) *
                             mySecondsPerCount;
    }
    
    return error;
}

//------------------------------------------------------------------------------
InputCaptureTimer::Config InputCaptureTimer::getConfig() const
{
    return myConfig;
}

//------------------------------------------------------------------------------
Plat4m::RealNumber InputCaptureTimer::getElapsedTimeS()
{
    if (!isEnabled())
    {
        return 0.0;
    }

    uint32_t value = driverGetTimerValue();

    return ((((RealNumber) value) * mySecondsPerCount) +
            myOverflowCountSeconds);
}

//------------------------------------------------------------------------------
Plat4m::RealNumber InputCaptureTimer::getElapsedCaptureTimeS()
{
    if (!isEnabled())
    {
        return 0.0;
    }

    uint32_t value = driverGetCaptureValue();

    return ((((RealNumber) value) * mySecondsPerCount) +
            myOverflowCountSeconds);
}

//------------------------------------------------------------------------------
void InputCaptureTimer::overflowInterruptHandler()
{
    myOverflowCount++;
    myOverflowCountSeconds = ((RealNumber) myOverflowCount) *
                             mySecondsPerPeriod;
}

//------------------------------------------------------------------------------
void InputCaptureTimer::captureInterruptHandler()
{
    if (isValidPointer(myCaptureInterruptCallback))
    {
        myCaptureInterruptCallback->call();
    }

    myOverflowCount = 0;
    myOverflowCountSeconds = 0.0;
}

//------------------------------------------------------------------------------
// Protected constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
InputCaptureTimer::InputCaptureTimer(GpioPin& gpioPin) :
    Module(),
    myGpioPin(gpioPin),
    myCaptureReadyCallback(0),
    myCaptureInterruptCallback(0),
    myConfig(),
    myOverflowCount(0),
    myCaptureOverflowCount(0),
    mySecondsPerCount(0.0),
    mySecondsPerPeriod(0.0),
    myOverflowCountSeconds(0.0)
{
}

//------------------------------------------------------------------------------
// Protected virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
InputCaptureTimer::~InputCaptureTimer()
{
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error InputCaptureTimer::interfaceSetEnabled(const bool enabled)
{
    myGpioPin.setEnabled(enabled);

    return Module::Error(Module::ERROR_CODE_NONE);
}
