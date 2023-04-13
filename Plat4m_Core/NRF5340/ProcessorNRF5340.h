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
// Copyright (c) 2021-2023 Benjamin Minerd
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
    // Private virtual methods overridden for Processor
    //--------------------------------------------------------------------------
    
    virtual Processor::Error driverReset() override;
    
    virtual Processor::Error driverConfigure(const Processor::Config& config) override;
    
    virtual Processor::Error driverSetPowerMode(
                                 const Processor::PowerMode powerMode) override;

    virtual uint32_t driverGetCoreClockFrequencyHz() override;

    virtual Processor::Error driverJumpToAddress(const intptr_t address);
};

}; // namespace Plat4m

#endif // PLAT4M_PROCESSOR_NRF5340_H
