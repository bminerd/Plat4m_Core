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
 * @file SpiDevice.h
 * @author Ben Minerd
 * @date 4/4/13
 * @brief SpiDevice class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <SpiDevice.h>

/*------------------------------------------------------------------------------
 * Public constructors and destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
SpiDevice::SpiDevice(GpioPin* chipSelectGpioPin, Spi& spi) :
    SlaveDevice(),
    myChipSelectGpioPin(chipSelectGpioPin),
    mySpi(spi)
{
}

/*------------------------------------------------------------------------------
 * Private implemented methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
SlaveDevice::Error SpiDevice::driverEnable(const bool enable)
{
    if (IS_VALID_POINTER(myChipSelectGpioPin))
    {
        myChipSelectGpioPin->enable(enable);
    
        if (enable)
        {
            GpioPin::Config gpioConfig;
            gpioConfig.mode     = GpioPin::MODE_DIGITAL_OUTPUT;
            gpioConfig.resistor = GpioPin::RESISTOR_PULL_UP;
            
            myChipSelectGpioPin->configure(gpioConfig);
            myChipSelectGpioPin->setLevel(GpioPin::LEVEL_HIGH);
        }
    }
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
SlaveDevice::Error SpiDevice::driverTx(const ByteArray& byteArray,
                                       const bool waitUntilDone,
                                       const uint32_t timeoutMs)
{
    ByteArray emptyByteArray;

    Spi::TransferMode transferMode;
    
    if (waitUntilDone)
    {
        transferMode = Spi::TRANSFER_MODE_TX_WAIT;
    }
    else
    {
        transferMode = Spi::TRANSFER_MODE_TX;
    }
    
    if (mySpi.masterTransfer(transferMode,
                             myChipSelectGpioPin,
                             byteArray,
                             emptyByteArray,
                             timeoutMs)             != Spi::ERROR_NONE)
    {
        return ERROR_COMMUNICATION;
    }
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
SlaveDevice::Error SpiDevice::driverRx(ByteArray& byteArray,
                                       const uint32_t timeoutMs)
{
    ByteArray emptyByteArray;

    if (mySpi.masterTransfer(Spi::TRANSFER_MODE_RX,
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
SlaveDevice::Error SpiDevice::driverTxRx(const ByteArray& txByteArray,
                                         ByteArray& rxByteArray,
                                         const uint32_t timeoutMs)
{
    if (mySpi.masterTransfer(Spi::TRANSFER_MODE_TX_RX_SEQUENTIAL,
                             myChipSelectGpioPin,
                             txByteArray,
                             rxByteArray,
                             timeoutMs)                 != Spi::ERROR_NONE)
    {
        return ERROR_COMMUNICATION;
    }
    
    return ERROR_NONE;
}