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
 * @file Spi4WireDevice.h
 * @author Ben Minerd
 * @date 4/24/14
 * @brief Spi4WireDevice class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Spi4WireDevice.h>

/*------------------------------------------------------------------------------
 * Public constructors and destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Spi4WireDevice::Spi4WireDevice(GpioPin& chipSelectGpioPin,
                               Spi& spi,
                               GpioPin& dataCommandGpioPin) :
    SlaveDevice(),
    mySpiDevice(&chipSelectGpioPin, spi),
    myDataCommandGpioPin(dataCommandGpioPin)
{
}

/*------------------------------------------------------------------------------
 * Private implemented methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
SlaveDevice::Error Spi4WireDevice::driverEnable(const bool enable)
{
    if (enable)
    {
        GpioPin::Config gpioConfig;
        gpioConfig.mode     = GpioPin::MODE_DIGITAL_OUTPUT;
        gpioConfig.resistor = GpioPin::RESISTOR_NONE;
        
        myChipSelectGpioPin->configure(gpioConfig);
        myChipSelectGpioPin->setLevel(GpioPin::LEVEL_LOW);
    }
    
    mySpiDevice.enable(enable);
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
SlaveDevice::Error Spi4WireDevice::driverTx(const ByteArray& byteArray,
                                            const bool waitUntilDone,
                                            const uint32_t timeoutMs)
{
    
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
SlaveDevice::Error Spi4WireDevice::driverRx(ByteArray& byteArray,
                                       const uint32_t timeoutMs)
{
    ByteArray emptyByteArray;

    if (mySpiDevice.masterTransfer(Spi::TRANSFER_MODE_RX,
                             myChipSelectGpioPin,
                             emptyByteArray,
                             byteArray,
                             timeoutMs)             != Spi::ERROR_NONE)
    {
        return ERROR_COMMUNICATION;
    }
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
SlaveDevice::Error Spi4WireDevice::txRx(const ByteArray& txByteArray,
                                   ByteArray& rxByteArray,
                                   const uint32_t timeoutMs)
{
    if (mySpi.masterTransfer(Spi::TRANSFER_MODE_TX_RX,
                             myChipSelectGpioPin,
                             txByteArray,
                             rxByteArray,
                             timeoutMs)                 != Spi::ERROR_NONE)
    {
        return ERROR_COMMUNICATION;
    }
    
    return ERROR_NONE;
}