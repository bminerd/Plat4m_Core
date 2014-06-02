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
 * @file AccelAdis26109.h
 * @author Ben Minerd
 * @date 4/8/13
 * @brief AccelAdis16209.
 */

#ifndef _ACCEL_ADIS16209_H_
#define _ACCEL_ADIS16209_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <Accel.h>
#include <SpiDevice.h>

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

class AccelAdis16209 : public Accel
{
public:
    
    /*--------------------------------------------------------------------------
     * Public Enumerations
     *------------------------------------------------------------------------*/
    
    /**
     * @brief Enumeration of ADIS16209 registers.
     */
    enum Register
    {
        REGISTER_ENDURANCE  = (uint8_t) 0x00,
        REGISTER_SUPPLY_OUT = (uint8_t) 0x02,
        REGISTER_XACCL_OUT  = (uint8_t) 0x04,
        REGISTER_YACCL_OUT  = (uint8_t) 0x06,
        REGISTER_AUX_ADC    = (uint8_t) 0x08,
        REGISTER_TEMP_OUT   = (uint8_t) 0x0A,
        REGISTER_XINCL_OUT  = (uint8_t) 0x0C,
        REGISTER_YINCL_OUT  = (uint8_t) 0x0E,
        REGISTER_ROT_OUT    = (uint8_t) 0x10,
        REGISTER_XACCL_NULL = (uint8_t) 0x12,
        REGISTER_YACCL_NULL = (uint8_t) 0x14,
        REGISTER_XINCL_NULL = (uint8_t) 0x16,
        REGISTER_YINCL_NULL = (uint8_t) 0x18,
        REGISTER_ROT_NULL   = (uint8_t) 0x1A,
        REGISTER_ALM_MAG1   = (uint8_t) 0x20,
        REGISTER_ALM_MAG2   = (uint8_t) 0x22,
        REGISTER_ALM_SMPL1  = (uint8_t) 0x24,
        REGISTER_ALM_SMPL2  = (uint8_t) 0x26,
        REGISTER_ALM_CTRL   = (uint8_t) 0x28,
        REGISTER_AUX_DAC    = (uint8_t) 0x30,
        REGISTER_GPIO_CTRL  = (uint8_t) 0x32,
        REGISTER_MSC_CTRL   = (uint8_t) 0x34,
        REGISTER_SMPL_PRD   = (uint8_t) 0x36,
        REGISTER_AVG_CNT    = (uint8_t) 0x38,
        REGISTER_SLP_CNT    = (uint8_t) 0x3A,
        REGISTER_STATUS     = (uint8_t) 0x3C,
        REGISTER_COMMAND    = (uint8_t) 0x3E,
        REGISTER_PROD_ID    = (uint8_t) 0x4A
    };
    
    /*--------------------------------------------------------------------------
     * Public constructors and destructors
     *------------------------------------------------------------------------*/
    
    AccelAdis16209(GpioPin* chipSelectGpioPin, Spi& spi);
    
private:
    
    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/
    
    SpiDevice mySpiDevice;
    
    /*--------------------------------------------------------------------------
     * Private implemented methods
     *------------------------------------------------------------------------*/
    
    Error driverEnable(const bool enable);
    
    Error driverConfigure(const Config& config);
    
    Error driverGetMeasurement(Measurement& measurement);
};

#endif // _ACCEL_ADIS16209_H_
