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
/// @file InterruptNRF5340.cpp
/// @author Ben Minerd
/// @date 1/6/2021
/// @brief InterruptNRF5340 class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/NRF5340/InterruptNRF5340.h>

#include <Plat4m_Core/NRF5340/ProcessorNRF5340.h>

using Plat4m::InterruptNRF5340;
using Plat4m::Interrupt;
using Plat4m::Module;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const IRQn_Type InterruptNRF5340::myInterruptMap[] =
{
    /// ID_RESET
    Reset_IRQn,
    /// ID_NON_MASKABLE
    NonMaskableInt_IRQn,
    /// ID_HARD_FAULT
    HardFault_IRQn,
    /// ID_MEMORY_MANAGEMENT
    MemoryManagement_IRQn,
    /// ID_BUS_FAULT
    BusFault_IRQn,
    /// ID_USAGE_FAULT
    UsageFault_IRQn,
    /// ID_SV_CALL
    SVCall_IRQn,
    /// ID_DEBUG_MONITOR
    DebugMonitor_IRQn,
    /// ID_PEND_SV
    PendSV_IRQn,
    /// ID_SYS_TICK
    SysTick_IRQn,
    /// ID_FPU
    FPU_IRQn,
    /// ID_CAHCE
    CACHE_IRQn,
    /// ID_SPU
    SPU_IRQn,
    /// ID_CLOCK_POWER
    CLOCK_POWER_IRQn,
    /// ID_SPIM0_SPIS0_TWIM0_TWIS0_UARTE0
    SPIM0_SPIS0_TWIM0_TWIS0_UARTE0_IRQn,
    /// ID_SPIM1_SPIS1_TWIM1_TWIS1_UARTE1
    SPIM1_SPIS1_TWIM1_TWIS1_UARTE1_IRQn,
    /// ID_SPIM4
    SPIM4_IRQn,
    /// ID_SPIM2_SPIS2_TWIM2_TWIS2_UARTE2
    SPIM2_SPIS2_TWIM2_TWIS2_UARTE2_IRQn,
    /// ID_SPIM3_SPIS3_TWIM3_TWIS3_UARTE3
    SPIM3_SPIS3_TWIM3_TWIS3_UARTE3_IRQn,
    /// ID_GPIOTE0
    GPIOTE0_IRQn,
    /// ID_SAADC
    SAADC_IRQn,
    /// ID_TIMER0
    TIMER0_IRQn,
    /// ID_TIMER1
    TIMER1_IRQn,
    /// ID_TIMER2
    TIMER2_IRQn,
    /// ID_RTC0
    RTC0_IRQn,
    /// ID_RTC1
    RTC1_IRQn,
    /// ID_WDT0
    WDT0_IRQn,
    /// ID_WDT1
    WDT1_IRQn,
    /// ID_COMP_LPCOMP
    COMP_LPCOMP_IRQn,
    /// ID_EGU0
    EGU0_IRQn,
    /// ID_EGU1
    EGU1_IRQn,
    /// ID_EGU2
    EGU2_IRQn,
    /// ID_EGU3
    EGU3_IRQn,
    /// ID_EGU4
    EGU4_IRQn,
    /// ID_EGU5
    EGU5_IRQn,
    /// ID_PWM0
    PWM0_IRQn,
    /// ID_PWM1
    PWM1_IRQn,
    /// ID_PWM2
    PWM2_IRQn,
    /// ID_PWM3
    PWM3_IRQn,
    /// ID_PDM0
    PDM0_IRQn,
    /// ID_I2S0
    I2S0_IRQn,
    /// ID_IPC
    IPC_IRQn,
    /// ID_QSPI
    QSPI_IRQn,
    /// ID_NFCT
    NFCT_IRQn,
    /// ID_GPIOTE1
    GPIOTE1_IRQn,
    /// ID_QDEC0
    QDEC0_IRQn,
    /// ID_QDEC1
    QDEC1_IRQn,
    /// ID_USBD
    USBD_IRQn,
    /// ID_USB_REGULATOR
    USBREGULATOR_IRQn,
    /// ID_KMU
    KMU_IRQn,
    /// ID_CRYPTOCEL
    CRYPTOCELL_IRQn
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
InterruptNRF5340::InterruptNRF5340(const Id id,
                                   HandlerCallback& handlerCallback) :
    Interrupt(handlerCallback),
    myId(id)
{
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error InterruptNRF5340::driverSetEnabled(const bool enabled)
{
    if (enabled)
    {
        NVIC_EnableIRQ(myInterruptMap[myId]);
    }
    else
    {
        NVIC_DisableIRQ(myInterruptMap[myId]);
    }

    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods implemented from Interrupt
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Interrupt::Error InterruptNRF5340::driverConfigure(const Config& config)
{
    NVIC_SetPriority(myInterruptMap[myId], config.priority);

    return Error(ERROR_CODE_NONE);
}
