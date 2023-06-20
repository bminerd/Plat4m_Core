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
/// @file DataObject.h
/// @author Ben Minerd
/// @date 5/30/2023
/// @brief DataObject class header file.
///

#ifndef PLAT4M_DATA_OBJECT_H
#define PLAT4M_DATA_OBJECT_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/DataObjectBase.h>
#include <Plat4m_Core/DataObjectInterface.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

///
/// @brief Class that inherits from DataObjectBase and is meant to be the main
/// interface for DataObjects.
/// @tparam DataType Type of data to be stored in this DataObject.
/// @tparam AccessOption Access option for the DataObject. Can be
/// ACCESS_READ_WRITE, ACCESS_READ_ONLY_PUSH, or ACCESS_READ_ONLY_PULL.
/// @note Due to the partial template specialization below with AccessOption =
/// ACCESS_READ_WRITE, this class effectively only represents AccessOption =
/// ACCESS_READ_ONLY_PUSH or ACCESS_READ_ONLY_PULL.
///
template <typename DataType, DataObjectBase::Access AccessOption>
class DataObject : public DataObjectInterface<DataType>
{
public:

    //--------------------------------------------------------------------------
    // Public static methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    static DataObject& find(const DataObjectBase::Id id)
    {
        DataObject* dataObject =
            DataObjectInterface<DataType>::
                          template find<DataObject<DataType, AccessOption>>(id);

        return (*dataObject);
    }

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual ~DataObject()
    {
    }

    //--------------------------------------------------------------------------
    // Public pure virtual methods
    //--------------------------------------------------------------------------

    virtual DataObjectBase::Error read(DataType& data) = 0;

protected:

    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    DataObject(const DataObjectBase::Id id) :
        DataObjectInterface<DataType>(id)
    {
    }
};

///
/// @brief Partial template specialization for
/// DataObject<DataType, AccessOption> where AccessOption = ACCESS_READ_WRITE.
/// @tparam DataType Type of data to be stored in this DataObject.
///
template <typename DataType>
class DataObject<DataType, DataObjectBase::ACCESS_READ_WRITE> :
                                            public DataObjectInterface<DataType>
{
public:

    //--------------------------------------------------------------------------
    // Public static methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    static DataObject& find(const DataObjectBase::Id id)
    {
        DataObject* dataObject =
            DataObjectInterface<DataType>::
                template find<DataObject<
                              DataType, DataObjectBase::ACCESS_READ_WRITE>>(id);

        return (*dataObject);
    }

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual ~DataObject()
    {
    }

    //--------------------------------------------------------------------------
    // Public pure virtual methods
    //--------------------------------------------------------------------------

    virtual DataObjectBase::Error read(DataType& data) = 0;

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    DataObjectBase::Error write(const DataType& data)
    {
        DataObjectBase::Error error;

        if (myIsIncrementalWriteInProgress)
        {
            DataObjectInterface<DataType>::getCurrentData() = data;

            error = DataObjectBase::Error(DataObjectBase::ERROR_CODE_NONE);
        }
        else
        {
            error = subclassWrite(data);
        }

        return error;
    }

    //--------------------------------------------------------------------------
    void startIncrementalWrite()
    {
        myIsIncrementalWriteInProgress = true;
    }

    //--------------------------------------------------------------------------
    DataObjectBase::Error finishIncrementalWrite()
    {
        myIsIncrementalWriteInProgress = false;

        return write(DataObjectInterface<DataType>::getCurrentData());
    }

protected:

    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    DataObject(const DataObjectBase::Id id) :
        DataObjectInterface<DataType>(id),
        myIsIncrementalWriteInProgress(false)
    {
    }

    //--------------------------------------------------------------------------
    // Protected pure virtual methods
    //--------------------------------------------------------------------------

    virtual DataObjectBase::Error subclassWrite(const DataType& data) = 0;

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    bool myIsIncrementalWriteInProgress;
};

///
/// @brief Partial template specialization for
/// DataObject<DataType, AccessOption> where AccessOption = ACCESS_WRITE_ONLY.
/// @tparam DataType Type of data to be stored in this DataObject.
///
template <typename DataType>
class DataObject<DataType, DataObjectBase::ACCESS_WRITE_ONLY> :
                                            public DataObjectInterface<DataType>
{
public:

    //--------------------------------------------------------------------------
    // Public static methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    static DataObject& find(const DataObjectBase::Id id)
    {
        DataObject* dataObject =
            DataObjectInterface<DataType>::
                template find<DataObject<
                              DataType, DataObjectBase::ACCESS_WRITE_ONLY>>(id);

        return (*dataObject);
    }

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual ~DataObject()
    {
    }

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    DataObjectBase::Error write(const DataType& data)
    {
        DataObjectBase::Error error;

        if (myIsIncrementalWriteInProgress)
        {
            DataObjectInterface<DataType>::getCurrentData() = data;

            error = DataObjectBase::Error(DataObjectBase::ERROR_CODE_NONE);
        }
        else
        {
            error = subclassWrite(data);
        }

        return error;
    }

    //--------------------------------------------------------------------------
    void startIncrementalWrite()
    {
        myIsIncrementalWriteInProgress = true;
    }

    //--------------------------------------------------------------------------
    DataObjectBase::Error finishIncrementalWrite()
    {
        myIsIncrementalWriteInProgress = false;

        return write(DataObjectInterface<DataType>::getCurrentData());
    }

protected:

    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    DataObject(const DataObjectBase::Id id) :
        DataObjectInterface<DataType>(id),
        myIsIncrementalWriteInProgress(false)
    {
    }

    //--------------------------------------------------------------------------
    // Protected pure virtual methods
    //--------------------------------------------------------------------------

    virtual DataObjectBase::Error subclassWrite(const DataType& data) = 0;

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    bool myIsIncrementalWriteInProgress;
};

//------------------------------------------------------------------------------
// Namespace functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template <typename DataType, DataObjectBase::Access AccessOption>
DataObject<DataType, AccessOption>& findDataObject(
                           const DataObjectBase::Id id,
                           const DataObject<DataType, AccessOption>& dataObject)
{
    return (DataObject<DataType, AccessOption>::find(id));
}

}; // namespace Plat4m

#endif // PLAT4M_DATA_OBJECT_H
