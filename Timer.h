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
 * @file Timer.h
 * @author Ben Minerd
 * @date 5/15/2013
 * @brief Timer class.
 */

#ifndef _TIMER_H_
#define _TIMER_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

class Timer
{

public:
    
    /*--------------------------------------------------------------------------
     * Public enumerations
     *------------------------------------------------------------------------*/
    
    /**
     * @brief Enumeration of timer errors.
     */
    enum Error
    {
        ERROR_NONE,
        ERROR_PARAMETER_INVALID,
        ERROR_MODE_INVALID,
        ERROR_PERIOD_INVALID,
        ERROR_NOT_ENABLED
    };

    enum Mode
    {
        MODE_COUNTING_UP = 0,
        MODE_COUNTING_DOWN,
        MODE_COUNTING_UP_DOWN,
        MODE_INPUT_CAPTURE,
        MODE_OUTPUT_COMPARE,
        MODE_PWM_INPUT,
        MODE_PWM_OUTPUT,
        MODE_PULSE
    };
    
    enum Polarity
    {
        POLARITY_HIGH = 0,
        POLARITY_LOW
    };
    
    enum Resolution
    {
        RESOLUTION_MIN = 0,
        RESOLUTION_MAX,
        RESOLUTION_LEVELS
    };
    
    /**
     * @brief Enumeration of timer interrupts.
     */
    enum Interrupt
    {
        INTERRUPT_OUTPUT_COMPARE
    };
    
    /*--------------------------------------------------------------------------
     * Public structures
     *------------------------------------------------------------------------*/
    
    struct Config
    {
        Mode mode;
        float frequencyHz;
        Resolution resolution;
        unsigned int resolutionLevels;
    };
    
    struct PwmConfig
    {
        Polarity polarity;
        float dutyCyclePercent;
    };
    
    /*--------------------------------------------------------------------------
     * Public virtual destructors
     *------------------------------------------------------------------------*/
    
    virtual ~Timer();
    
    /*--------------------------------------------------------------------------
     * Public methods
     *------------------------------------------------------------------------*/

    /**
     * @brief Sets the given timer enabled or disabled.
     * @param enabled Flag that indicates if the timer should be enabled or
     * disabled.
     * @return Timer error.
     */
    Error enable(const bool enable);

    /**
     * @brief Checks to see if the given timer is enabled or disabled.
     * @param isEnabled Flag that indicates if the timer is enabled or disabled.
     * @return Timer error.
     */
    Error isEnabled(bool& isEnabled);

    /**
     * @brief Configures the given timer.
     * @param config Timer configuration.
     * @return Timer error.
     */
    Error configure(const Config& config);
    
    const Config& getConfig() const;
    
    Error pwmConfigure(const PwmConfig& config);
    
protected:
    
    /*--------------------------------------------------------------------------
     * Protected constructors
     *------------------------------------------------------------------------*/
    
    Timer();
    
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
    
    virtual Error driverPwmConfigure(const PwmConfig& pwmConfig) = 0;
};

#endif // _TIMER_H_
