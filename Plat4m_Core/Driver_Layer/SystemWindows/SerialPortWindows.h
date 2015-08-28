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
 * Copyright (c) 2015 Benjamin Minerd
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
 * @file SerialPortWindows.h
 * @author Ben Minerd
 * @date 6/3/15
 * @brief SerialPortWindows class.
 */

#ifndef _SERIAL_PORT_WINDOWS_H_
#define _SERIAL_PORT_WINDOWS_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <ErrorTemplate.h>
#include <SerialPort.h>
#include <windows.h>

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

class SerialPortWindows : public SerialPort
{
public:
    
    /*--------------------------------------------------------------------------
     * Public enumerations
     *------------------------------------------------------------------------*/

    /**
     * @brief Enumeration of UART driver IDs.
     */
    enum Id
    {
        ID_1 = 0,
        ID_2,
        ID_3,
        ID_4,
        ID_5,
        ID_6
    };

    /*--------------------------------------------------------------------------
     * Public static methods
     *------------------------------------------------------------------------*/
    
    static SerialPortWindows& get(const char* comPort);
    
private:
    
    /*--------------------------------------------------------------------------
     * Private constructors and destructors
     *------------------------------------------------------------------------*/
    
    SerialPortWindows(const char* comPort);

    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/

    HANDLE mySerialHandle;

    /*--------------------------------------------------------------------------
	 * Private implemented methods from Module
	 *------------------------------------------------------------------------*/

	Module::Error driverEnable(const bool enable);

    /*--------------------------------------------------------------------------
     * Private implemented methods from SerialPort
     *------------------------------------------------------------------------*/
    
    SerialPort::Error driverConfigure(const Config& config);
    
    SerialPort::Error driverTx(const ByteArray& byteArray);
    
    unsigned int driverRxBytesAvailable();

    SerialPort::Error driverGetRxBytes(ByteArray& byteArray,
    								   const unsigned int nBytes);
};

#endif // _SERIAL_PORT_WINDOWS_H_
