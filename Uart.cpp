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
// Copyright (c) 2013 Benjamin Minerd
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
/// @file Uart.cpp
/// @author Ben Minerd
/// @date 3/22/2013
/// @brief Uart class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Uart.h>
#include <Plat4m_Core/System.h>

using Plat4m::Uart;
using Plat4m::ComInterface;

//------------------------------------------------------------------------------
// Public methods implemented from ComInterface
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ComInterface::Error Uart::transmitBytes(const ByteArray& byteArray,
                                        const bool waitUntilDone)
{
    if (!isEnabled())
    {
        return ComInterface::Error(ComInterface::ERROR_CODE_NOT_ENABLED);
    }

    return driverTransmitBytes(byteArray, waitUntilDone);
}

//------------------------------------------------------------------------------
uint32_t Uart::getReceivedBytesCount()
{
    return driverGetReceivedBytesCount();
}

//------------------------------------------------------------------------------
ComInterface::Error Uart::getReceivedBytes(ByteArray& byteArray,
                                           const uint32_t nBytes)
{
    return driverGetReceivedBytes(byteArray, nBytes);
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Uart::Config Uart::getConfig() const
{
    return myConfig;
}

//------------------------------------------------------------------------------
Uart::Error Uart::setConfig(const Config& config)
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
Uart::Uart() :
    ComInterface()
{
}

//------------------------------------------------------------------------------
// Protected virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Uart::~Uart()
{
}
