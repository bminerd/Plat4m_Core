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
// Copyright (c) 2024 Benjamin Minerd
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
/// @file CanPcan.h
/// @author Ben Minerd
/// @date 7/25/2024
/// @brief CanPcan class header file.
///

#ifndef PLAT4M_CAN_PCAN_H
#define PLAT4M_CAN_PCAN_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>

#include <windows.h>

#include <Plat4m_Core/Can.h>
#include <Plat4m_Core/ErrorTemplate.h>
#include <Plat4m_Core/Thread.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class CanPcan : public Can
{
public:

    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------

    enum ErrorCode
    {
        ERROR_CODE_NONE = 0,
        ERROR_CODE_CHANNEL_INITIALIZATION_FAILED
    };

    using Error = ErrorTemplate<ErrorCode>;

    struct CanPcanConfig
    {
        std::uint16_t rxFifoSize;
        std::uint16_t rxThreshold;
        std::uint16_t txFifoSize;
        std::uint16_t txThreshold;
    };

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    CanPcan();

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~CanPcan();

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    Error setCanPcanConfig(const CanPcanConfig& config);

protected:

    //--------------------------------------------------------------------------
    // Protected virtual methods overridden for Module
    //--------------------------------------------------------------------------

    virtual Module::Error subclassSetEnabled(const bool enabled) override;

    //--------------------------------------------------------------------------
    // Protected virtual methods overridden for Can
    //--------------------------------------------------------------------------

    virtual Can::Error subclassSetConfig(const Config& config) override;

    virtual Can::Error subclassSendMessage(const Message& message) override;

    virtual Can::Error subclassAddAcceptanceFilter(
                                             const IdType idType,
                                             const std::uint32_t filter,
                                             const std::uint32_t mask) override;

    virtual Can::Error subclassClearReceivedMessages() override;

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    HANDLE myPortHandle;

    HANDLE myChannelHandle;

    Thread& myReceiveThread;

    bool myIsInitialConfigurationSet;

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------

    void receiveThreadCallback();
};

}; // namespace Plat4m

#endif // PLAT4M_CAN_PCAN_H
