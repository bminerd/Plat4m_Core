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
// Copyright (c) 2013 Benjamin Minerd
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
/// @file Processor.h
/// @author Ben Minerd
/// @date 12/26/2013
/// @brief Processor class header file.
///

#ifndef PLAT4M_PROCESSOR_H
#define PLAT4M_PROCESSOR_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <stdint.h>

#include <Plat4m_Core/ErrorTemplate.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class Processor
{
public:

    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------
    
    enum ErrorCode
    {
        ERROR_CODE_NONE,
        ERROR_CODE_PARAMETER_INVALID,
        ERROR_CODE_NOT_ENABLED
    };

    enum Interrupt
    {
        INTERRUPT_OUTPUT_COMPARE
    };
    
    enum PowerMode
    {
        POWER_MODE_RUN,
        POWER_MODE_SLEEP
    };
    
    typedef ErrorTemplate<ErrorCode> Error;
    
    struct Config
    {
        int a; // Placeholder
    };
    
    //--------------------------------------------------------------------------
    // Public static methods
    //--------------------------------------------------------------------------

    static VoltageV getCoreVoltageV();
    
    static uint32_t getClockSourceFrequencyHz();
    
    static Error reset();
    
    static Error configure(const Config& config);
    
    static Error setPowerMode(const PowerMode powerMode);
    
    static uint32_t getCoreClockFrequencyHz();

protected:
    
    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------
    
    Processor(const VoltageV coreVoltageV,
              const uint32_t clockSourceFrequencyHz);
    
    //--------------------------------------------------------------------------
    // Protected virtual destructors
    //--------------------------------------------------------------------------

    virtual ~Processor();

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------
    
    static Processor* myDriver;
    
    static VoltageV myCoreVoltageV;
    
    static uint32_t myClockSourceFrequencyHz;
    
    static Config myConfig;
    
    //--------------------------------------------------------------------------
    // Private pure virtual methods
    //--------------------------------------------------------------------------
    
    virtual Processor::Error driverReset() = 0;
    
    virtual Processor::Error driverConfigure(
                                           const Processor::Config& config) = 0;
    
    virtual Processor::Error driverSetPowerMode(
                                      const Processor::PowerMode powerMode) = 0;

    virtual uint32_t driverGetCoreClockFrequencyHz() = 0;
};

}; // namespace Plat4m

#endif // PLAT4M_PROCESSOR_H
