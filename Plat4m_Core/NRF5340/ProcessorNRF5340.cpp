/**
 * Copyright (c) 2021, SignalQuest LLC
 */

///
/// @file ProcessorNRF5340.cpp
/// @author Ben Minerd
/// @date 1/6/2021
/// @brief ProcessorNRF5340 class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <nrf5340_application_peripherals.h>
#include <nrf5340_application_bitfields.h>

#include <Plat4m_Core/NRF5340/ProcessorNRF5340.h>

using Plat4m::ProcessorNRF5340;
using Plat4m::Processor;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const uint32_t ProcessorNRF5340::myExternalClockFrequencyHz = 32000000;

const uint32_t ProcessorNRF5340::myCoreClockFrequencyHz     = 128000000;

// Variables

bool ProcessorNRF5340::myIsConfigSet = false;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ProcessorNRF5340::ProcessorNRF5340(const float coreVoltage) :
    Processor(ENDIAN_LITTLE, coreVoltage, myExternalClockFrequencyHz),
    myClock(NRF_CLOCK_S)
{
}

//------------------------------------------------------------------------------
ProcessorNRF5340::ProcessorNRF5340(const float coreVoltage,
                                   const ProcessorNRF5340::Config& config) :
    Processor(ENDIAN_LITTLE, coreVoltage, myExternalClockFrequencyHz),
    myClock(NRF_CLOCK_S)
{
    myIsConfigSet = true;
    myConfig = config;
}

//------------------------------------------------------------------------------
// Public static methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ProcessorNRF5340::Config ProcessorNRF5340::getDefaultConfig()
{
    Config config;
    config.clockSource = ProcessorNRF5340::CLOCK_SOURCE_EXTERNAL;
    config.enableHighFrequencyExternalOscillator = true;
    config.enableLowFrequencyExternalOscillator  = false;

    return config;
}

//------------------------------------------------------------------------------
// Public virtual methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ProcessorNRF5340::Error ProcessorNRF5340::setConfig(const Config& config)
{
    if (config.disableTrustZone)
    {
        myClock = NRF_CLOCK_NS;

        clearBit(NRF_SPU_S->PERIPHID[5].PERM, 4);
        clearBit(NRF_SPU_S->PERIPHID[66].PERM, 4);
        clearBits(NRF_SPU_S->GPIOPORT[0].PERM, 0xFFFFFFFF);
        clearBits(NRF_SPU_S->GPIOPORT[1].PERM, 0xFFFFFFFF);
    }

    setBit(NRF_CACHE_S->ENABLE, 0);

    if (config.enableHighFrequencyExternalOscillator)
    {
        // setBit(myClock->HFCLKALWAYSRUN, 0);

        // If HFXO already running, we are done
        if (isBitSet(myClock->HFCLKSTAT, CLOCK_HFCLKSTAT_SRC_Pos))
        {
            return Error(ERROR_CODE_NONE);
        }

        clearBit(myClock->HFCLKCTRL, 0);

        // Check if HFXO "start" task has already been requested - if not,
        // request it
        if (!isBitSet(myClock->HFCLKRUN, CLOCK_HFCLKRUN_STATUS_Pos))
        {
            setBit(myClock->TASKS_HFCLKSTART,
                   CLOCK_TASKS_HFCLKSTART_TASKS_HFCLKSTART_Pos);
        }

        // Wait for HFXO to be started and acting as the clock source
        // TODO: want a watchdog timer in production use to make sure we start!
        while (!isBitSet(myClock->EVENTS_HFCLKSTARTED, 0))
        {
            // Do nothing
        }

        while (!isBitSet(myClock->HFCLKSTAT, CLOCK_HFCLKSTAT_SRC_Pos))
        {
            // Do nothing
        }
    }

    if (config.enableLowFrequencyExternalOscillator)
    {
        // See if there is a pending request to start the LFCLK - if so, wait
        // for it to start
        if (isBitSet(myClock->LFCLKRUN, CLOCK_LFCLKRUN_STATUS_Pos))
        {
            // TODO: want a watchdog timer in production use to make sure we
            // start!
            while (!isBitSet(myClock->LFCLKSTAT, CLOCK_LFCLKSTAT_STATE_Pos))
            {
                // Do nothing
            }
        }

        // See if the LFCLK is running - if yes and crystal is the source we are
        // done, return, otherwise stop it
        if (isBitSet(myClock->LFCLKSTAT, CLOCK_LFCLKSTAT_STATE_Pos))
        {
            if ((myClock->LFCLKSTAT & CLOCK_LFCLKSTAT_SRC_Msk) ==
                           (CLOCK_LFCLKSRC_SRC_LFXO << CLOCK_LFCLKSTAT_SRC_Pos))
            {
                return Error(ERROR_CODE_NONE);
            }
            
            /* request a stop and wait for the clock to stop */
            myClock->TASKS_LFCLKSTOP =
                (CLOCK_TASKS_LFCLKSTOP_TASKS_LFCLKSTOP_Trigger <<
                                    CLOCK_TASKS_LFCLKSTOP_TASKS_LFCLKSTOP_Pos);
            while (myClock->LFCLKSTAT &
                        (CLOCK_LFCLKSTAT_STATE_Running <<
                                                    CLOCK_LFCLKSTAT_STATE_Pos))
            {
                // Do nothing
            }
        }

        // Set the LFCLK source to the crystal oscillator
        setBits(myClock->LFCLKSRC,
                CLOCK_LFCLKSRC_SRC_LFXO << CLOCK_LFCLKSRC_SRC_Pos);

        // Start the LFCLK and wait for it to be ready
        setBit(myClock->TASKS_LFCLKSTART, 0);
        // TODO: want a watchdog timer in production use to make sure we start!
        while (!isBitSet(myClock->EVENTS_LFCLKSTARTED, 0))
        {
            // Do nothing
        }
    }

    myConfig = config;
    myIsConfigSet = true;
    
    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
ProcessorNRF5340::ClockSource ProcessorNRF5340::getCoreClockSource()
{
    return (myConfig.clockSource);
}

//------------------------------------------------------------------------------
void ProcessorNRF5340::setCoreClockSource(const ClockSource coreClockSource)
{
    // Set clock source
}

//------------------------------------------------------------------------------
// Private virtual methods implemented from Processor
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Processor::Error ProcessorNRF5340::driverReset()
{
    if (!myIsConfigSet)
    {
        myConfig = getDefaultConfig();
    }

    setConfig(myConfig);

    return Processor::Error(Processor::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
Processor::Error ProcessorNRF5340::driverConfigure(
                                                const Processor::Config& config)
{
    return Processor::Error(Processor::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
Processor::Error ProcessorNRF5340::driverSetPowerMode(
                                           const Processor::PowerMode powerMode)
{
    switch (powerMode)
    {
        case Processor::POWER_MODE_SLEEP:
        {
            __WFE();

            break;
        }
        default:
        {
            break;
        }
    }
    
    return Processor::Error(Processor::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
uint32_t ProcessorNRF5340::driverGetCoreClockFrequencyHz()
{
    return myCoreClockFrequencyHz;
}

//------------------------------------------------------------------------------
Processor::Error ProcessorNRF5340::driverJumpToAddress(const intptr_t address)
{
    // if (address_is_in_range...)
    {
        uint32_t jumpAddress = *(__IO uint32_t*) (address + 4);
        void (*jumpToApplication)()  = (void (*)()) jumpAddress;

        // Initialize user application's Stack Pointer
        __set_MSP(* ( __IO uint32_t* ) address);

        jumpToApplication();
    }

    return Processor::Error(Processor::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Interrupt service routines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
extern "C" void NMI_Handler(void)
{
}

//------------------------------------------------------------------------------
extern "C" void HardFault_Handler(void)
{
	// TODO: Define additional behavior
	while (true)
    {
        // Do nothing
    }
}

//------------------------------------------------------------------------------
extern "C" void MemManage_Handler(void)
{
	// TODO: Define additional behavior
    while (true)
    {
        // Do nothing
    }
}

//------------------------------------------------------------------------------
extern "C" void BusFault_Handler(void)
{
	// TODO: Define additional behavior
    while (true)
    {
        // Do nothing
    }
}

//------------------------------------------------------------------------------
extern "C" void UsageFault_Handler(void)
{
	// TODO: Define additional behavior
	while (true)
    {
        // Do nothing
    }
}

//------------------------------------------------------------------------------
extern "C" void DebugMon_Handler(void)
{
    while (true)
    {
        // Do nothing
    }
}
