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
/// @file ByteArrayParser.cpp
/// @author Ben Minerd
/// @date 6/17/13
/// @brief ByteArrayParser class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <ByteArrayParser.h>

using Plat4m::ByteArrayParser;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ByteArrayParser::ByteArrayParser(const ByteArray& byteArray,
                                 const Endian endian,
                                 const ParseDirection parseDirection) :
    myByteArray(byteArray),
    myEndian(endian),
    myParseDirection(parseDirection),
    myIndex(0)
{
    if (myParseDirection == PARSE_DIRECTION_BACKWARD)
    {
        myIndex = byteArray.getSize() - 1;
    }
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ByteArrayParser::~ByteArrayParser()
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool ByteArrayParser::parse(uint8_t& item)
{
   item = myByteArray[myIndex];

   myIndex += 1;

   return true;
}

//------------------------------------------------------------------------------
bool ByteArrayParser::parse(uint16_t& item)
{
   if (myEndian == ENDIAN_BIG)
   {
       item = (((uint16_t) myByteArray[(myIndex)]) << 8) |
              (((uint16_t) myByteArray[(myIndex + 1)]));
   }
   else // myEndian == ENDIAN_LITTLE
   {
       item = (((uint16_t) myByteArray[(myIndex)]) << 8) |
              (((uint16_t) myByteArray[(myIndex + 1)]));
   }

   myIndex += 2;

   return true;
}

//------------------------------------------------------------------------------
bool ByteArrayParser::parse(uint32_t& item)
{
   if (myEndian == ENDIAN_BIG)
   {
       item = (((uint32_t) myByteArray[(myIndex)])     << 24) |
              (((uint32_t) myByteArray[(myIndex + 1)]) << 16) |
              (((uint32_t) myByteArray[(myIndex + 2)]) << 8)  |
              (((uint32_t) myByteArray[(myIndex + 3)]));
   }
   else // myEndian == ENDIAN_LITTLE
   {
       item = (((uint32_t) myByteArray[(myIndex + 3)]) << 24) |
              (((uint32_t) myByteArray[(myIndex + 2)]) << 16) |
              (((uint32_t) myByteArray[(myIndex + 1)]) << 8)  |
              (((uint32_t) myByteArray[(myIndex)]));
   }

   myIndex += 4;

   return true;
}

//------------------------------------------------------------------------------
bool ByteArrayParser::parse(float& item)
{
   if (myEndian == ENDIAN_BIG)
   {
       *((uint32_t*)(&item)) = ((((uint32_t) myByteArray[(myIndex)])     << 24) |
                       (((uint32_t) myByteArray[(myIndex + 1)]) << 16) |
                       (((uint32_t) myByteArray[(myIndex + 2)]) << 8)  |
                       (((uint32_t) myByteArray[(myIndex + 3)])));
   }
   else // myEndian == ENDIAN_LITTLE
   {
       item = (float) ((((uint32_t) myByteArray[(myIndex + 3)]) << 24) |
                       (((uint32_t) myByteArray[(myIndex + 2)]) << 16) |
                       (((uint32_t) myByteArray[(myIndex + 1)]) << 8)  |
                       (((uint32_t) myByteArray[(myIndex)])));
   }

   myIndex += 4;

   return true;
}

//------------------------------------------------------------------------------
bool ByteArrayParser::parse(uint64_t& item)
{
   if (myEndian == ENDIAN_BIG)
   {
       item = (((uint64_t) myByteArray[(myIndex)])     << 56) |
              (((uint64_t) myByteArray[(myIndex + 1)]) << 48) |
              (((uint64_t) myByteArray[(myIndex + 2)]) << 40) |
              (((uint64_t) myByteArray[(myIndex + 3)]) << 32) |
              (((uint64_t) myByteArray[(myIndex + 4)]) << 24) |
              (((uint64_t) myByteArray[(myIndex + 5)]) << 16) |
              (((uint64_t) myByteArray[(myIndex + 6)]) << 8)  |
              (((uint64_t) myByteArray[(myIndex + 7)]));
   }
   else // myEndian == ENDIAN_LITTLE
   {
       item = (((uint64_t) myByteArray[(myIndex + 7)]) << 56) |
              (((uint64_t) myByteArray[(myIndex + 6)]) << 48) |
              (((uint64_t) myByteArray[(myIndex + 5)]) << 40) |
              (((uint64_t) myByteArray[(myIndex + 4)]) << 32) |
              (((uint64_t) myByteArray[(myIndex + 3)]) << 24) |
              (((uint64_t) myByteArray[(myIndex + 2)]) << 16) |
              (((uint64_t) myByteArray[(myIndex + 1)]) << 8)  |
              (((uint64_t) myByteArray[(myIndex)]));
   }

   myIndex += 8;

   return true;
}

//------------------------------------------------------------------------------
bool ByteArrayParser::parse(double& item)
{
   if (myEndian == ENDIAN_BIG)
   {
       item = (double) ((((uint64_t) myByteArray[(myIndex)])     << 56) |
                        (((uint64_t) myByteArray[(myIndex + 1)]) << 48) |
                        (((uint64_t) myByteArray[(myIndex + 2)]) << 40) |
                        (((uint64_t) myByteArray[(myIndex + 3)]) << 32) |
                        (((uint64_t) myByteArray[(myIndex + 4)]) << 24) |
                        (((uint64_t) myByteArray[(myIndex + 5)]) << 16) |
                        (((uint64_t) myByteArray[(myIndex + 6)]) << 8)  |
                        (((uint64_t) myByteArray[(myIndex + 7)])));
   }
   else // myEndian == ENDIAN_LITTLE
   {
       item = (double) ((((uint64_t) myByteArray[(myIndex + 7)]) << 56) |
                        (((uint64_t) myByteArray[(myIndex + 6)]) << 48) |
                        (((uint64_t) myByteArray[(myIndex + 5)]) << 40) |
                        (((uint64_t) myByteArray[(myIndex + 4)]) << 32) |
                        (((uint64_t) myByteArray[(myIndex + 3)]) << 24) |
                        (((uint64_t) myByteArray[(myIndex + 2)]) << 16) |
                        (((uint64_t) myByteArray[(myIndex + 1)]) << 8)  |
                        (((uint64_t) myByteArray[(myIndex)])));
   }

   myIndex += 8;

   return true;
}
