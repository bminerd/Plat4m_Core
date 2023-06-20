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
/// @file DataObjectManager.cpp
/// @author Ben Minerd
/// @date 10/24/2022
/// @brief DataObjectManager class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/DataObjectManager.h>
#include <Plat4m_Core/DataObjectBase.h>

using namespace Plat4m;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

DataObjectManager* DataObjectManager::myInstance = 0;

//------------------------------------------------------------------------------
// Public static methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void DataObjectManager::add(DataObjectBase& dataObject)
{
    if (isNullPointer(myInstance))
    {
        Error error(ERROR_CODE_INSTANCE_NOT_CREATED);

        // Lock up
        while (true)
        {
        }
    }

    myInstance->addPrivate(dataObject);
}

//------------------------------------------------------------------------------
void DataObjectManager::remove(DataObjectBase& dataObject)
{
    if (isNullPointer(myInstance))
    {
        Error error(ERROR_CODE_INSTANCE_NOT_CREATED);

        // Lock up
        while (true)
        {
        }
    }

    myInstance->removePrivate(dataObject);
}

//------------------------------------------------------------------------------
DataObjectBase* DataObjectManager::find(const DataObjectBase::Id id)
{
    if (isNullPointer(myInstance))
    {
        Error error(ERROR_CODE_INSTANCE_NOT_CREATED);

        // Lock up
        while (true)
        {
        }
    }

    return (myInstance->findPrivate(id));
}

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
DataObjectManager::DataObjectManager()
{
    if (isValidPointer(myInstance))
    {
        Error error(ERROR_CODE_INSTANCE_ALREADY_CREATED);

        // Error, lock up
        while (true)
        {
        }
    }

    myInstance = this;
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
DataObjectManager::~DataObjectManager()
{
    typename List<DataObjectBase*>::Iterator iterator =
                                                    myDataObjectList.iterator();

    while (iterator.hasCurrent())
    {
        DataObjectBase* dataobject = iterator.current();
        
        dataobject->~DataObjectBase();

        iterator.next();
    }

    myInstance = 0;
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void DataObjectManager::addPrivate(DataObjectBase& dataObject)
{
    DataObjectBase* pointer = &dataObject;

    myDataObjectList.append(pointer);
}

//------------------------------------------------------------------------------
void DataObjectManager::removePrivate(DataObjectBase& dataobject)
{
    DataObjectBase* pointer = &dataobject;

    myDataObjectList.remove(pointer);
}

//------------------------------------------------------------------------------
DataObjectBase* DataObjectManager::findPrivate(const DataObjectBase::Id id)
{
    DataObjectBase* existingDataObject = 0;

    typename List<DataObjectBase*>::Iterator iterator =
                                                    myDataObjectList.iterator();

    while (iterator.hasCurrent())
    {
        DataObjectBase* dataObject = iterator.current();
        
        if (dataObject->getId() == id)
        {
            existingDataObject = dataObject;

            break;
        }

        iterator.next();
    }

    return existingDataObject;
}
