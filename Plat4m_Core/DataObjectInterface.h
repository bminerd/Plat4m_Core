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
/// @file DataObjectInterface.h
/// @author Ben Minerd
/// @date 6/14/2023
/// @brief DataObjectInterface class header file.
///

#ifndef PLAT4M_DATA_OBJECT_INTERFACE_H
#define PLAT4M_DATA_OBJECT_INTERFACE_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/DataObjectBase.h>
#include <Plat4m_Core/DataObjectManager.h>
#include <Plat4m_Core/Callback.h>
#include <Plat4m_Core/List.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

template <typename DataType>
class DataObjectInterface : public DataObjectBase
{
public:

    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------

    using DataCallback = Callback<void, const DataType&>;

    //--------------------------------------------------------------------------
    // Public static methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    template <typename DataObjectDervied>
    static DataObjectDervied* find(const DataObjectBase::Id id)
    {
        DataObjectBase* dataObjectBase = DataObjectManager::find(id);

        if (isNullPointer(dataObjectBase))
        {
            Error error(ERROR_CODE_DATA_OBJECT_NOT_INSTANTIATED);

            // Lock up, attempting to access a DataObject that hasn't been
            // instantiated yet. Current requirement is that all DataObjects
            // must be created first before attempting access.
            while (true)
            {
            }

            return 0;
        }

        DataObjectDervied* dataObjectDerived =
                               dynamic_cast<DataObjectDervied*>(dataObjectBase);

        if (isNullPointer(dataObjectDerived))
        {
            Error error(ERROR_CODE_DATA_OBJECT_TYPE_ID_MISMATCH);

            return 0;
        }

        return dataObjectDerived;
    }

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual ~DataObjectInterface()
    {
    }

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    void addDataUpdatedCallback(DataCallback& callback)
    {
        myDataUpdatedCallbackList.append(callback);
    }

    //--------------------------------------------------------------------------
    void addDataChangedCallback(DataCallback& callback)
    {
        myDataChangedCallbackList.append(callback);
    }

    //--------------------------------------------------------------------------
    DataType& getCurrentData()
    {
        return myCurrentData;
    }

    //--------------------------------------------------------------------------
    const DataType& getCurrentData() const
    {
        return myCurrentData;
    }

protected:

    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    DataObjectInterface(const DataObjectBase::Id id) :
        DataObjectBase(id),
        myCurrentData(),
        myDataUpdatedCallbackList(),
        myDataChangedCallbackList()
    {
    }

    //--------------------------------------------------------------------------
    void dataUpdated(const DataType& data)
    {
        typename List<DataCallback&>::Iterator iterator =
                                           myDataUpdatedCallbackList.iterator();

        while (iterator.hasCurrent())
        {
            DataCallback& callback = iterator.current();

            callback.call(data);

            iterator.next();
        }

        if (data != myCurrentData)
        {
            dataChanged(data);
        }

        myCurrentData = data;
    }

    //--------------------------------------------------------------------------
    void dataChanged(const DataType& data)
    {
        typename List<DataCallback&>::Iterator iterator =
                                           myDataChangedCallbackList.iterator();

        while (iterator.hasCurrent())
        {
            DataCallback& callback = iterator.current();

            callback.call(data);

            iterator.next();
        }
    }

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    DataType myCurrentData;

    List<DataCallback&> myDataUpdatedCallbackList;

    List<DataCallback&> myDataChangedCallbackList;
};

}; // namespace Plat4m

#endif // PLAT4M_DATA_OBJECT_INTERFACE_H
