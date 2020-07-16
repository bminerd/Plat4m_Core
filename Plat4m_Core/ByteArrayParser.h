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
/// @file ByteArrayParser.h
/// @author Ben Minerd
/// @date 6/17/2013
/// @brief ByteArrayParser class header file.
///

#ifndef PLAT4M_BYTE_ARRAY_PARSER_H
#define PLAT4M_BYTE_ARRAY_PARSER_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/ByteArray.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class ByteArrayParser
{
public:
    
    //--------------------------------------------------------------------------
    // Public enumerations
    //--------------------------------------------------------------------------

    enum ParseDirection
    {
        PARSE_DIRECTION_FORWARD,
        PARSE_DIRECTION_BACKWARD
    };

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------
    
    ByteArrayParser(const ByteArray& byteArray,
                    const Endian endian,
                    const ParseDirection parseDirection);

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~ByteArrayParser();

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    bool parse(uint8_t& item);

    bool parse(uint16_t& item);

    bool parse(uint32_t& item);

    bool parse(float& item);

    bool parse(uint64_t& item);

    bool parse(double& item);

private:
    
    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    const ByteArray& myByteArray;

    const Endian myEndian;

    const ParseDirection myParseDirection;

    uint32_t myIndex;
};

}; // namespace Plat4m

#endif // PLAT4M_BYTE_ARRAY_PARSER_H
