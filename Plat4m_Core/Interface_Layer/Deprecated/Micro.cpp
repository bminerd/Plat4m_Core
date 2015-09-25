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
 * @file Micro.cpp
 * @author Ben Minerd
 * @date 6/3/2013
 * @brief Micro class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Micro.h>

/*------------------------------------------------------------------------------
 * Static data members
 *----------------------------------------------------------------------------*/

Micro* Micro::myDriver                      = NULL_POINTER;
float Micro::myCoreVoltage                  = 0.0f;
uint32_t Micro::myClockSourceFrequencyHz    = 0;
Micro::Config Micro::myConfig;

/*------------------------------------------------------------------------------
 * Public static methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
float Micro::getCoreVoltage()
{
    return myCoreVoltage;
}

//------------------------------------------------------------------------------
uint32_t Micro::getClockSourceFrequencyHz()
{
    return myClockSourceFrequencyHz;
}

//------------------------------------------------------------------------------
Micro::Error Micro::reset()
{
    return myDriver->driverReset();
}

//------------------------------------------------------------------------------
Micro::Error Micro::configure(const Config& config)
{
    Micro::Error error = myDriver->driverConfigure(config);

    if (error == Micro::ERROR_NONE)
    {
        myConfig = config;
    }
    
    return error;
}

//------------------------------------------------------------------------------
Micro::Error Micro::setPowerMode(const PowerMode powerMode)
{
    return myDriver->driverSetPowerMode(powerMode);
}

/*------------------------------------------------------------------------------
 * Protected constructors and destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Micro::Micro(const float coreVoltage, const uint32_t clockSourceFrequencyHz)
{
    if (IS_NULL_POINTER(myDriver))
    {
        myCoreVoltage               = coreVoltage;
        myClockSourceFrequencyHz    = clockSourceFrequencyHz;
        myDriver                    = this;
    }
}
