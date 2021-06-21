/**
 * Copyright (c) 2021, SignalQuest LLC
 */

///
/// @file HardwareTimerNRF5340.cpp
/// @author Ben Minerd
/// @date 1/6/2021
/// @brief HardwareTimerNRF5340 class.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/NRF5340/HardwareTimerNRF5340.h>
#include <Plat4m_Core/NRF5340/ProcessorNRF5340.h>

using Plat4m::HardwareTimerNRF5340;
using Plat4m::HardwareTimer;
using Plat4m::Module;
using Plat4m::TimerNRF5340;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
HardwareTimerNRF5340::HardwareTimerNRF5340(const TimerNRF5340::Id id) :
    HardwareTimer(),
    myTimer(id)
{
}

//------------------------------------------------------------------------------
HardwareTimerNRF5340::HardwareTimerNRF5340(
                           const TimerNRF5340::Id id,
                           InterruptHandlerCallback& interruptHandlerCallback) :
    HardwareTimer(),
    myTimer(id, interruptHandlerCallback)
{
}

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
TimerNRF5340& HardwareTimerNRF5340::getTimer()
{
    return myTimer;
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error HardwareTimerNRF5340::driverSetEnabled(const bool enabled)
{
    myTimer.setEnabled(enabled);
    
    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods implemented from HardwareTimer
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
HardwareTimer::Error HardwareTimerNRF5340::driverSetConfig(
                                                           const Config& config)
{
    TimerNRF5340::Config timerConfig;
    timerConfig.mode        = TimerNRF5340::MODE_TIMER;
    timerConfig.frequencyHz = config.frequencyHz;
    timerConfig.resolution  = TimerNRF5340::RESOLUTION_MAX;
    // timerConfig.resolution  = TimerNRF5340::RESOLUTION_MIN;
    timerConfig.maxFrequencyErrorPercent = 0.5;

    myTimer.setConfig(timerConfig);

    return Error(ERROR_CODE_NONE);
}
