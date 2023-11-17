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
/// @file SerialPort.h
/// @author Ben Minerd
/// @date 2/19/2018
/// @brief SerialPort class header file.
///

#ifndef PLAT4M_SERIAL_PORT_H
#define PLAT4M_SERIAL_PORT_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <stdint.h>

#include <Plat4m_Core/ComInterface.h>
#include <Plat4m_Core/ErrorTemplate.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class SerialPort : public ComInterface
{
public:

    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------

    enum ErrorCode
    {
        ERROR_CODE_NONE,
        ERROR_CODE_NOT_ENABLED,
        ERROR_CODE_ENABLE_FAILED,
        ERROR_CODE_SET_CONFIG_FAILED,
        ERROR_CODE_TRANSMIT_BUFFER_FULL,
        ERROR_CODE_RECEIVE_FAILED
    };

    enum WordBits
    {
        WORD_BITS_8 = 0,
        WORD_BITS_9
    };

    enum StopBits
    {
        STOP_BITS_1 = 0,
        STOP_BITS_2
    };

    enum ParityBit
    {
        PARITY_BIT_NONE = 0,
        PARITY_BIT_EVEN,
        PARITY_BIT_ODD
    };

    enum HardwareFlowControl
    {
        HARDWARE_FLOW_CONTROL_NONE = 0
    };

    typedef ErrorTemplate<ErrorCode> Error;
    
    //--------------------------------------------------------------------------
    // Public structures
    //--------------------------------------------------------------------------

    struct Config
    {
        uint32_t baudRate;
        WordBits wordBits;
        StopBits stopBits;
        ParityBit parityBit;
        HardwareFlowControl hardwareFlowControl;
    };

    //--------------------------------------------------------------------------
    // Public virtual methods overridden for ComInterface
    //--------------------------------------------------------------------------

    virtual ComInterface::Error transmitBytes(
                                     const ByteArray& byteArray,
                                     const bool waitUntilDone = false) override;

    virtual uint32_t getReceivedBytesCount() override;

    virtual ComInterface::Error getReceivedBytes(
                                            ByteArray& byteArray,
                                            const uint32_t nBytes = 0) override;

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    Config getConfig() const;

    Error setConfig(const Config& config);

    const char* getName() const;

protected:

    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------

    SerialPort(const char* name);

    //--------------------------------------------------------------------------
    // Protected virtual destructors
    //--------------------------------------------------------------------------

    virtual ~SerialPort();

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    const char* myName;

    Config myConfig;

    //--------------------------------------------------------------------------
    // Private pure virtual methods
    //--------------------------------------------------------------------------

    virtual Error driverSetConfig(const Config& config) = 0;

    virtual ComInterface::Error driverTransmitBytes(
                                                  const ByteArray& byteArray,
                                                  const bool waitUntilDone) = 0;

    virtual uint32_t driverGetReceivedBytesCount() = 0;

    virtual ComInterface::Error driverGetReceivedBytes(
                                                     ByteArray& byteArray,
                                                     const uint32_t nBytes) = 0;
};

}; // namespace plat4m

#endif // PLAT4M_SERIAL_PORT_H
