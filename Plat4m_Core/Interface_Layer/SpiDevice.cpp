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

using Plat4m::SpiDevice;
using Plat4m::SlaveDevice;
using Plat4m::Module;

/*------------------------------------------------------------------------------
 * Public constructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
SpiDevice::SpiDevice(GpioPin* chipSelectGpioPin, Spi& spi) :
    SlaveDevice(),
    myChipSelectGpioPin(chipSelectGpioPin),
    mySpi(spi)
{
}

/*------------------------------------------------------------------------------
 * Public destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
SpiDevice::~SpiDevice()
{
}

/*------------------------------------------------------------------------------
 * Private implemented methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Module::Error SpiDevice::driverEnable(const bool enable)
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
    
    return Module::Error(Module::ERROR_CODE_NONE);
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
    
    Spi::Error error = mySpi.masterTransfer(transferMode,
                                            myChipSelectGpioPin,
                                            byteArray,
                                            emptyByteArray,
                                            timeoutMs);

    if (error.getCode() != Spi::ERROR_CODE_NONE)
    {
        return SlaveDevice::Error(SlaveDevice::ERROR_CODE_COMMUNICATION);
    }
    
    return SlaveDevice::Error(SlaveDevice::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
SlaveDevice::Error SpiDevice::driverRx(ByteArray& byteArray,
                                       const uint32_t timeoutMs)
{
    ByteArray emptyByteArray;

    Spi::Error error = mySpi.masterTransfer(Spi::TRANSFER_MODE_RX,
                                            myChipSelectGpioPin,
                                            emptyByteArray,
                                            byteArray,
                                            timeoutMs);

    if (error.getCode() != Spi::ERROR_CODE_NONE)
    {
        return Error(SlaveDevice::ERROR_CODE_COMMUNICATION);
    }
    
    return Error(SlaveDevice::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
SlaveDevice::Error SpiDevice::driverTxRx(const ByteArray& txByteArray,
                                         ByteArray& rxByteArray,
                                         const uint32_t timeoutMs)
{
    Spi::Error error = mySpi.masterTransfer(Spi::TRANSFER_MODE_TX_RX_SEQUENTIAL,
                                            myChipSelectGpioPin,
                                            txByteArray,
                                            rxByteArray,
                                            timeoutMs);

    if (error.getCode() != Spi::ERROR_CODE_NONE)
    {
        return SlaveDevice::Error(SlaveDevice::ERROR_CODE_COMMUNICATION);
    }
    
    return SlaveDevice::Error(ERROR_CODE_NONE);
}
