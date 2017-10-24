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
/// @file MemoryFlashSTM32F30x.h
/// @author Ben Minerd
/// @date 3/18/2016
/// @brief MemoryFlashSTM32F30x class header file.
///

#ifndef PLAT4M_MEMORY_FLASH_STM32F30X_H
#define PLAT4M_MEMORY_FLASH_STM32F30X_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m.h>
#include <Memory.h>
     
#include <stm32f30x.h>
#include <stm32f30x_flash.h>
#include <stm32f30x_rcc.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class MemoryFlashSTM32F30x : public Memory<intptr_t>
{
public:
    
    //--------------------------------------------------------------------------
    // Public enumerations
    //--------------------------------------------------------------------------
    
    //--------------------------------------------------------------------------
    // Public typedefs
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Public static methods
    //--------------------------------------------------------------------------
    
    static MemoryFlashSTM32F30x& get();

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    static MemoryFlashSTM32F30x* myObject;

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------
    
    //--------------------------------------------------------------------------
    // Private constructors
    //--------------------------------------------------------------------------
    
    MemoryFlashSTM32F30x();

    //--------------------------------------------------------------------------
    // Private methods implemented from Module
    //--------------------------------------------------------------------------
    
    Module::Error driverEnable(const bool enable);

    //--------------------------------------------------------------------------
    // Private methods implemented from Memory
    //--------------------------------------------------------------------------
    
    Memory::Error driverConfigure(const Config& config);

    Memory::Error driverClear(const intptr_t address,
                              const unsigned int nBytes);

    Memory::Error driverRead(const intptr_t address, ByteArray& dataArray);

    Memory::Error driverWrite(const intptr_t address,
                              const ByteArray& dataArray);

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------
};

}; // namespace Plat4m

#endif // PLAT4M_MEMORY_FLASH_STM32F30X_H
