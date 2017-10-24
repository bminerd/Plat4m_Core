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
 * @file Uart.cpp
 * @author Ben Minerd
 * @date 3/22/13
 * @brief Uart class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Uart.h>
#include <System.h>
#include <MutexLock.h>

/*------------------------------------------------------------------------------
 * Public virtual destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Uart::~Uart()
{
}

/*------------------------------------------------------------------------------
 * Public implemented methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
ComInterface::Error Uart::tx(const ByteArray& byteArray)
{
    if (!myIsEnabled)
    {
        return ComInterface::ERROR_NOT_ENABLED;
    }
    
    int size = byteArray.getSize();
    
    for (int i = 0; i < size; i++)
    {
        if (!(myTxBuffer.write(byteArray[i])))
        {
            return ComInterface::ERROR_TX_BUFFER_FULL;
        }
    }
    
    Error error = driverInterruptEnable(INTERRUPT_TX, true);
    
    return ComInterface::ERROR_NONE;
}

//------------------------------------------------------------------------------
unsigned int Uart::rxBytesAvailable()
{
    return myRxBuffer.count();
}

//------------------------------------------------------------------------------
ComInterface::Error Uart::getRxBytes(ByteArray& byteArray, unsigned int nBytes)
{
    unsigned int size;
    
    if (nBytes == 0)
    {
        size = myRxBuffer.count();
    }
    else
    {
        size = nBytes;
    }
    
    for (int i = 0; i < size; i++)
    {
        uint8_t byte;
        
        if (myRxBuffer.read(byte))
        {
            byteArray.append(byte);
        }
    }
    
    return ComInterface::ERROR_NONE;
}

/*------------------------------------------------------------------------------
 * Public methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Uart::Error Uart::enable(const bool enable)
{
    if (enable == myIsEnabled)
    {
        return ERROR_NONE;
    }

    Error error = driverEnable(enable);
    
    if (error == ERROR_NONE)
    {
        myIsEnabled = enable;
    }
    
    error = driverInterruptEnable(Uart::INTERRUPT_RX, enable);
    
    return error;
}

//------------------------------------------------------------------------------
Uart::Error Uart::isEnabled(bool& isEnabled)
{
    isEnabled = myIsEnabled;
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
Uart::Error Uart::configure(const Config& config)
{
    if (!myIsEnabled)
    {
        return ERROR_NOT_ENABLED;
    }
    
    Error error = driverConfigure(config);
    
    if (error == ERROR_NONE)
    {
        myConfig = config;
    }
    
    return error;
}

//------------------------------------------------------------------------------
void Uart::interruptHandler(const Interrupt interrupt)
{
    uint8_t byte;
    Error error;
    
    switch (interrupt)
    {
        case INTERRUPT_TX:
        {
            if (myTxBuffer.read(byte))
            {
                error = driverTx(byte);
            }
            else
            {
                error = driverInterruptEnable(INTERRUPT_TX, false);
            }
            
            break;
        }
        case INTERRUPT_RX:
        {
            error = driverRx(byte);
            
            // Rx callback?
            
            if (!myRxBuffer.write(byte))
            {
                // Error
            }
            
            break;
        }
    }
}

/*------------------------------------------------------------------------------
 * Protected constructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Uart::Uart() :
     myIsEnabled(false)
{
}