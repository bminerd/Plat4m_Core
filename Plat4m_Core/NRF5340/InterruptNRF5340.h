/**
 * Copyright (c) 2021, SignalQuest LLC
 */

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

#include <Plat4m_Core/Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/Plat4m_Core/Interrupt.h>

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
        ID_POWER_CLOCK,
        ID_RADIO,
        ID_UARTE0_UART0,
        ID_SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0,
        ID_SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1,
        ID_NFCT,
        ID_GPIOTE,
        ID_SAADC,
        ID_TIMER0,
        ID_TIMER1,
        ID_TIMER2,
        ID_RTC0,
        ID_TEMP,
        ID_RNG,
        ID_ECB,
        ID_CCM_AAR,
        ID_WDT,
        ID_RTC1,
        ID_QDEC,
        ID_COMP_LPCOMP,
        ID_SWI0_EGU0,
        ID_SWI1_EGU1,
        ID_SWI2_EGU2,
        ID_SWI3_EGU3,
        ID_SWI4_EGU4,
        ID_SWI5_EGU5,
        ID_TIMER3,
        ID_TIMER4,
        ID_PWM0,
        ID_PDM,
        ID_MWU,
        ID_PWM1,
        ID_PWM2,
        ID_SPIM2_SPIS2_SPI2,
        ID_RTC2,
        ID_I2S,
        ID_FPU,
        ID_USBD,
        ID_UARTE1,
        ID_QSPI,
        ID_CRYPTOCELL,
        ID_PWM3,
        ID_SPIM3
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
