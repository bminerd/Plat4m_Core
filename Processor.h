/*------------------------------------------------------------------------------
 *       _______    __                           ___
 *      ||  ___ \  || |             __          //  |
 *      || |  || | || |   _______  || |__      //   |    _____  ___
 *      || |__|| | || |  // ___  | ||  __|    // _  |   ||  _ \/ _ \
 *      ||  ____/  || | || |  || | || |      // /|| |   || |\\  /\\ \
 *      || |       || | || |__|| | || |     // /_|| |_  || | || | || |
 *      || |       || |  \\____  | || |__  //_____   _| || | || | || |
 *      ||_|       ||_|       ||_|  \\___|       ||_|   ||_| ||_| ||_|
 *
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2013 Benjamin Minerd
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *----------------------------------------------------------------------------*/

/**
 * @file Processor.h
 * @author Ben Minerd
 * @date 12/26/2013
 * @brief Processor namespace.
 */

#ifndef _PROCESSOR_H_
#define _PROCESSOR_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

class Processor
{
public:

    /*--------------------------------------------------------------------------
     * Public enumerations
     *------------------------------------------------------------------------*/
    
    /**
     * @brief Enumeration of micro errors.
     */
    enum Error
    {
        ERROR_NONE,
        ERROR_PARAMETER_INVALID,
        ERROR_NOT_ENABLED
    };

    /**
     * @brief Enumeration of timer interrupts.
     */
    enum Interrupt
    {
        INTERRUPT_OUTPUT_COMPARE
    };
    
    enum PowerMode
    {
        POWER_MODE_RUN,
        POWER_MODE_SLEEP
    };
    
    /*--------------------------------------------------------------------------
     * Public structures
     *------------------------------------------------------------------------*/
    
    struct Config
    {
        int a; // Placeholder
    };
    
    /*--------------------------------------------------------------------------
     * Public static methods
     *------------------------------------------------------------------------*/

    static float getCoreVoltage();
    
    static uint32_t getClockSourceFrequencyHz();
    
    static Error reset();
    
    static Error configure(const Config& config);
    
    static Error setPowerMode(const PowerMode powerMode);
    
protected:
    
    /*--------------------------------------------------------------------------
     * Protected constructors and destructors
     *------------------------------------------------------------------------*/
    
    Processor(const float coreVoltage, const uint32_t clockSourceFrequencyHz);
    
private:

    /*--------------------------------------------------------------------------
     * Private static data members
     *------------------------------------------------------------------------*/
    
    static Processor* myDriver;
    
    static float myCoreVoltage;
    
    static uint32_t myClockSourceFrequencyHz;
    
    static Config myConfig;
    
    /*--------------------------------------------------------------------------
     * Private virtual methods
     *------------------------------------------------------------------------*/
    
    virtual Processor::Error driverReset() = 0;
    
    virtual Processor::Error driverConfigure(
                                           const Processor::Config& config) = 0;
    
    virtual Processor::Error driverSetPowerMode(
                                      const Processor::PowerMode powerMode) = 0;
};

#endif // _PROCESSOR_H_
