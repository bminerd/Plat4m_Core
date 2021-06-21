/**
 * Copyright (c) 2021, SignalQuest LLC
 */

///
/// @file ProcessorNRF5340.h
/// @author Ben Minerd
/// @date 1/6/2021
/// @brief ProcessorNRF5340 class header file.
///

#ifndef PLAT4M_PROCESSOR_NRF5340_H
#define PLAT4M_PROCESSOR_NRF5340_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <stdint.h>

#include <Plat4m_Core/Processor.h>
#include <Plat4m_Core/ErrorTemplate.h>

#include <nrf5340_application.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class ProcessorNRF5340 : public Processor
{
public:
    
    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------
    
    enum ErrorCode
    {
        ERROR_CODE_NONE = 0,
        ERROR_CODE_INPUT_VOLTAGE_INVALID,
        ERROR_CODE_CLOCK_STARTUP_FAILED,
    };
    
    enum ClockSource
    {
        CLOCK_SOURCE_INTERNAL,
        CLOCK_SOURCE_EXTERNAL
    };

    typedef ErrorTemplate<ErrorCode> Error;

    struct Config
    {
        ClockSource clockSource;
        bool enableHighFrequencyExternalOscillator;
        bool enableLowFrequencyExternalOscillator;
        bool disableTrustZone;
    };
    
    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    ProcessorNRF5340(const float coreVoltage);

    ProcessorNRF5340(const float coreVoltage, const Config& config);

    //--------------------------------------------------------------------------
    // Public static methods
    //--------------------------------------------------------------------------
    
    static Config getDefaultConfig();

    static bool isSoftDevicePresent();

    //--------------------------------------------------------------------------
    // Public virtual methods
    //--------------------------------------------------------------------------
    
    virtual Error setConfig(const Config& config);
    
    virtual ClockSource getCoreClockSource();
    
    virtual void setCoreClockSource(const ClockSource coreClockSource);
    
private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    // Constants

    static const uint32_t myExternalClockFrequencyHz;

    static const uint32_t myCoreClockFrequencyHz;

    // Variables
    
    static bool myIsConfigSet;

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    Config myConfig;

    NRF_CLOCK_Type* myClock;

    //--------------------------------------------------------------------------
    // Private virtual methods implemented from Processor
    //--------------------------------------------------------------------------
    
    virtual Processor::Error driverReset();
    
    virtual Processor::Error driverConfigure(const Processor::Config& config);
    
    virtual Processor::Error driverSetPowerMode(
                                          const Processor::PowerMode powerMode);

    virtual uint32_t driverGetCoreClockFrequencyHz();

    virtual Processor::Error driverJumpToAddress(const intptr_t address);
};

}; // namespace Plat4m

#endif // PLAT4M_PROCESSOR_NRF5340_H
