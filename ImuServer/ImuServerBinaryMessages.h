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
// Copyright (c) 2017 Benjamin Minerd
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
/// @file ImuServerBinaryMessages.h
/// @author Ben Minerd
/// @date 4/18/2017
/// @brief ImuServerBinaryMessages namespace header file.
///

#ifndef PLAT4M_IMU_SERVER_BINARY_MESSAGES_H
#define PLAT4M_IMU_SERVER_BINARY_MESSAGES_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <stdint.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

namespace ImuServerBinaryMessages
{
    static const uint16_t groupId = 0x0004;

    static const uint16_t getCountBinaryMessageId          = 0x0000;
    static const uint16_t getCountResponseBinaryMessageId  = 0x0001;

    static const uint16_t setConfigBinaryMessageId         = 0x0002;
    static const uint16_t setConfigResponseBinaryMessageId = 0x0003;

    static const uint16_t getConfigBinaryMessageId         = 0x0004;
    static const uint16_t getConfigResponseBinaryMessageId = 0x0005;

    static const uint16_t measurementBinaryMessageId       = 0x0004;

    static const uint16_t imuSetConfigBinaryMessageId         = 0x0006;
    static const uint16_t imuSetConfigResponseBinaryMessageId = 0x0007;

    static const uint16_t imuGetConfigBinaryMessageId         = 0x0008;
    static const uint16_t imuGetConfigResponseBinaryMessageId = 0x0009;

    static const uint16_t getMeasurementBinaryMessageId       = 0x000A;
}; // namespace ImuServerBinaryMessages

}; // namespace Plat4m

#endif // PLAT4M_IMU_SERVER_BINARY_MESSAGES_H
