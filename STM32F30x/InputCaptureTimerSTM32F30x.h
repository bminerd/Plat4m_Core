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
/// @file InputCaptureTimerSTM32F30x.h
/// @author Ben Minerd
/// @date 5/15/2017
/// @brief InputCaptureTimerSTM32F30x class header file.
///

#ifndef PLAT4M_INPUT_CAPTURE_TIMER_STM32F30X_H
#define PLAT4M_INPUT_CAPTURE_TIMER_STM32F30X_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <InputCaptureTimer.h>
#include <TimerSTM32F30x.h>
#include <GpioPinSTM32F30x.h>

#include <stdint.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class InputCaptureTimerSTM32F30x : public InputCaptureTimer
{
public:

    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------
    
    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    InputCaptureTimerSTM32F30x(TimerSTM32F30x& timer,
                               const TimerSTM32F30x::ChannelId channelId,
                               GpioPinSTM32F30x& gpioPin);

    InputCaptureTimerSTM32F30x(TimerSTM32F30x& timer,
                               const TimerSTM32F30x::ChannelId channelId,
                               GpioPinSTM32F30x& gpioPin,
                               CaptureReadyCallback& captureReadyCallback);

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~InputCaptureTimerSTM32F30x();

    //--------------------------------------------------------------------------
    // Public methods implemented from InputCapturetimer
    //--------------------------------------------------------------------------

    uint32_t getValueFast();

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    TimerSTM32F30x& getTimer();

private:
    
    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------
    
    TimerSTM32F30x& myTimer;
    
    const TimerSTM32F30x::ChannelId myChannelId;

    GpioPinSTM32F30x& myGpioPin;

    //--------------------------------------------------------------------------
    // Private methods implemented from Module
    //--------------------------------------------------------------------------
    
    Module::Error driverSetEnabled(const bool enabled);
    
    //--------------------------------------------------------------------------
    // Private methods implemented from InputCaptureTimer
    //--------------------------------------------------------------------------

    Error driverSetConfig(const Config& config);

    uint32_t driverGetTimerValue();

    uint32_t driverGetCaptureValue();

    uint32_t driverGetPeriodValue();
};

}; // namespace Plat4m

#endif // PLAT4M_INPUT_CAPTURE_TIMER_STM32F30X_H
