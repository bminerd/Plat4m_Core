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
 * @file Accel.h
 * @author Ben Minerd
 * @date 2/27/13
 * @brief Accel class.
 */

#ifndef _ACCEL_H_
#define _ACCEL_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

class Accel
{
public:
    
    /*--------------------------------------------------------------------------
     * Public enumerations
     *------------------------------------------------------------------------*/
    
    /**
     * @brief Enumeration of accelerometer errors.
     */
    enum Error
    {
        ERROR_NONE,
        ERROR_PARAMETER_INVALID,
        ERROR_NOT_ENABLED,
        ERROR_COMMUNICATION
    };

    /**
     * @brief Enumeration of accelerometer degrees of freedom.
     */
    enum Dof
    {
        DOF_X = 0,
        DOF_Y,
        DOF_Z
    };
    
    /*--------------------------------------------------------------------------
     * Public structures
     *------------------------------------------------------------------------*/
    
    struct Config
    {
        int a; // Placeholder
    };
    
    struct Measurement
    {
        int16_t rawSamples[3];
        float samples[3];
    };
    
    /*--------------------------------------------------------------------------
     * Public virtual destructors
     *------------------------------------------------------------------------*/
    
    virtual ~Accel();
    
    /*--------------------------------------------------------------------------
     * Public methods
     *------------------------------------------------------------------------*/
    
    /**
     * @brief Sets this accelerometer enabled or disabled.
     * @param enable Flag that indicates if the accelerometer should be enabled or
     * disabled.
     * @return Accelerometer error.
     */
    Error enable(const bool enable);

    /**
     * @brief Checks to see if this accelerometer is enabled or disabled.
     * @param isEnabled Flag that indicates if the accelerometer is enabled or
     * disabled.
     * @return Accelerometer error.
     */
    Error isEnabled(bool& isEnabled);

    /**
     * @brief Configures this accelerometer.
     * @param config Accelerometer configuration.
     * @return Accelerometer error.
     */
    Error configure(const Config& config);

    /**
     * @brief Gets an accelerometer reading.
     * @param reading Accelerometer reading.
     * @return Accelerometer error.
     */
    Error getMeasurement(Measurement& measurement);
    
protected:
    
    /*--------------------------------------------------------------------------
     * Protected constructors
     *------------------------------------------------------------------------*/
    
    Accel();
    
private:
    
    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/
    
    bool myIsEnabled;
    
    /*--------------------------------------------------------------------------
     * Private virtual methods
     *------------------------------------------------------------------------*/
    
    virtual Error driverEnable(const bool enable) = 0;
    
    virtual Error driverConfigure(const Config& config) = 0;
    
    virtual Error driverGetMeasurement(Measurement& measurement) = 0;
};

#endif // _ACCEL_H_