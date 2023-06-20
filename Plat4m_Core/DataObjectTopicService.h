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
/// @file DataObjectTopicService.h
/// @author Ben Minerd
/// @date 5/30/2023
/// @brief DataObjectTopicService class header file.
///

#ifndef PLAT4M_DATA_OBJECT_TOPIC_SERVICE_H
#define PLAT4M_DATA_OBJECT_TOPIC_SERVICE_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>

#include <Plat4m_Core/DataObject.h>
#include <Plat4m_Core/DataObjectBase.h>
#include <Plat4m_Core/TopicBase.h>
#include <Plat4m_Core/TopicSubscriber.h>
#include <Plat4m_Core/ServiceBase.h>
#include <Plat4m_Core/ServiceClient.h>
#include <Plat4m_Core/CallbackMethod.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

///
/// @brief Class that inherits from DataObject and represents a DataObject
/// implementation that is driven from Topics and Services.
/// @tparam DataType Type of data to be stored in this DataObject.
/// @tparam AccessOption Access option for the DataObject. Can be
/// ACCESS_READ_WRITE, ACCESS_READ_ONLY_PUSH, or ACCESS_READ_ONLY_PULL.
/// @note Due to the partial template specializations below, this class
/// effectively only represents AccessOption = ACCESS_READ_ONLY_PUSH.
///
template <typename DataType,
          DataObjectBase::Access AccessOption,
          typename DataAccessError = void*>
class DataObjectTopicService : public DataObject<DataType, AccessOption>
{
public:

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    DataObjectTopicService(const DataObjectBase::Id dataObjectId,
                           const TopicBase::Id topicId) :
        DataObject<DataType, AccessOption>(dataObjectId),
        myTopicSubscriber(
            topicId,
            createCallback(
                this,
                &DataObjectTopicService<DataType, AccessOption>::
                                                                sampleCallback))
    {
    }

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual ~DataObjectTopicService()
    {
    }

    //--------------------------------------------------------------------------
    // Public virtual methods overridden for DataObject
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual DataObjectBase::Error read(DataType& data) override
    {
        data = DataObjectInterface<DataType>::getCurrentData();

        return DataObjectBase::Error(DataObjectBase::ERROR_CODE_NONE);
    }

protected:

    //--------------------------------------------------------------------------
    // Private virtual methods overridden for Plat4m::Module
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual Module::Error driverSetEnabled(const bool enabled) override
    {
        myTopicSubscriber.setEnabled(enabled);

        return Module::Error(Module::ERROR_CODE_NONE);
    }

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    TopicSubscriber<DataType> myTopicSubscriber;

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    void sampleCallback(const TopicSample<DataType>& sample)
    {
        DataObjectInterface<DataType>::dataUpdated(sample.data);
    }
};

///
/// @brief Partial template specialization for
/// DataObjectTopicService<DataType, AccessOption> where AccessOption =
/// ACCESS_READ_ONLY_PULL.
/// @tparam DataType Type of data to be stored in this DataObject.
///
template <typename DataType>
class DataObjectTopicService<DataType,
                             DataObjectBase::ACCESS_READ_ONLY_PULL> :
              public DataObject<DataType, DataObjectBase::ACCESS_READ_ONLY_PULL>
{
public:

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    DataObjectTopicService(const DataObjectBase::Id dataObjectId,
                           const ServiceBase::Id getValueServiceId) :
        DataObject<DataType, DataObjectBase::ACCESS_READ_ONLY_PULL>(
                                                                  dataObjectId),
        myGetValueServiceClient(getValueServiceId)
    {
    }

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual ~DataObjectTopicService()
    {
    }

    //--------------------------------------------------------------------------
    // Public virtual methods overridden for DataObject
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual DataObjectBase::Error read(DataType& data) override
    {
        ServiceBase::Error serviceError = 
                                       myGetValueServiceClient.request(0, data);

        if (serviceError.getCode() != ServiceBase::ERROR_CODE_NONE)
        {
            return DataObjectBase::Error(
                                        DataObjectBase::ERROR_CODE_READ_FAILED);
        }

        DataObjectInterface<DataType>::dataUpdated(data);

        return DataObjectBase::Error(DataObjectBase::ERROR_CODE_NONE);
    }

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    DataType myCurrentData;

    ServiceClient<void*, DataType> myGetValueServiceClient;
};

///
/// @brief Partial template specialization for
/// DataObjectTopicService<DataType, AccessOption> where AccessOption =
/// ACCESS_READ_WRITE.
/// @tparam DataType Type of data to be stored in this DataObject.
///
template <typename DataType, typename DataAccessError>
class DataObjectTopicService<DataType,
                             DataObjectBase::ACCESS_READ_WRITE,
                             DataAccessError> :
                  public DataObject<DataType, DataObjectBase::ACCESS_READ_WRITE>
{
public:

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    DataObjectTopicService(const DataObjectBase::Id dataObjectId,
                           const ServiceBase::Id setValueServiceId,
                           const ServiceBase::Id getValueServiceId) :
        DataObject<DataType, DataObjectBase::ACCESS_READ_WRITE>(dataObjectId),
        mySetValueServiceClient(setValueServiceId),
        myGetValueServiceClient(getValueServiceId)
    {
    }

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual ~DataObjectTopicService()
    {
    }

    //--------------------------------------------------------------------------
    // Public virtual methods overridden for DataObject
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual DataObjectBase::Error read(DataType& data) override
    {
        ServiceBase::Error serviceError = 
                                       myGetValueServiceClient.request(0, data);

        if (serviceError.getCode() != ServiceBase::ERROR_CODE_NONE)
        {
            return DataObjectBase::Error(
                                        DataObjectBase::ERROR_CODE_READ_FAILED);
        }

        DataObjectInterface<DataType>::dataUpdated(data);

        return DataObjectBase::Error(DataObjectBase::ERROR_CODE_NONE);
    }

protected:

    //--------------------------------------------------------------------------
    // Protected virtual methods overridden for DataObject
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual DataObjectBase::Error subclassWrite(const DataType& data) override
    {
        DataAccessError dataAccessError;

        ServiceBase::Error serviceError =
                         mySetValueServiceClient.request(data, dataAccessError);

        if (serviceError.getCode() != ServiceBase::ERROR_CODE_NONE)
        {
            return DataObjectBase::Error(
                                       DataObjectBase::ERROR_CODE_WRITE_FAILED);
        }

        DataObjectInterface<DataType>::dataUpdated(data);

        return DataObjectBase::Error(DataObjectBase::ERROR_CODE_NONE);
    }

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    ServiceClient<DataType, DataAccessError> mySetValueServiceClient;

    ServiceClient<void*, DataType> myGetValueServiceClient;
};

///
/// @brief Partial template specialization for
/// DataObjectTopicService<DataType, AccessOption> where AccessOption =
/// ACCESS_WRITE_ONLY.
/// @tparam DataType Type of data to be stored in this DataObject.
///
template <typename DataType, typename DataAccessError>
class DataObjectTopicService<DataType,
                             DataObjectBase::ACCESS_WRITE_ONLY,
                             DataAccessError> :
                  public DataObject<DataType, DataObjectBase::ACCESS_WRITE_ONLY>
{
public:

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    DataObjectTopicService(const DataObjectBase::Id dataObjectId,
                           const ServiceBase::Id setValueServiceId) :
        DataObject<DataType, DataObjectBase::ACCESS_WRITE_ONLY>(dataObjectId),
        mySetValueServiceClient(setValueServiceId)
    {
    }

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual ~DataObjectTopicService()
    {
    }

protected:

    //--------------------------------------------------------------------------
    // Protected virtual methods overridden for DataObject
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual DataObjectBase::Error subclassWrite(const DataType& data) override
    {
        DataAccessError dataAccessError;

        ServiceBase::Error serviceError =
                         mySetValueServiceClient.request(data, dataAccessError);

        if (serviceError.getCode() != ServiceBase::ERROR_CODE_NONE)
        {
            return DataObjectBase::Error(
                                       DataObjectBase::ERROR_CODE_WRITE_FAILED);
        }

        DataObjectInterface<DataType>::dataUpdated(data);

        return DataObjectBase::Error(DataObjectBase::ERROR_CODE_NONE);
    }

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    ServiceClient<DataType, DataAccessError> mySetValueServiceClient;
};

}; // namespace Plat4m

#endif // PLAT4M_DATA_OBJECT_TOPIC_SERVICE_H
