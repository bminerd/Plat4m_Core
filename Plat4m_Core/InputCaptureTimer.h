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
// Copyright (c) 2016 Benjamin Minerd
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
/// @file InputCaptureTimer.h
/// @author Ben Minerd
/// @date 12/22/2016
/// @brief InputCaptureTimer class header file.
///

#ifndef PLAT4M_INPUT_CAPTURE_TIMER_H
#define PLAT4M_INPUT_CAPTURE_TIMER_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/Module.h>
#include <Plat4m_Core/ErrorTemplate.h>
#include <Plat4m_Core/Callback.h>
#include <Plat4m_Core/GpioPin.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class InputCaptureTimer : public Module
{
public:
    
    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------
    
    enum ErrorCode
    {
        ERROR_CODE_NONE,
        ERROR_CODE_PARAMETER_INVALID,
        ERROR_CODE_MODE_INVALID,
        ERROR_CODE_PERIOD_INVALID,
        ERROR_CODE_NOT_ENABLED
    };

    typedef ErrorTemplate<ErrorCode> Error;
    
    typedef Callback<> CaptureReadyCallback;

    typedef Callback<> CaptureInterruptCallback;

    struct Config
    {
        RealNumber frequencyHz;
    };

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    GpioPin& getGpioPin();

    void setCaptureReadyCallback(CaptureReadyCallback& captureReadyCallback);

    void setCaptureInterruptCallback(
                            CaptureInterruptCallback& captureInterruptCallback);

    Error setConfig(const Config& config);
    
    Config getConfig() const;

    RealNumber getElapsedTimeS();

    RealNumber getElapsedCaptureTimeS();

    void overflowInterruptHandler();

    void captureInterruptHandler();

protected:
    
    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------
    
    InputCaptureTimer(GpioPin& gpioPin);
    
    //--------------------------------------------------------------------------
    // Protected virtual destructors
    //--------------------------------------------------------------------------

    virtual ~InputCaptureTimer();

private:
    
    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------
    
    GpioPin& myGpioPin;

    CaptureReadyCallback* myCaptureReadyCallback;

    CaptureInterruptCallback* myCaptureInterruptCallback;

    Config myConfig;

    uint32_t myOverflowCount;

    uint32_t myCaptureOverflowCount;

    RealNumber mySecondsPerCount;

    RealNumber mySecondsPerPeriod;

    RealNumber myOverflowCountSeconds;

    //--------------------------------------------------------------------------
    // Private methods implemented from Module
    //--------------------------------------------------------------------------

    Module::Error interfaceSetEnabled(const bool enabled);

    //--------------------------------------------------------------------------
    // Private pure virtual methods
    //--------------------------------------------------------------------------
    
    virtual Error driverSetConfig(const Config& config) = 0;

    virtual uint32_t driverGetTimerValue() = 0;

    virtual uint32_t driverGetCaptureValue() = 0;

    virtual uint32_t driverGetPeriodValue() = 0;
};

}; // namespace Plat4m

#endif // PLAT4M_INPUT_CAPTURE_TIMER_H
