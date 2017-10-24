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
 * @file SerialPortWindows.cpp
 * @author Ben Minerd
 * @date 5/14/13
 * @brief SerialPortWindows class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <SerialPortWindows.h>

using namespace Plat4m;

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

static uint8_t wordBitsMap[] =
{
	8, /// SerialPort::WORD_BITS_8
	9  /// SerialPort::WORD_BITS_9
};

static uint8_t stopBitsMap[] =
{
	1, /// SerialPort::STOP_BITS_1
	2  /// SerialPort::STOP_BITS_2
};

static uint8_t parityMap[] =
{
	0, /// SerialPort::PARITY_NONE
	2, /// SerialPort::PARITY_EVEN
	1, /// SerialPort::PARITY_ODD
	3, /// SerialPort::PARITY_MARK
	4, /// SerialPort::PARITY_SPACE
};

static SerialPortWindows* objectMap[1]; // TODO Update

/*------------------------------------------------------------------------------
 * Public static methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
SerialPortWindows& SerialPortWindows::get(const char* comPort)
{
    if (IS_NULL_POINTER(objectMap[0]))
    {
        objectMap[0] = new SerialPortWindows(comPort);
    }
    else
    {
        // Error?
    }
    
    return *(objectMap[0]);
}

/*------------------------------------------------------------------------------
 * Private constructors and destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
SerialPortWindows::SerialPortWindows(const char* comPort) :
    SerialPort(comPort),
	mySerialHandle(NULL_POINTER)
{
}

/*------------------------------------------------------------------------------
 * Private implemented methods from Module
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Module::Error SerialPortWindows::driverEnable(const bool enable)
{
	if (enable)
	{
		if (isNullPointer(mySerialHandle))
		{
			mySerialHandle = CreateFile(getComPort(),
										GENERIC_READ | GENERIC_WRITE,
										0,
										0,
										OPEN_EXISTING,
										FILE_ATTRIBUTE_NORMAL,
										0);

			if (mySerialHandle == INVALID_HANDLE_VALUE)
			{
				if (GetLastError() == ERROR_FILE_NOT_FOUND)
				{
					return Module::Error(ERROR_CODE_ENABLE);
				}
			}
		}
	}
	else
	{
		if (isValidPointer(mySerialHandle))
		{
			CloseHandle(mySerialHandle);
		}
	}

	return Module::Error(Module::ERROR_CODE_NONE);
}

/*------------------------------------------------------------------------------
 * Private implemented methods from SerialPort
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
SerialPort::Error SerialPortWindows::driverConfigure(const Config& config)
{
	DCB serialParametersDcb;

	serialParametersDcb.DCBlength = sizeof(serialParametersDcb);

	if (!GetCommState(mySerialHandle, &serialParametersDcb))
	{
		// Error
	}

	serialParametersDcb.BaudRate = config.baudRate;
	serialParametersDcb.ByteSize = wordBitsMap[config.wordBits];
	serialParametersDcb.StopBits = stopBitsMap[config.stopBits];
	serialParametersDcb.Parity   = parityMap[config.parity];

	if (!SetCommState(mySerialHandle, &serialParametersDcb))
	{
		return Error(ERROR_CODE_CONFIGURE_FAILED);
	}

	// TODO Make this part of Config?
	COMMTIMEOUTS timeouts = {0};
	timeouts.ReadIntervalTimeout 		 = 50;
	timeouts.ReadTotalTimeoutConstant    = 50;
	timeouts.ReadTotalTimeoutMultiplier  = 10;
	timeouts.WriteTotalTimeoutConstant   = 50;
	timeouts.WriteTotalTimeoutMultiplier = 10;

	if (!SetCommTimeouts(mySerialHandle, &timeouts))
	{
		return Error(ERROR_CODE_CONFIGURE_FAILED);
	}

	return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
SerialPort::Error SerialPortWindows::driverTx(const ByteArray& byteArray)
{
	DWORD bytesWritten = 0;

	if (!WriteFile(mySerialHandle,
				   byteArray.getDataConst(),
				   byteArray.getSize(),
				   &bytesWritten,
				   NULL))
	{
		return Error(ERROR_CODE_TX_FAILED);
	}

	return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
unsigned int SerialPortWindows::driverRxBytesAvailable()
{
	DWORD dwErrors;
	COMSTAT comStat;

	unsigned int nBytes = ClearCommError(mySerialHandle, &dwErrors, &comStat);

	return nBytes;
}

//------------------------------------------------------------------------------
SerialPort::Error SerialPortWindows::driverGetRxBytes(ByteArray& byteArray,
													  const unsigned int nBytes)
{
	DWORD bytesReceived = 0;

	// TODO Fix this so it's non-blocking (overlapped I/O?)...

	if (!SetCommMask(mySerialHandle, EV_RXCHAR))
	{
	    // Error?
	}

//	DWORD dwCommEvent;
//
//	if (!WaitCommEvent(mySerialHandle, &dwCommEvent, NULL))
//	{
//	    return Error(ERROR_CODE_RX_FAILED);
//	}

	if (nBytes == 0)
	{
//	    byteArray.setSize(driverRxBytesAvailable());

		if (!ReadFile(mySerialHandle,
					  byteArray.getData(),
					  byteArray.getSize(),
					  &bytesReceived,
					  NULL))
		{
			return Error(ERROR_CODE_RX_FAILED);
		}
	}
	else
	{
		if (!ReadFile(mySerialHandle,
					  byteArray.getData(),
					  nBytes,
					  &bytesReceived,
					  NULL))
		{
			return Error(ERROR_CODE_RX_FAILED);
		}
	}

	byteArray.setSize(bytesReceived);

	return Error(ERROR_CODE_NONE);
}
