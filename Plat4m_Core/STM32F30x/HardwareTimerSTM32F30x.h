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
/// @file HardwareTimerSTM32F30x.h
/// @author Ben Minerd
/// @date 7/12/2016
/// @brief HardwareTimerSTM32F30x class header file.
///

#ifndef PLAT4M_HARDWARE_TIMER_STM32F30X_H
#define PLAT4M_HARDWARE_TIMER_STM32F30X_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <stdint.h>

#include <Plat4m_Core/HardwareTimer.h>
#include <Plat4m_Core/STM32F30x/TimerSTM32F30x.h>
#include <Plat4m_Core/STM32F30x/InterruptSTM32F30x.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class HardwareTimerSTM32F30x : public HardwareTimer
{
public:
    
    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    HardwareTimerSTM32F30x(const TimerSTM32F30x::Id id);

    HardwareTimerSTM32F30x(const TimerSTM32F30x::Id id,
                           InterruptHandlerCallback& interruptHandlerCallback);

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    TimerSTM32F30x& getTimer();

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    static const TimerSTM32F30x::Interrupt myInterruptMap[];
    
    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------
    
    InterruptSTM32F30x* myUpdateInterrupt;

    TimerSTM32F30x myTimer;

    uint32_t myPeriod;

    //--------------------------------------------------------------------------
    // Private methods implemented from Module
    //--------------------------------------------------------------------------
    
    Module::Error driverSetEnabled(const bool enable);
    
    //--------------------------------------------------------------------------
    // Private methods implemented from HardwareTimer
    //--------------------------------------------------------------------------

    Error driverSetConfig(const Config& config);
};

}; // namespace Plat4m

#endif // PLAT4M_HARDWARE_TIMER_STM32F30X_H
