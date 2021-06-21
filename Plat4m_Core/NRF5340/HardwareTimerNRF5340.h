/**
 * Copyright (c) 2021, SignalQuest LLC
 */

///
/// @file HardwareTimerNRF5340.h
/// @author Ben Minerd
/// @date 1/6/2021
/// @brief HardwareTimerNRF5340 class header file.
///

#ifndef PLAT4M_HARDWARE_TIMER_NRF5340_H
#define PLAT4M_HARDWARE_TIMER_NRF5340_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <stdint.h>

#include <Plat4m_Core/HardwareTimer.h>
#include <Plat4m_Core/NRF5340/TimerNRF5340.h>
#include <Plat4m_Core/NRF5340/InterruptNRF5340.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class HardwareTimerNRF5340 : public HardwareTimer
{
public:
    
    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    HardwareTimerNRF5340(const TimerNRF5340::Id id);

    HardwareTimerNRF5340(const TimerNRF5340::Id id,
                          InterruptHandlerCallback& interruptHandlerCallback);

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    TimerNRF5340& getTimer();

private:
    
    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    TimerNRF5340 myTimer;

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

#endif // PLAT4M_HARDWARE_TIMER_NRF5340_H
