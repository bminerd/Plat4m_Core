/*------------------------------------------------------------------------------
 *       _______    __                           ___
 *      ||  ___ \  || |             __          //  |
 *      || |  || | || |   _______  || |__      //   |    _____  ___
 *      || |__|| | || |  // ___  | ||  __|    // _  |   ||  _ \/ _ \
 *      ||  ____/  || | || |  || | || |      // /|| |   || |\\  /\\ \
 *      || |       || | || |__|| | || |     // /_|| |_  || | || | || |
 *      || |       || |  \\____  | || |__  //_____   _| || | || | || |
 *      ||_|       ||_|       ||_|  \\___|       ||_|   ||_| ||_| ||_|
 *
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2013 Benjamin Minerd
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *----------------------------------------------------------------------------*/

/**
 * @file ByteArray.cpp
 * @author Ben Minerd
 * @date 6/17/13
 * @brief ByteArray class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <ByteArray.h>

/*------------------------------------------------------------------------------
 * Public constructors and destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
ByteArray::ByteArray() :
    Array<uint8_t>()
{
}

//------------------------------------------------------------------------------
ByteArray::ByteArray(uint8_t bytes[],
                     const unsigned int nBytes,
                     const int nUsedBytes) :
    Array<uint8_t>(bytes, nBytes, nUsedBytes)
{
}

//------------------------------------------------------------------------------
ByteArray::ByteArray(const Array<uint8_t>& array) :
    Array<uint8_t>(array)
{
}

/*------------------------------------------------------------------------------
 * Public methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
bool ByteArray::append(const char string[])
{
    return Array<uint8_t>::append((uint8_t*) string, strlen(string));
}

////------------------------------------------------------------------------------
//bool ByteArray::append(const int byte)
//{
//    return Array<uint8_t>::append((uint8_t) byte);
//}

//------------------------------------------------------------------------------
bool ByteArray::prepend(const char* string)
{
    return Array<uint8_t>::prepend((uint8_t*) string, strlen(string));
}

//------------------------------------------------------------------------------
bool ByteArray::insert(const char* string, const unsigned int index)
{
    return Array<uint8_t>::insert((uint8_t*) string, strlen(string), index);
}

//------------------------------------------------------------------------------
void ByteArray::setValue(const uint8_t value)
{
    memset(getItems(), value, getMaxSize());
    setSize(getMaxSize());
}