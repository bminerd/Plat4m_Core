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
// Copyright (c) 2016 Benjamin Minerd
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
/// @file Dac.h
/// @author Ben Minerd
/// @date 4/3/2013
/// @brief Dac class header file.
///

#ifndef DAC_H
#define DAC_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Module.h>

#include <stdint.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class Dac : public Module
{
public:

    //--------------------------------------------------------------------------
    // Public enumerations
    //--------------------------------------------------------------------------
    
    /**
     * @brief Enumeration of DAC errors.
     */
    enum Error
    {
        ERROR_NONE,
        ERROR_PARAMETER_INVALID,
        ERROR_DAC_INVALID,
        ERROR_CHANNEL_INVALID,
        ERROR_NOT_ENABLED,
        ERROR_COMMUNICATION
    };
    
    //--------------------------------------------------------------------------
    // Public structures
    //--------------------------------------------------------------------------
    
    struct Config
    {
        int a; // Placeholder
    };
    
    //--------------------------------------------------------------------------
    // Public virtual methods
    //--------------------------------------------------------------------------
    
    /**
     * @brief Configures this DAC.
     * @param config DAC configuration.
     * @return DAC error.
     */
    virtual Error configure(const Config& config);

    /**
     * @brief Sets a voltage on this DAC.
     * @param voltage Voltage to set on the DAC.
     * @return DAC error.
     */
    virtual Error setVoltage(const float voltage);
    
    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    float getMaxVoltage() const;
    
    float getMaxValue() const;
    
    float getLevelsPerVolt() const;
    
    float getVoltsPerLevel() const;
    
protected:
    
    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------
    
    Dac(const float maxVoltage, const uint32_t maxValue);
    
    //--------------------------------------------------------------------------
    // Protected virtual destructors
    //--------------------------------------------------------------------------

    virtual ~Dac();

    //--------------------------------------------------------------------------
    // Protected methods
    //--------------------------------------------------------------------------
    
    uint32_t computeDacValue(const float voltage);
    
private:
    
    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------
    
    Config myConfig;
    
    const float myMaxVoltage;
    
    const uint32_t myMaxValue;
    
    const float myLevelsPerVolt;
    
    //--------------------------------------------------------------------------
    // Private pure virtual methods
    //--------------------------------------------------------------------------
    
    virtual Error driverConfigure(const Config& config) = 0;
    
    virtual Error driverSetVoltage(const float voltage) = 0;
};

}; // namespace Plat4m

#endif // _DAC_H_
