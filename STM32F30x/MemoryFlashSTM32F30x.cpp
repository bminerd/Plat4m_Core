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
// Copyright (c) 2016 Benjamin Minerd
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
/// @file MemoryFlashSTM32F30x.cpp
/// @author Ben Minerd
/// @date 3/18/2016
/// @brief MemoryFlashSTM32F30x class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/STM32F30x/MemoryFlashSTM32F30x.h>
#include <Plat4m_Core/ByteArrayN.h>

using Plat4m::MemoryFlashSTM32F30x;
using Plat4m::Memory;
using Plat4m::Module;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

MemoryFlashSTM32F30x* MemoryFlashSTM32F30x::myObject = 0;

//------------------------------------------------------------------------------
// Public static methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
MemoryFlashSTM32F30x& MemoryFlashSTM32F30x::get()
{
    if (isNullPointer(myObject))
    {
//        myObject = new MemoryFlashSTM32F30x();
    }

    return (*myObject);
}

//------------------------------------------------------------------------------
// Private constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
MemoryFlashSTM32F30x::MemoryFlashSTM32F30x() :
    Memory<intptr_t>()
{
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error MemoryFlashSTM32F30x::driverEnable(const bool enable)
{
    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods implemented from Memory
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Memory<intptr_t>::Error MemoryFlashSTM32F30x::driverConfigure(
                                                           const Config& config)
{
    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
Memory<intptr_t>::Error MemoryFlashSTM32F30x::driverClear(
                                                      const intptr_t address,
                                                      const unsigned int nBytes)
{
    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
Memory<intptr_t>::Error MemoryFlashSTM32F30x::driverRead(
                                                         const intptr_t address,
                                                         ByteArray& dataArray)
{
    intptr_t startingAddress = address;
    uint32_t size = dataArray.getMaxSize();

    uint32_t wordSize = (size / sizeof(intptr_t)) + (size % sizeof(intptr_t));

    for (uint32_t i = 0; i < wordSize; i++)
    {
        uint32_t value = *(__IO uint32_t*) startingAddress;
        dataArray.append(value, ENDIAN_BIG, true);

        startingAddress += 4;
    }

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
Memory<intptr_t>::Error MemoryFlashSTM32F30x::driverWrite(
                                                     const intptr_t address,
                                                     const ByteArray& dataArray)
{
    int size = dataArray.getSize();

    Error error(ERROR_CODE_NONE);

    for (int i = 0; i < size; i++)
    {
        FLASH_Status status = FLASH_ProgramWord(address, dataArray[i]);

        if (status != FLASH_COMPLETE)
        {
            // Error
        }
    }

    return error;
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

