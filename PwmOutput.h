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
// Copyright (c) 2017 Benjamin Minerd
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
/// @file PwmOutput.h
/// @author Ben Minerd
/// @date 5/15/2013
/// @brief PwmOutput class header file.
///

#ifndef PLAT4M_PWM_OUTPUT_H
#define PLAT4M_PWM_OUTPUT_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Module.h>
#include <ErrorTemplate.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class PwmOutput : public Module
{
public:
    
    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------
    
    enum ErrorCode
    {
        ERROR_CODE_NONE,
        ERROR_CODE_PARAMETER_INVALID,
        ERROR_CODE_MODE_INVALID,
        ERROR_CODE_PERIOD_INVALID,
        ERROR_CODE_NOT_ENABLED
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
    
    enum Alignment
    {
        ALIGNMENT_EDGE = 0,
        ALIGNMENT_CENTER
    };

    typedef ErrorTemplate<ErrorCode> Error;
    
    struct Config
    {
        float frequencyHz;
        Polarity polarity;
        float dutyCyclePercent;
        float minDutyCyclePercent;
        float maxDutyCyclePercent;
        Alignment alignment;
        bool complementaryOutputEnabled;
        Polarity complementaryPolarity;
    };

    //--------------------------------------------------------------------------
    // Public inline pure virtual methods
    //--------------------------------------------------------------------------

    virtual inline void setDutyCyclePercentFast(
    										  const float dutyCyclePercent) = 0;
    
    virtual inline void setDutyCycleNormalizedFast(
    									   const float dutyCycleNormalized) = 0;

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    Error setConfig(const Config& config);
    
    Config getConfig() const;

    Error setDutyCyclePercent(const float dutyCyclePercent);

    Error setActive(const bool active);

    Error setComplementaryActive(const bool active);

protected:
    
    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------
    
    PwmOutput();
    
    //--------------------------------------------------------------------------
    // Protected virtual destructors
    //--------------------------------------------------------------------------

    virtual ~PwmOutput();

private:
    
    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------
    
    Config myConfig;
    
    float myDutyCycleRangePercent;

    bool myIsActive;

    //--------------------------------------------------------------------------
    // Private pure virtual methods
    //--------------------------------------------------------------------------
    
    virtual Error driverSetConfig(const Config& config) = 0;

    virtual Error driverSetDutyCyclePercent(const float dutyCyclePercent) = 0;

    virtual Error driverSetActive(const bool active) = 0;

    virtual Error driverSetComplementaryActive(const bool active) = 0;
};

}; // namespace Plat4m

#endif // PLAT4M_PWM_OUTPUT_H
