//------------------------------------------------------------------------------
//       _______    __                           ___
//      ||  ___ \  || |             __          //  |
//      || |  || | || |   _______  || |__      //   |    _____  ___
//      || |__|| | || |  // ___  | ||  __|    // _  |   ||  _ \/ _ \
//      ||  ____/  || | || |  || | || |      // /|| |   || |\\  /\\ \
//      || |       || | || |__|| | || |     // /_|| |_  || | || | || |
//      || |       || |  \\____  | || |__  //_____   _| || | || | || |
//      ||_|       ||_|       ||_|  \\___|       ||_|   ||_| ||_| ||_|
//
//
// The MIT License (MIT)
//
// Copyright (c) 2015 Benjamin Minerd
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------------

///
/// @file SerialPortWindows.cpp
/// @author Ben Minerd
/// @date 6/3/2015
/// @brief SerialPortWindows class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/SystemWindows/SerialPortWindows.h>
#include <Plat4m_Core/System.h>
#include <Plat4m_Core/CallbackMethod.h>
#include <Plat4m_Core/MutexLock.h>

using Plat4m::SerialPortWindows;
using Plat4m::SerialPort;
using Plat4m::Module;
using Plat4m::ComInterface;

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

const uint8_t SerialPortWindows::myWordBitsMap[] =
{
	8, /// SerialPort::WORD_BITS_8
	9  /// SerialPort::WORD_BITS_9
};

const uint8_t SerialPortWindows::myStopBitsMap[] =
{
	1, /// SerialPort::STOP_BITS_1
	2  /// SerialPort::STOP_BITS_2
};

const uint8_t SerialPortWindows::myParityMap[] =
{
	0, /// SerialPort::PARITY_NONE
	2, /// SerialPort::PARITY_EVEN
	1, /// SerialPort::PARITY_ODD
	3, /// SerialPort::PARITY_MARK
	4, /// SerialPort::PARITY_SPACE
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
SerialPortWindows::SerialPortWindows(const char* comPort) :
    SerialPort(comPort),
	mySerialHandle(0),
	myReceiveThread(System::createThread(
	          createCallback(this, &SerialPortWindows::receiveThreadCallback))),
	myMutex(System::createMutex(myReceiveThread))
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
SerialPortWindows::~SerialPortWindows()
{
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error SerialPortWindows::driverSetEnabled(const bool enabled)
{
	if (enabled)
	{
		if (isNullPointer(mySerialHandle))
		{
			mySerialHandle = CreateFile(getName(),
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
					return Module::Error(Module::ERROR_CODE_ENABLE_FAILED);
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

		myReceiveThread.setEnabled(false);
	}

	return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods implemented from SerialPort
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
SerialPort::Error SerialPortWindows::driverSetConfig(const Config& config)
{
	DCB serialParametersDcb;

	serialParametersDcb.DCBlength = sizeof(serialParametersDcb);

	if (!GetCommState(mySerialHandle, &serialParametersDcb))
	{
		// Error
	}

	serialParametersDcb.BaudRate = config.baudRate;
	serialParametersDcb.ByteSize = myWordBitsMap[config.wordBits];
	serialParametersDcb.StopBits = myStopBitsMap[config.stopBits];
	serialParametersDcb.Parity   = myParityMap[config.parityBit];

	if (!SetCommState(mySerialHandle, &serialParametersDcb))
	{
		return Error(ERROR_CODE_SET_CONFIG_FAILED);
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
		return Error(ERROR_CODE_SET_CONFIG_FAILED);
	}

	myReceiveThread.setEnabled(true);

	return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
ComInterface::Error SerialPortWindows::driverTransmitBytes(
                                                     const ByteArray& byteArray,
                                                     const bool waitUntilDone)
{
	DWORD bytesWritten = 0;

	if (!WriteFile(mySerialHandle,
				   byteArray.getItems(),
				   byteArray.getSize(),
				   &bytesWritten,
				   NULL))
	{
		return ComInterface::Error(
		                         ComInterface::ERROR_CODE_TRANSMIT_BUFFER_FULL);
	}

	return ComInterface::Error(ComInterface::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
uint32_t SerialPortWindows::driverGetReceivedBytesCount()
{
    MutexLock mutexLock(myMutex);

    return (getReceiveBuffer()->count());
}

//------------------------------------------------------------------------------
ComInterface::Error SerialPortWindows::driverGetReceivedBytes(
                                                          ByteArray& byteArray,
                                                          const uint32_t nBytes)
{
    uint8_t byte;
    uint32_t nBytesToRead;

    if (nBytes == 0)
    {
        nBytesToRead = driverGetReceivedBytesCount();
    }
    else
    {
        nBytesToRead = nBytes;
    }

    while (nBytesToRead--)
    {
        MutexLock mutexLock(myMutex);

        if (getReceiveBuffer()->read(byte))
        {
            byteArray.append(byte);
        }
    }

	return ComInterface::Error(ComInterface::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void SerialPortWindows::receiveThreadCallback()
{
    DWORD dwErrors;
    COMSTAT comStat;

    unsigned int nBytes = ClearCommError(mySerialHandle, &dwErrors, &comStat);

    while (nBytes--)
    {
        uint8_t byte;
        DWORD bytesReceived;

        if (!ReadFile(mySerialHandle,
                      &byte,
                      1,
                      &bytesReceived,
                      NULL))
        {

        }

        MutexLock mutexLock(myMutex);

        if (!(getReceiveBuffer()->write(byte)))
        {
            // Buffer overflow
        }

        mutexLock.setLocked(false);

        byteReceived(byte);
    }
}
