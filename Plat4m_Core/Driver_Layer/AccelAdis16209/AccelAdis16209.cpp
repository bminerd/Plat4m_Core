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
 * @file accel_driver_adis16209.c
 * @author Ben Minerd
 * @date 2/25/12
 * @brief Accelerometer driver layer for the ADIS16209.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <AccelAdis16209.h>
#include <ByteArrayN.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

#define ACCEL_REG_SIZE                          2
#define ACCEL_INCLINATION_CONVERSION_FACTOR     0.025f

/*------------------------------------------------------------------------------
 * Public constructors and destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
AccelAdis16209::AccelAdis16209(GpioPin* chipSelectGpioPin, Spi& spi) :
    Accel(),
    mySpiDevice(chipSelectGpioPin, spi)
{
}

/*------------------------------------------------------------------------------
 * Private implemented methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Accel::Error AccelAdis16209::driverEnable(const bool enable)
{
    mySpiDevice.enable(enable);
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
Accel::Error AccelAdis16209::driverConfigure(const Config& config)
{
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
Accel::Error AccelAdis16209::driverGetMeasurement(Measurement& measurement)
{
    ByteArrayN<2> txByteArray;
    txByteArray.append((uint8_t) REGISTER_XINCL_OUT);
    txByteArray.append((uint8_t) 0);
    
    ByteArrayN<2> rxByteArray;

    // X sample
    if (mySpiDevice.txRx(txByteArray, rxByteArray) != SlaveDevice::ERROR_NONE)
    {
        return ERROR_COMMUNICATION;
    }

    txByteArray[0] = (uint8_t) REGISTER_YINCL_OUT;
    measurement.rawSamples[DOF_X] = (((uint16_t) rxByteArray[0]) << 10) |
                                    (((uint16_t) rxByteArray[1]) << 2);
    measurement.rawSamples[DOF_X] /= 4;
    measurement.samples[DOF_X] = measurement.rawSamples[DOF_X] *
                                 ACCEL_INCLINATION_CONVERSION_FACTOR;

    // Y sample
    if (mySpiDevice.txRx(txByteArray, rxByteArray) != SlaveDevice::ERROR_NONE)
    {
        return ERROR_COMMUNICATION;
    }

    measurement.rawSamples[DOF_Y] = (((uint16_t) rxByteArray[0]) << 10) |
                                    (((uint16_t) rxByteArray[1]) << 2);
    measurement.rawSamples[DOF_Y] /= 4;
    measurement.samples[DOF_Y] = measurement.rawSamples[DOF_Y] *
                                 ACCEL_INCLINATION_CONVERSION_FACTOR;

    return ERROR_NONE;
}