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
// Copyright (c) 2018-2023 Benjamin Minerd
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
/// @file SerialPort.cpp
/// @author Ben Minerd
/// @date 3/22/2013
/// @brief SerialPort class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/SerialPort.h>
#include <Plat4m_Core/System.h>

using Plat4m::SerialPort;
using Plat4m::ComInterface;

//------------------------------------------------------------------------------
// Public virtual methods overridden for ComInterface
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ComInterface::Error SerialPort::transmitBytes(const ByteArray& byteArray,
                                              const bool waitUntilDone)
{
    if (!isEnabled())
    {
        return ComInterface::Error(ComInterface::ERROR_CODE_NOT_ENABLED);
    }

    return driverTransmitBytes(byteArray, waitUntilDone);
}

//------------------------------------------------------------------------------
uint32_t SerialPort::getReceivedBytesCount()
{
    return driverGetReceivedBytesCount();
}

//------------------------------------------------------------------------------
ComInterface::Error SerialPort::getReceivedBytes(ByteArray& byteArray,
                                                 const uint32_t nBytes)
{
    return driverGetReceivedBytes(byteArray, nBytes);
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
const char* SerialPort::getName() const
{
    return myName;
}

//------------------------------------------------------------------------------
SerialPort::Config SerialPort::getConfig() const
{
    return myConfig;
}

//------------------------------------------------------------------------------
SerialPort::Error SerialPort::setConfig(const Config& config)
{
    if (!isEnabled())
    {
        return Error(ERROR_CODE_NOT_ENABLED);
    }

    Error error = driverSetConfig(config);

    if (error == ERROR_CODE_NONE)
    {
        myConfig = config;
    }

    return error;
}

//------------------------------------------------------------------------------
// Protected constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
SerialPort::SerialPort(const char* name) :
    ComInterface(),
    myName(name),
    myConfig()
{
}

//------------------------------------------------------------------------------
// Protected virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
SerialPort::~SerialPort()
{
}
