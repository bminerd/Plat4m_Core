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
// Copyright (c) 2023 Benjamin Minerd
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
/// @file MemoryManager.cpp
/// @author Ben Minerd
/// @date 4/21/2023
/// @brief MemoryManager class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Plat4m.h>

#include <Plat4m_Core/MemoryManager.h>
#include <Plat4m_Core/MemoryAllocator.h>
#include <Plat4m_Core/ErrorTemplate.h>

using namespace Plat4m;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

MemoryManager* MemoryManager::myInstance = 0;

//------------------------------------------------------------------------------
// Public static methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool MemoryManager::isInitialized()
{
    return (isValidPointer(myInstance));
}

//------------------------------------------------------------------------------
void MemoryManager::addBasePointerForDeletion(Base** basePointer)
{
    if (isValidPointer(myInstance))
    {
        myInstance->addBasePointerForDeletionPrivate(basePointer);
    }
}

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
MemoryManager::MemoryManager() :
    myBaseObjectDeletionList()
{
    if (isValidPointer(myInstance))
    {
        PLAT4M_REPORT_ERROR(MemoryManager::Error,
                            MemoryManager::ERROR_CODE_INSTANCE_ALREADY_CREATED,
                            ErrorBase::SEVERITY_HIGH,
                            this);
    }

    myInstance = this;
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
MemoryManager::~MemoryManager()
{
    List<Base**>::Iterator iterator = myBaseObjectDeletionList.iterator();

    while (iterator.hasCurrent())
    {
        Base** basePointer = iterator.current();

        if (isValidPointer(basePointer))
        {
            MemoryAllocator::deallocate(basePointer);
        }

        iterator.next();
    }

    myInstance = 0;
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void MemoryManager::addBasePointerForDeletionPrivate(Base** basePointer)
{
    Base** pointer = basePointer;

    myBaseObjectDeletionList.append(pointer);
}
