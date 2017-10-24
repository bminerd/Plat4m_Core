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
 * @file DacLtc2637.h
 * @author Ben Minerd
 * @date 4/3/2013
 * @brief DacLtc2637 class.
 */

#ifndef _DAC_LTC2637_H_
#define _DAC_LTC2637_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <Dac.h>
#include <DacIcLtc2637.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

#define DAC_IC_LTC2637_CHANNEL_COUNT 8

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Structures
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

class DacLtc2637 : public Dac
{
public:
    
    /*--------------------------------------------------------------------------
     * Public enumerations
     *------------------------------------------------------------------------*/
    
    enum ChannelId
    {
        CHANNEL_ID_A = 0,
        CHANNEL_ID_B,
        CHANNEL_ID_C,
        CHANNEL_ID_D,
        CHANNEL_ID_E,
        CHANNEL_ID_F,
        CHANNEL_ID_G,
        CHANNEL_ID_H
    };
    
    /*--------------------------------------------------------------------------
     * Public constructors and destructors
     *------------------------------------------------------------------------*/
    
    DacLtc2637(const ChannelId channelId, DacIc& dacIc);
    
private:
    
    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/
    
    const ChannelId myChannelId;
    
    DacIc& myDacIc;
    
    /*--------------------------------------------------------------------------
     * Private implemented methods
     *------------------------------------------------------------------------*/
    
    Error driverEnable(const bool enable);
    
    Error driverConfigure(const Config& config);
    
    Error driverSetVoltage(const float voltage);
};

#endif // _DAC_LTC2637_H_
