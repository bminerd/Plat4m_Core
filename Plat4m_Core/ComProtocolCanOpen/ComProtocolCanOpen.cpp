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
/// @file ComProtocolCanOpen.cpp
/// @author Ben Minerd
/// @date 7/18/2024
/// @brief ComProtocolCanOpen class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/ComProtocolCanOpen/ComProtocolCanOpen.h>
#include <Plat4m_Core/ByteArrayParser.h>
#include <Plat4m_Core/System.h>

using namespace Plat4m;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ComProtocolCanOpen::ComProtocolCanOpen(ComLink& comLink) :
    ComProtocol(1000, comLink),
    myReceiveMessageByteArray(),
    myIsWaitingForSdoResponse(false),
    myMutex(System::createMutex(comLink.getDataParsingThread())),
    sdoResponseTimeout(0, 20000000)
{
    ComLink::Config comLinkConfig;
    comLinkConfig.minParseByteCount = 10;

    comLink.setConfig(comLinkConfig);
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ComProtocolCanOpen::~ComProtocolCanOpen()
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ComProtocolCanOpen::Error ComProtocolCanOpen::setConfig(const Config& config)
{
    myConfig = config;

    return (Error(ERROR_CODE_NONE));
}

//------------------------------------------------------------------------------
ComProtocolCanOpen::Config ComProtocolCanOpen::getConfig() const
{
    return myConfig;
}

//------------------------------------------------------------------------------
ComProtocolCanOpen::Error ComProtocolCanOpen::setTpdoEnabled(
                                                     const std::uint32_t number,
                                                     const bool enabled)
{
    const std::uint16_t index = myTpdoBaseObjectIndex + (number - 1);
    const std::uint8_t subIndex = 0x01;

    std::uint32_t value = 0;

    Error error;

    error = sdoRead(index, subIndex, value);

    if (error.getCode() != ERROR_CODE_NONE)
    {
        return error;
    }

    setBitSet(value, 31, !enabled);

    error = sdoWrite(index, subIndex, 4, value);

    return error;
}

//------------------------------------------------------------------------------
ComProtocolCanOpen::Error ComProtocolCanOpen::tpdoEnable(
                                                     const std::uint32_t number)
{
    return (setTpdoEnabled(number, true));
}

//------------------------------------------------------------------------------
ComProtocolCanOpen::Error ComProtocolCanOpen::tpdoDisable(
                                                     const std::uint32_t number)
{
    return (setTpdoEnabled(number, false));
}

//------------------------------------------------------------------------------
ComProtocolCanOpen::Error ComProtocolCanOpen::sdoRead(
                                              const std::uint16_t index,
                                              const std::uint8_t subIndex,
                                              const std::uint8_t valueSizeBytes,
                                              std::uint32_t& value,
                                              const bool supressTimeoutError)
{
    // TODO: Rearchitect this into CanOpenSdoReadMessage

    ByteArrayN<12> data;
    data.append<std::uint32_t>(mySdoRequestBaseCanId + myConfig.nodeId, ENDIAN_LITTLE);
    data.append(FUNCTION_CODE_SDO_READ_REQUEST, ENDIAN_LITTLE);
    data.append(index, ENDIAN_LITTLE);
    data.append(subIndex, ENDIAN_LITTLE);
    data.append<std::uint8_t>(0x00, ENDIAN_LITTLE);
    data.append<std::uint8_t>(0x00, ENDIAN_LITTLE);
    data.append<std::uint8_t>(0x00, ENDIAN_LITTLE);
    data.append<std::uint8_t>(0x00, ENDIAN_LITTLE);

    getComLink().transmitBytes(data);

    TimeStamp timeoutTime = System::getTimeStamp() + sdoResponseTimeout;

    myMutex.lock();

    myIsWaitingForSdoResponse = true;

    while (myIsWaitingForSdoResponse)
    {
        myMutex.unlock();

        System::delayTimeMs(1);

        if (System::getTimeStamp() > timeoutTime)
        {
            myReceiveMessageByteArray.clear();

            if (supressTimeoutError)
            {
                return (Error(ERROR_CODE_COMMUNICATION_TIMEOUT));
            }
            else
            {
                return (PLAT4M_REPORT_ERROR(ComProtocolCanOpen::Error,
                                            ERROR_CODE_COMMUNICATION_TIMEOUT,
                                            ErrorBase::SEVERITY_HIGH,
                                            this));
            }
        }

        myMutex.lock();
    }

    myMutex.unlock();

    ByteArrayParser byteArrayParser(myReceiveMessageByteArray,
                                    ENDIAN_LITTLE,
                                    ByteArrayParser::PARSE_DIRECTION_FORWARD);

    std::uint8_t functionCode = 0;
    byteArrayParser.parse(functionCode);

    if ((functionCode != FUNCTION_CODE_SDO_READ_RESPONSE_1_BYTE)  &&
        (functionCode != FUNCTION_CODE_SDO_READ_RESPONSE_2_BYTES) &&
        (functionCode != FUNCTION_CODE_SDO_READ_RESPONSE_4_BYTES))
    {
        myReceiveMessageByteArray.clear();

        return (PLAT4M_REPORT_ERROR(ComProtocolCanOpen::Error,
                                    ERROR_CODE_SDO_READ_FAILED,
                                    ErrorBase::SEVERITY_HIGH,
                                    this));
    }

    std::uint16_t receivedIndex = 0;
    byteArrayParser.parse(receivedIndex);

    std::uint8_t receivedSubIndex = 0;
    byteArrayParser.parse(receivedSubIndex);

    byteArrayParser.parse(value);

    myReceiveMessageByteArray.clear();

    return (Error(ERROR_CODE_NONE));
}

//------------------------------------------------------------------------------
template <typename IntegerType>
ComProtocolCanOpen::Error ComProtocolCanOpen::sdoRead(
                                                 const std::uint16_t index,
                                                 const std::uint8_t subIndex,
                                                 IntegerType& value,
                                                 const bool supressTimeoutError)
{
    std::uint32_t value32 = 0;

    Error error = sdoRead(index,
                          subIndex,
                          sizeof(IntegerType),
                          value32,
                          supressTimeoutError);

    if (error.getCode() != ERROR_CODE_NONE)
    {
        return error;
    }

    value = static_cast<IntegerType>(value32);

    return error;
}

//------------------------------------------------------------------------------
ComProtocolCanOpen::Error ComProtocolCanOpen::sdoWrite(
                                              const std::uint16_t index,
                                              const std::uint8_t subIndex,
                                              const std::uint8_t valueSizeBytes,
                                              const std::uint32_t value)
{
    // TODO: Rearchitect this into CanOpenSdoWriteMessage

    ByteArrayN<12> data;

    data.append<std::uint32_t>(mySdoRequestBaseCanId + myConfig.nodeId,
                               ENDIAN_LITTLE);

    std::uint8_t functionCode = 0;

    switch (valueSizeBytes)
    {
        case 1:
        {
            functionCode = FUNCTION_CODE_SDO_WRITE_REQUEST_1_BYTE;

            break;
        }
        case 2:
        {
            functionCode = FUNCTION_CODE_SDO_WRITE_REQUEST_2_BYTES;

            break;
        }
        case 4:
        {
            functionCode = FUNCTION_CODE_SDO_WRITE_REQUEST_4_BYTES;

            break;
        }
        default:
        {
            return (PLAT4M_REPORT_ERROR(ComProtocolCanOpen::Error,
                                        ERROR_CODE_SDO_VALUE_SIZE_INVALID,
                                        ErrorBase::SEVERITY_HIGH,
                                        this));
        }
    }

    data.append(functionCode, ENDIAN_LITTLE);
    data.append(index,        ENDIAN_LITTLE);
    data.append(subIndex,     ENDIAN_LITTLE);
    data.append(value,        ENDIAN_LITTLE);

    getComLink().transmitBytes(data);

    TimeStamp timeoutTime = System::getTimeStamp() + sdoResponseTimeout;

    myMutex.lock();

    myIsWaitingForSdoResponse = true;

    while (myIsWaitingForSdoResponse)
    {
        myMutex.unlock();

        System::delayTimeMs(1);

        if (System::getTimeStamp() > timeoutTime)
        {
            myReceiveMessageByteArray.clear();

            return (PLAT4M_REPORT_ERROR(ComProtocolCanOpen::Error,
                                        ERROR_CODE_COMMUNICATION_TIMEOUT,
                                        ErrorBase::SEVERITY_HIGH,
                                        this));
        }

        myMutex.lock();
    }

    myMutex.unlock();

    ByteArrayParser byteArrayParser(myReceiveMessageByteArray,
                                    ENDIAN_LITTLE,
                                    ByteArrayParser::PARSE_DIRECTION_FORWARD);

    std::uint8_t receivedFunctionCode = 0;
    byteArrayParser.parse(receivedFunctionCode);

    if (receivedFunctionCode != FUNCTION_CODE_SDO_WRITE_RESPONSE)
    {
        myReceiveMessageByteArray.clear();

        return (PLAT4M_REPORT_ERROR(ComProtocolCanOpen::Error,
                                    ERROR_CODE_SDO_WRITE_FAILED,
                                    ErrorBase::SEVERITY_HIGH,
                                    this));
    }

    myReceiveMessageByteArray.clear();

    return (Error(ERROR_CODE_NONE));
}

//------------------------------------------------------------------------------
template <typename IntegerType>
ComProtocolCanOpen::Error ComProtocolCanOpen::sdoWrite(
                                                    const std::uint16_t index,
                                                    const std::uint8_t subIndex,
                                                    const IntegerType value)
{
    return (sdoWrite(index, subIndex, sizeof(IntegerType), value));
}

//------------------------------------------------------------------------------
// Protected virtual methods overridden for ComProtocol
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ComProtocol::ParseStatus ComProtocolCanOpen::subclassParseData(
                                              const ByteArray& receiveByteArray,
                                              ByteArray& transmitByteArray,
                                              Callback<>*& followUpCallback)
{
    ParseStatus parseStatus = PARSE_STATUS_NOT_A_FRAME;

    if (receiveByteArray.getSize() == 0)
    {
        return parseStatus;
    }

    parseStatus = PARSE_STATUS_MID_FRAME;

    if (receiveByteArray.getSize() < 4)
    {
        return parseStatus;
    }

    ByteArrayParser byteArrayParser(receiveByteArray,
                                    ENDIAN_LITTLE,
                                    ByteArrayParser::PARSE_DIRECTION_FORWARD);

    std::uint32_t canId = 0;
    byteArrayParser.parse(canId);

    // TODO: rearchitect this into array of CAN IDs

    switch (canId - myConfig.nodeId)
    {
        case myTpdo1BaseCanId: // Fall through
        case myTpdo2BaseCanId: // Fall through
        case myTpdo3BaseCanId:
        {
            if (receiveByteArray.getSize() < 10)
            {
                return parseStatus;
            }

            // Currently we don't handle these messages

            parseStatus = PARSE_STATUS_FOUND_FRAME;

            break;
        }
        case mySdoResponseBaseCanId:
        {
            if (receiveByteArray.getSize() < 12)
            {
                return parseStatus;
            }

            myReceiveMessageByteArray.append(receiveByteArray.subArray(4, 8));

            myMutex.lock();

            myIsWaitingForSdoResponse = false;

            myMutex.unlock();

            parseStatus = PARSE_STATUS_FOUND_FRAME;

            break;
        }
        case mySdoRequestBaseCanId:
        {
            if (receiveByteArray.getSize() < 12)
            {
                return parseStatus;
            }

            // Don't handle messages sent by us

            parseStatus = PARSE_STATUS_FOUND_FRAME;

            break;
        }
        case myEmcyBaseCanId:
        {
            if (receiveByteArray.getSize() < 12)
            {
                return parseStatus;
            }

            // Don't handle messages sent by us

            parseStatus = PARSE_STATUS_FOUND_FRAME;

            break;
        }
        default:
        {
            parseStatus = PARSE_STATUS_UNSUPPORTED_FRAME;

            break;
        }
    }

    return parseStatus;
}

//------------------------------------------------------------------------------
// Explicit template instantiations
//------------------------------------------------------------------------------

template ComProtocolCanOpen::Error Plat4m::ComProtocolCanOpen::sdoRead<std::uint32_t>(const std::uint16_t, const std::uint8_t, std::uint32_t&, const bool);
template ComProtocolCanOpen::Error Plat4m::ComProtocolCanOpen::sdoRead<std::int32_t>(const std::uint16_t, const std::uint8_t, std::int32_t&, const bool);
template ComProtocolCanOpen::Error Plat4m::ComProtocolCanOpen::sdoRead<std::uint16_t>(const std::uint16_t, const std::uint8_t, std::uint16_t&, const bool);
template ComProtocolCanOpen::Error Plat4m::ComProtocolCanOpen::sdoRead<std::int16_t>(const std::uint16_t, const std::uint8_t, std::int16_t&, const bool);
template ComProtocolCanOpen::Error Plat4m::ComProtocolCanOpen::sdoRead<std::uint8_t>(const std::uint16_t, const std::uint8_t, std::uint8_t&, const bool);
template ComProtocolCanOpen::Error Plat4m::ComProtocolCanOpen::sdoRead<std::int8_t>(const std::uint16_t, const std::uint8_t, std::int8_t&, const bool);

template ComProtocolCanOpen::Error Plat4m::ComProtocolCanOpen::sdoWrite<std::uint32_t>(const std::uint16_t, const std::uint8_t, std::uint32_t);
template ComProtocolCanOpen::Error Plat4m::ComProtocolCanOpen::sdoWrite<std::int32_t>(const std::uint16_t, const std::uint8_t, std::int32_t);
template ComProtocolCanOpen::Error Plat4m::ComProtocolCanOpen::sdoWrite<std::uint16_t>(const std::uint16_t, const std::uint8_t, std::uint16_t);
template ComProtocolCanOpen::Error Plat4m::ComProtocolCanOpen::sdoWrite<std::int16_t>(const std::uint16_t, const std::uint8_t, std::int16_t);
template ComProtocolCanOpen::Error Plat4m::ComProtocolCanOpen::sdoWrite<std::uint8_t>(const std::uint16_t, const std::uint8_t, std::uint8_t);
template ComProtocolCanOpen::Error Plat4m::ComProtocolCanOpen::sdoWrite<std::int8_t>(const std::uint16_t, const std::uint8_t, std::int8_t);
