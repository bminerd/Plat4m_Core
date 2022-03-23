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
/// @file InterruptNRF5340.h
/// @author Ben Minerd
/// @date 1/6/2021
/// @brief InterruptNRF5340 class header file.
///

#ifndef PLAT4M_INTERRUPT_NRF5340_H
#define PLAT4M_INTERRUPT_NRF5340_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <nrf5340_application.h>

#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/Interrupt.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class InterruptNRF5340 : public Interrupt
{
public:
    
    //--------------------------------------------------------------------------
    // Public enumerations
    //--------------------------------------------------------------------------
    
    enum Id
    {
        ID_RESET = 0,
        ID_NON_MASKABLE,
        ID_HARD_FAULT,
        ID_MEMORY_MANAGEMENT,
        ID_BUS_FAULT,
        ID_USAGE_FAULT,
        ID_SV_CALL,
        ID_DEBUG_MONITOR,
        ID_PEND_SV,
        ID_SYS_TICK,
        ID_FPU,
        ID_CAHCE,
        ID_SPU,
        ID_CLOCK_POWER,
        ID_SPIM0_SPIS0_TWIM0_TWIS0_UARTE0,
        ID_SPIM1_SPIS1_TWIM1_TWIS1_UARTE1,
        ID_SPIM4,
        ID_SPIM2_SPIS2_TWIM2_TWIS2_UARTE2,
        ID_SPIM3_SPIS3_TWIM3_TWIS3_UARTE3,
        ID_GPIOTE0,
        ID_SAADC,
        ID_TIMER0,
        ID_TIMER1,
        ID_TIMER2,
        ID_RTC0,
        ID_RTC1,
        ID_WDT0,
        ID_WDT1,
        ID_COMP_LPCOMP,
        ID_EGU0,
        ID_EGU1,
        ID_EGU2,
        ID_EGU3,
        ID_EGU4,
        ID_EGU5,
        ID_PWM0,
        ID_PWM1,
        ID_PWM2,
        ID_PWM3,
        ID_PDM0,
        ID_I2S0,
        ID_IPC,
        ID_QSPI,
        ID_NFCT,
        ID_GPIOTE1,
        ID_QDEC0,
        ID_QDEC1,
        ID_USBD,
        ID_USB_REGULATOR,
        ID_KMU,
        ID_CRYPTOCELL
    };

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    InterruptNRF5340(const Id id, HandlerCallback& handlerCallback);

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    // Constants

    static const IRQn_Type myInterruptMap[];

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------
    
    const Id myId;
    
    //--------------------------------------------------------------------------
    // Private methods implemented from Module
    //--------------------------------------------------------------------------
    
    Module::Error driverSetEnabled(const bool enabled);

    //--------------------------------------------------------------------------
    // Private methods implemented from Interrupt
    //--------------------------------------------------------------------------
    
    Error driverConfigure(const Config& config);
};

}; // namespace Plat4m

#endif // PLAT4M_INTERRUPT_NRF5340_H
