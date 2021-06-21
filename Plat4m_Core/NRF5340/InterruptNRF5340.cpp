/**
 * Copyright (c) 2021, SignalQuest LLC
 */

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
    /// ID_POWER_CLOCK,
    POWER_CLOCK_IRQn,
    /// ID_RADIO
    RADIO_IRQn,
    /// ID_UARTE0_UART0
    UARTE0_UART0_IRQn,
    /// ID_SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0
    SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQn,
    /// ID_SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1
    SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQn,
    /// ID_NFCT
    NFCT_IRQn,
    /// ID_GPIOTE
    GPIOTE_IRQn,
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
    /// ID_TEMP
    TEMP_IRQn,
    /// ID_RNG
    RNG_IRQn,
    /// ID_ECB
    ECB_IRQn,
    /// ID_CCM_AAR
    CCM_AAR_IRQn,
    /// ID_WDT
    WDT_IRQn,
    /// ID_RTC1
    RTC1_IRQn,
    /// ID_QDEC,
    QDEC_IRQn,
    /// ID_COMP_LPCOMP
    COMP_LPCOMP_IRQn,
    /// ID_SWI0_EGU0
    SWI0_EGU0_IRQn,
    /// ID_SWI1_EGU1
    SWI1_EGU1_IRQn,
    /// ID_SWI2_EGU2
    SWI2_EGU2_IRQn,
    /// ID_SWI3_EGU3
    SWI3_EGU3_IRQn,
    /// ID_SWI4_EGU4
    SWI4_EGU4_IRQn,
    /// ID_SWI5_EGU5
    SWI5_EGU5_IRQn,
    /// ID_TIMER3
    TIMER3_IRQn,
    /// ID_TIMER4
    TIMER4_IRQn,
    /// ID_PWM0
    PWM0_IRQn,
    /// ID_PDM
    PDM_IRQn,
    /// ID_MWU
    MWU_IRQn,
    /// ID_PWM1
    PWM1_IRQn,
    /// ID_PWM2
    PWM2_IRQn,
    /// ID_SPIM2_SPIS2_SPI2
    SPIM2_SPIS2_SPI2_IRQn,
    /// ID_RTC2
    RTC2_IRQn,
    /// ID_I2S
    I2S_IRQn,
    /// ID_FPU
    FPU_IRQn,
    /// ID_USBD
    USBD_IRQn,
    /// ID_UARTE1
    UARTE1_IRQn,
    /// ID_QSPI
    QSPI_IRQn,
    /// ID_CRYPTOCELL
    CRYPTOCELL_IRQn,
    /// ID_PWM3
    PWM3_IRQn,
    /// ID_SPIM3
    SPIM3_IRQn
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
        if (ProcessorNRF5340::isSoftDevicePresent())
        {
            sd_nvic_EnableIRQ(myInterruptMap[myId]);
        }
        else
        {
            NVIC_EnableIRQ(myInterruptMap[myId]);
        }

    }
    else
    {
        if (ProcessorNRF5340::isSoftDevicePresent())
        {
            sd_nvic_DisableIRQ(myInterruptMap[myId]);
        }
        else
        {
            NVIC_DisableIRQ(myInterruptMap[myId]);
        }
        
    }

    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods implemented from Interrupt
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Interrupt::Error InterruptNRF5340::driverConfigure(const Config& config)
{
    if (ProcessorNRF5340::isSoftDevicePresent())
    {
        sd_nvic_SetPriority(myInterruptMap[myId], config.priority);
    }
    else
    {
        NVIC_SetPriority(myInterruptMap[myId], config.priority);
    }

    return Error(ERROR_CODE_NONE);
}
