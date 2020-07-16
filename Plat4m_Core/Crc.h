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
/// @file Crc.h
/// @author Ben Minerd
/// @date 5/8/2017
/// @brief CrcCalculator namespace header file.
///

#ifndef PLAT4M_CRC_H
#define PLAT4M_CRC_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <stdint.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

namespace Crc
{

    //--------------------------------------------------------------------------
    // Static variables
    //--------------------------------------------------------------------------

    static const uint16_t crc16CcittPlynomial = 0x8005;

    //--------------------------------------------------------------------------
    // Inline functions
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    inline uint8_t reverseBitOrder(const uint8_t value)
    {
        uint8_t reversedValue = 0;

        reversedValue |= (value & 0x01) << 7;
        reversedValue |= (value & 0x02) << 5;
        reversedValue |= (value & 0x04) << 3;
        reversedValue |= (value & 0x08) << 1;
        reversedValue |= (value & 0x10) >> 1;
        reversedValue |= (value & 0x20) >> 3;
        reversedValue |= (value & 0x40) >> 5;
        reversedValue |= (value & 0x80) >> 7;

        return reversedValue;
    }

    //--------------------------------------------------------------------------
    inline uint16_t reverseBitOrder(const uint16_t value)
    {
        uint16_t reversedValue = 0;

        reversedValue |= (value & 0x0001) << 15;
        reversedValue |= (value & 0x0002) << 13;
        reversedValue |= (value & 0x0004) << 11;
        reversedValue |= (value & 0x0008) << 9;
        reversedValue |= (value & 0x0010) << 7;
        reversedValue |= (value & 0x0020) << 5;
        reversedValue |= (value & 0x0040) << 3;
        reversedValue |= (value & 0x0080) << 1;
        reversedValue |= (value & 0x0100) >> 1;
        reversedValue |= (value & 0x0200) >> 3;
        reversedValue |= (value & 0x0400) >> 5;
        reversedValue |= (value & 0x0800) >> 7;
        reversedValue |= (value & 0x1000) >> 9;
        reversedValue |= (value & 0x2000) >> 11;
        reversedValue |= (value & 0x4000) >> 13;
        reversedValue |= (value & 0x8000) >> 15;

        return reversedValue;
    }

    //--------------------------------------------------------------------------
    inline uint16_t calculateCrc16Ccitt(const ByteArray& data)
    {
        uint16_t crc = 0;
        uint32_t size = data.getSize();

        for (uint32_t i = 0; i < size; i++)
        {
            uint8_t reversedByte = reverseBitOrder(data[i]);

            crc ^= (((uint16_t) reversedByte) << 8);

            for (uint32_t j = 8; j > 0; j--)
            {
                if (areBitsSet(crc, 0x8000))
                {
                    crc = (crc << 1) ^ crc16CcittPlynomial;
                }
                else
                {
                    crc = (crc << 1);
                }
            }
        }

        crc = reverseBitOrder(crc);

        return crc;
    }

    //--------------------------------------------------------------------------
    inline bool checkCrc16Ccitt(const ByteArray& data, const uint16_t crc)
    {
        uint16_t calculatedCrc = calculateCrc16Ccitt(data);

        return (calculatedCrc == crc);
    }
};

}; // namespace Plat4m

#endif // PLAT4M_CRC_H
