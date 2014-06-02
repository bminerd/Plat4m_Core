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
 * @file DacLtc2637.c
 * @author Ben Minerd
 * @date 4/3/2013
 * @brief DacLtc2637 class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <DacLtc2637.h>
#include <math.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

static const float maxVoltage       = 2.5f;
static const unsigned int maxValue  = 4095;

/*------------------------------------------------------------------------------
 * Public constructors and destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
DacLtc2637::DacLtc2637(const ChannelId channelId, DacIc& dacIc) :
    Dac(maxVoltage, maxValue),
    myChannelId(channelId),
    myDacIc(dacIc)
{
}

/*------------------------------------------------------------------------------
 * Private methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Dac::Error DacLtc2637::driverEnable(const bool enable)
{
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
Dac::Error DacLtc2637::driverConfigure(const Config& config)
{
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
Dac::Error DacLtc2637::driverSetVoltage(const float voltage)
{
    uint16_t value = computeDacValue(voltage);

    if (myDacIc.registerWrite((DacIcLtc2637::COMMAND_WRITE_N_UPDATE_N << 4) |
                              myChannelId,
                              value) != DacIc::ERROR_NONE)
    {
        return ERROR_COMMUNICATION;
    }

    return ERROR_NONE;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/