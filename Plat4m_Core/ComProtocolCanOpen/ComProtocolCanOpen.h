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
/// @file ComProtocolCanOpen.h
/// @author Ben Minerd
/// @date 7/18/2024
/// @brief ComProtocolCanOpen class header file.
///

#ifndef PLAT4M_COM_PROTOCOL_CAN_OPEN_H
#define PLAT4M_COM_PROTOCOL_CAN_OPEN_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>

#include <Plat4m_Core/ComProtocol.h>
#include <Plat4m_Core/ErrorTemplate.h>
#include <Plat4m_Core/Callback.h>
#include <Plat4m_Core/ByteArray.h>
#include <Plat4m_Core/ByteArrayN.h>
#include <Plat4m_Core/List.h>
#include <Plat4m_Core/ComLink.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class ComProtocolCanOpen : public ComProtocol
{
public:

    //--------------------------------------------------------------------------
    // Public enumerations
    //--------------------------------------------------------------------------

    enum ErrorCode
    {
        ERROR_CODE_NONE = 0,
        ERROR_CODE_SDO_VALUE_SIZE_INVALID,
        ERROR_CODE_SDO_READ_FAILED,
        ERROR_CODE_SDO_WRITE_FAILED,
        ERROR_CODE_COMMUNICATION_TIMEOUT
    };

    using Error = ErrorTemplate<ErrorCode>;

    struct Config
    {
        std::uint8_t nodeId;
    };

    enum FunctionCode : std::uint8_t
    {
        FUNCTION_CODE_SDO_READ_REQUEST          = 0x40,
        FUNCTION_CODE_SDO_READ_RESPONSE_1_BYTE  = 0x4F,
        FUNCTION_CODE_SDO_READ_RESPONSE_2_BYTES = 0x4B,
        FUNCTION_CODE_SDO_READ_RESPONSE_4_BYTES = 0x43,
        FUNCTION_CODE_SDO_WRITE_REQUEST_1_BYTE  = 0x2F,
        FUNCTION_CODE_SDO_WRITE_REQUEST_2_BYTES = 0x2B,
        FUNCTION_CODE_SDO_WRITE_REQUEST_4_BYTES = 0x23,
        FUNCTION_CODE_SDO_WRITE_RESPONSE        = 0x60
    };

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    ComProtocolCanOpen(ComLink& comLink);

    //--------------------------------------------------------------------------
    // Public destructors
    //--------------------------------------------------------------------------

    virtual ~ComProtocolCanOpen();

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    Error setConfig(const Config& config);

    Config getConfig() const;

    Error setTpdoEnabled(const std::uint32_t number, const bool enabled);

    Error tpdoEnable(const std::uint32_t number);

    Error tpdoDisable(const std::uint32_t number);

    Error sdoRead(const std::uint16_t index,
                  const std::uint8_t subIndex,
                  const std::uint8_t valueSizeBytes,
                  std::uint32_t& value,
                  const bool supressTimeoutError = false);

    template <typename IntegerType>
    Error sdoRead(const std::uint16_t index,
                  const std::uint8_t subIndex,
                  IntegerType& value,
                  const bool supressTimeoutError = false);

    Error sdoWrite(const std::uint16_t index,
                   const std::uint8_t subIndex,
                   const std::uint8_t valueSizeBytes,
                   const std::uint32_t value);

    template <typename IntegerType>
    Error sdoWrite(const std::uint16_t index,
                   const std::uint8_t subIndex,
                   const IntegerType value);

protected:

    //--------------------------------------------------------------------------
    // Protected methods overridden for ComProtocol
    //--------------------------------------------------------------------------

    virtual ParseStatus subclassParseData(
                                        const ByteArray& receiveByteArray,
                                        ByteArray& transmitByteArray,
                                        Callback<>*& followUpCallback) override;

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    static constexpr std::uint16_t myTpdoBaseObjectIndex = 0x1800;

    static constexpr std::uint32_t myTpdo1BaseCanId = 0x180;

    static constexpr std::uint32_t myTpdo2BaseCanId = 0x280;

    static constexpr std::uint32_t myTpdo3BaseCanId = 0x380;

    static constexpr std::uint32_t myTpdo4BaseCanId = 0x480;

    static constexpr std::uint32_t mySdoRequestBaseCanId = 0x600;

    static constexpr std::uint32_t mySdoResponseBaseCanId = 0x580;

    static constexpr std::uint32_t myEmcyBaseCanId = 0x80;

    static constexpr std::uint32_t myBootBaseCanId = 0x700;

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    Config myConfig;

    ByteArrayN<256> myReceiveMessageByteArray;

    bool myIsWaitingForSdoResponse;

    Mutex& myMutex;

    TimeStamp sdoResponseTimeout;
};

}; // namespace Plat4m

#endif // PLAT4M_COM_PROTOCOL_CAN_OPEN_H
