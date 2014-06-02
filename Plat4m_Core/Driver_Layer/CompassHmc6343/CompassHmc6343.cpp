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
 * @file CompassHmc6343.cpp
 * @author Ben Minerd
 * @date 4/4/13
 * @brief CompassHmc6343 class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <CompassHmc6343.h>
#include <ByteArrayN.h>
#include <System.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/**
 * @brief Compass I2C address.
 */
#define COMPASS_HMC6343_I2C_ADDRESS         0x19
     
/**
 * @brief Compass reading conversion factor.
 */
#define COMPASS_READING_CONVERSION_FACTOR   0.1f

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

static const uint8_t i2cAddress = 0x19;

static const float conversionFactor = 0.1f;

/*------------------------------------------------------------------------------
 * Public constructors and destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
CompassHmc6343::CompassHmc6343(I2c& i2c) :
    myI2cDevice(i2cAddress, i2c)
{
}

/*------------------------------------------------------------------------------
 * Private implemented methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Compass::Error CompassHmc6343::driverEnable(const bool enable)
{
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
Compass::Error CompassHmc6343::driverConfigure(const Config& config)
{
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
Compass::Error CompassHmc6343::driverGetMeasurement(Measurement& measurement)
{
    ByteArrayN<1> txData;
    txData.append(0x50);
    
    ByteArrayN<6> rxData;

    myI2cDevice.tx(txData);
    System::timeDelayMs(1);
    myI2cDevice.rx(rxData);
    
//    if (registerRead(REGISTER_DEVIATION_LSB, data) != I2c::ERROR_NONE)
//    {
//        return ERROR_COMMUNICATION;
//    }

    measurement.rawSample = (((uint16_t) rxData[0]) << 8) |
                            ((uint16_t) rxData[1]);
    measurement.sample = (float) measurement.rawSample * conversionFactor;

    return ERROR_NONE;
}

/*------------------------------------------------------------------------------
 * Private methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
SlaveDevice::Error CompassHmc6343::registerRead(const Register reg,
                                                ByteArray& byteArray)
{
    ByteArrayN<1> txByteArray;
    txByteArray.append((uint8_t) reg);

    return myI2cDevice.txRx(txByteArray, byteArray);
}

//------------------------------------------------------------------------------
SlaveDevice::Error CompassHmc6343::registerWrite(const Register reg,
                                                 const ByteArray& byteArray)
{    
    // Make large enough to handle multiple (6) sequential register writes.
    ByteArrayN<7> txByteArray;
    txByteArray.append((uint8_t) reg);

    txByteArray.append(byteArray);

    return myI2cDevice.tx(txByteArray);
}