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
// Copyright (c) 2013-2023 Benjamin Minerd
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
/// @file HardwareTimerSTM32F4xx.h
/// @author Ben Minerd
/// @date 5/15/2013
/// @brief HardwareTimerSTM32F4xx class header file.
///

#ifndef PLAT4M_HARDWARE_TIMER_STM32F4XX_H
#define PLAT4M_HARDWARE_TIMER_STM32F4XX_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <stdint.h>

#include <Plat4m_Core/HardwareTimer.h>
#include <Plat4m_Core/STM32F4xx/TimerSTM32F4xx.h>
#include <Plat4m_Core/STM32F4xx/InterruptSTM32F4xx.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class HardwareTimerSTM32F4xx : public HardwareTimer
{
public:

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    HardwareTimerSTM32F4xx(const TimerSTM32F4xx::Id id);

    HardwareTimerSTM32F4xx(const TimerSTM32F4xx::Id id,
                           InterruptHandlerCallback& interruptHandlerCallback);

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~HardwareTimerSTM32F4xx();

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    TimerSTM32F4xx& getTimer();

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    InterruptSTM32F4xx* myUpdateInterrupt;

    TimerSTM32F4xx myTimer;

    uint32_t myPeriod;

    //--------------------------------------------------------------------------
    // Private virtual methods overridden for Module
    //--------------------------------------------------------------------------

    virtual Module::Error driverSetEnabled(const bool enable) override;

    //--------------------------------------------------------------------------
    // Private virtual methods overridden for HardwareTimer
    //--------------------------------------------------------------------------

    virtual Error driverSetConfig(const HardwareTimer::Config& config) override;
};

}; // namespace Plat4m

#endif // PLAT4M_HARDWARE_TIMER_STM32F4XX_H
