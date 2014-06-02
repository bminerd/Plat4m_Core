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
 * Copyright (c) 2014 Benjamin Minerd
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
 * @file SlaveDevice.h
 * @author Ben Minerd
 * @date 4/24/14
 * @brief SlaveDevice class.
 */

#ifndef _SLAVE_DEVICE_H_
#define _SLAVE_DEVICE_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <ByteArray.h>

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

class SlaveDevice
{
public:
    
    /*--------------------------------------------------------------------------
     * Public enumerations
     *------------------------------------------------------------------------*/
    
    enum Error
    {
        ERROR_NONE,
        ERROR_NOT_ENABLED,
        ERROR_COMMUNICATION
    };
    
    /*--------------------------------------------------------------------------
     * Public methods
     *------------------------------------------------------------------------*/
    
    Error enable(const bool enable);
    
    /**
     * @brief Transmits the given byte array to this Slave device.
     * @param data Byte array to transmit.
     * @return Slave error.
     */
    Error tx(const ByteArray& byteArray,
             const bool waitUntilDone = true,
             const uint32_t timeoutMs = 100);

    /**
     * @brief Receives data from this Slave device.
     * @param data Byte array that was received.
     * @return Slave error.
     */
    Error rx(ByteArray& byteArray, const uint32_t timeoutMs = 100);

    /**
     * @brief Transmits and receives data to/from this Slave device.
     * @param device Slave device to access.
     * @param txData Byte array to transmit.
     * @param rxData Byte array that was received.
     * @return Slave error.
     */
    Error txRx(const ByteArray& txByteArray,
               ByteArray& rxByteArray,
               const uint32_t timeoutMs = 100);
    
protected:

    /*--------------------------------------------------------------------------
     * Protected constructors and destructors
     *------------------------------------------------------------------------*/
    
    SlaveDevice();
    
private:
    
    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/
    
    bool myIsEnabled;
    
    /*--------------------------------------------------------------------------
     * Private pure virtual methods
     *------------------------------------------------------------------------*/
    
    virtual Error driverEnable(const bool enable) = 0;
    
    virtual Error driverTx(const ByteArray& byteArray,
                           const bool waitUntilDone,
                           const uint32_t timeoutMs) = 0;
    
    virtual Error driverRx(ByteArray& byteArray, const uint32_t timeoutMs) = 0;
    
    virtual Error driverTxRx(const ByteArray& txByteArray,
                             ByteArray& rxByteArray,
                             const uint32_t timeoutMs) = 0;
};

#endif // _SLAVE_DEVICE_H_