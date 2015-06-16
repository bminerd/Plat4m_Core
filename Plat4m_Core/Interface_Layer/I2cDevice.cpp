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
 * @file I2cDevice.cpp
 * @author Ben Minerd
 * @date 3/26/13
 * @brief I2cDevice class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <I2cDevice.h>

/*------------------------------------------------------------------------------
 * Public constructors and destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
I2cDevice::I2cDevice(const uint8_t address, I2c& i2c) :
    SlaveDevice(),
    myAddress(address),
    myI2c(i2c)
{
}

/*------------------------------------------------------------------------------
 * Public methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
uint8_t I2cDevice::getAddress()
{
    return myAddress;
}

/*------------------------------------------------------------------------------
 * Private implemented methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
SlaveDevice::Error I2cDevice::driverEnable(const bool enable)
{
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
SlaveDevice::Error I2cDevice::driverTx(const ByteArray& byteArray,
                                       const bool waitUntilDone,
                                       const uint32_t timeoutMs)
{
    ByteArray emptyByteArray;
    I2c::TransferMode transferMode;
    
    if (waitUntilDone)
    {
        transferMode = I2c::TRANSFER_MODE_TX_WAIT;
    }
    else
    {
        transferMode = I2c::TRANSFER_MODE_TX;
    }
    
    if (myI2c.masterTransfer(transferMode,
                             myAddress,
                             byteArray,
                             emptyByteArray,
                             timeoutMs)     != I2c::ERROR_NONE)
    {
        return ERROR_COMMUNICATION;
    }
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
SlaveDevice::Error I2cDevice::driverRx(ByteArray& byteArray,
                                       const uint32_t timeoutMs)
{
    ByteArray emptyByteArray;

    if (myI2c.masterTransfer(I2c::TRANSFER_MODE_RX,
                             myAddress,
                             emptyByteArray,
                             byteArray,
                             timeoutMs)             != I2c::ERROR_NONE)
    {
        return ERROR_COMMUNICATION;
    }
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
SlaveDevice::Error I2cDevice::driverTxRx(const ByteArray& txByteArray,
                                         ByteArray& rxByteArray,
                                         const uint32_t timeoutMs)
{
    if (myI2c.masterTransfer(I2c::TRANSFER_MODE_TX_RX,
                             myAddress,
                             txByteArray,
                             rxByteArray,
                             timeoutMs)                 != I2c::ERROR_NONE)
    {
        return ERROR_COMMUNICATION;
    }
    
    return ERROR_NONE;
}