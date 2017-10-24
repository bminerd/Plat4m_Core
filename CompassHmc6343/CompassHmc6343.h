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
 * @file CompassHmc6343.h
 * @author Ben Minerd
 * @date 4/4/13
 * @brief CompassHmc6343 class.
 */

#ifndef _COMPASS_HMC6343_H_
#define _COMPASS_HMC6343_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <Compass.h>
#include <I2cDevice.h>

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

class CompassHmc6343 : public Compass
{
public:
    
    /*--------------------------------------------------------------------------
     * Public enumerations
     *------------------------------------------------------------------------*/
    
    /**
     * @brief Enumeration of compass registers.
     */
    enum Register
    {
        REGISTER_SLAVE_ADDRESS   = (uint8_t) 0x00,
        REGISTER_SW_VERSION      = (uint8_t) 0x02,
        REGISTER_OP_MODE_1       = (uint8_t) 0x04,
        REGISTER_OP_MODE_2       = (uint8_t) 0x05,
        REGISTER_SN_LSB          = (uint8_t) 0x06,
        REGISTER_SN_MSB          = (uint8_t) 0x07,
        REGISTER_DATE_YY         = (uint8_t) 0x08,
        REGISTER_DATE_WW         = (uint8_t) 0x09,
        REGISTER_DEVIATION_LSB   = (uint8_t) 0x0A,
        REGISTER_DEVIATION_MSB   = (uint8_t) 0x0B,
        REGISTER_VARIATION_LSB   = (uint8_t) 0x0C,
        REGISTER_VARIATION_MSB   = (uint8_t) 0x0D,
        REGISTER_X_OFFSET_LSB    = (uint8_t) 0x0E,
        REGISTER_X_OFFSET_MSB    = (uint8_t) 0x0F,
        REGISTER_Y_OFFSET_LSB    = (uint8_t) 0x10,
        REGISTER_Y_OFFSET_MSB    = (uint8_t) 0x11,
        REGISTER_Z_OFFSET_LSB    = (uint8_t) 0x12,
        REGISTER_Z_OFFSET_MSB    = (uint8_t) 0x13,
        REGISTER_FILTER_LSB      = (uint8_t) 0x14,
        REGISTER_FILTER_MSB      = (uint8_t) 0x15
    };
    
    /*--------------------------------------------------------------------------
     * Public constructors and destructors
     *------------------------------------------------------------------------*/
    
    CompassHmc6343(I2c& i2c);
    
private:
    
    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/
    
    I2cDevice myI2cDevice;
    
    /*--------------------------------------------------------------------------
     * Private implemented methods
     *------------------------------------------------------------------------*/
    
    Error driverEnable(const bool enable);
    
    Error driverConfigure(const Config& config);
    
    Error driverGetMeasurement(Measurement& measurement);
    
    /*--------------------------------------------------------------------------
     * Private methods
     *------------------------------------------------------------------------*/
    
    SlaveDevice::Error registerRead(const Register reg, ByteArray& byteArray);
    
    SlaveDevice::Error registerWrite(const Register reg,
                                     const ByteArray& byteArray);
};

#endif // _COMPASS_HMC6343_H_
