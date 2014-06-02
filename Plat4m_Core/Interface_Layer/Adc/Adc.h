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
 * @file Adc.h
 * @author Ben Minerd
 * @date 4/10/13
 * @brief Adc class.
 */

#ifndef _ADC_H_
#define _ADC_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

class Adc
{
public:

    /*--------------------------------------------------------------------------
     * Public enumerations
     *------------------------------------------------------------------------*/
    
    /**
     * @brief Enumeration of ADC errors.
     */
    enum Error
    {
        ERROR_NONE,
        ERROR_PARAMETER_INVALID,
        ERROR_NOT_ENABLED,
        ERROR_COMMUNICATION
    };
    
    /**
     * @brief Enumeration of ADC modes.
     */
    enum Mode
    {
        MODE_INDEPENDENT,
        MODE_INTERLEAVED
    };
    
    /**
     * @brief Enumeration of ADC conversions.
     */
    enum ConversionMode
    {
        CONVERSION_MODE_SINGLE,
        CONVERSION_MODE_CONTINUOUS
    };
    
    /*--------------------------------------------------------------------------
     * Public structures
     *------------------------------------------------------------------------*/

    /**
     * @brief ADC configuration type.
     */
    struct Config
    {
        ConversionMode conversionMode;
    };
    
    /*--------------------------------------------------------------------------
     * Public virtual destructors
     *------------------------------------------------------------------------*/
    
    virtual ~Adc();
    
    /*--------------------------------------------------------------------------
     * Public methods
     *------------------------------------------------------------------------*/

    /**
     * @brief Sets this ADC enabled or disabled.
     * @param enable Flag that indicates if the ADC should be enabled or disabled.
     * @return ADC error.
     */
    Error enable(const bool enable);

    /**
     * @brief Checks to see if this ADC is enabled or disabled.
     * @param isEnabled Flag that indicates if the ADC is enabled or disabled.
     * @return ADC error.
     */
    Error isEnabled(bool& isEnabled);

    /**
     * @brief Configures this ADC.
     * @param config ADC configuration.
     * @return ADC error.
     */
    Error configure(const Config& config);

    /**
     * @brief Reads a value from this ADC.
     * @param voltage Voltage read from the ADC.
     * @return ADC error.
     */
    Error readVoltage(float& voltage, const unsigned int nReadings = 1);

protected:
    
    /*--------------------------------------------------------------------------
     * Protected constructors
     *------------------------------------------------------------------------*/
    
    Adc();
    
private:

    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/
    
    bool myIsEnabled;
    
    Config myConfig;
    
    /*--------------------------------------------------------------------------
     * Private pure virtual methods
     *------------------------------------------------------------------------*/
    
    virtual Error driverEnable(const bool enable) = 0;
    
    virtual Error driverConfigure(const Config& config) = 0;
    
    virtual Error driverReadVoltage(float& voltage) = 0;
};

#endif // _ADC_H_
