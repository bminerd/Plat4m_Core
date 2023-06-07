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
// Copyright (c) 2022-2023 Benjamin Minerd
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
/// @file Service.h
/// @author Ben Minerd
/// @date 9/22/2022
/// @brief Service class header file.
///

#ifndef PLAT4M_SERVICE_H
#define PLAT4M_SERVICE_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <new>

#include <Plat4m_Core/ServiceBase.h>
#include <Plat4m_Core/Callback.h>
#include <Plat4m_Core/List.h>
#include <Plat4m_Core/System.h>
#include <Plat4m_Core/ServiceManager.h>
#include <Plat4m_Core/ServiceRequest.h>
#include <Plat4m_Core/ServiceResponse.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

template <typename RequestType, typename ResponseType>
class Service : public ServiceBase
{
public:

    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------

    using ServiceCallback = Callback<Error,
                                     const ServiceRequest<RequestType>&,
                                     ServiceResponse<ResponseType>&>;

    //--------------------------------------------------------------------------
    // Public static methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    static Service& create(const ServiceBase::Id id, ServiceCallback& callback)
    {
        Service* service = findPrivate(id);

        if (isNullPointer(service))
        {
            service = createPrivate(id, callback);
        }
        else
        {
            service->myCallback = &callback;
        }

        return (*service);
    }

    //--------------------------------------------------------------------------
    static Service& find(const ServiceBase::Id id)
    {
        Service* service = findOrCreate(id);

        return (*service);
    }

    //--------------------------------------------------------------------------
    static Error request(const ServiceBase::Id id,
                         const RequestType& request,
                         ResponseType& response)
    {
        Service* service = findPrivate(id);

        if (isNullPointer(service))
        {
            return Error(ERROR_CODE_SERVICE_INVALID);
        }

        Error error = service->request(request, response);

        return error;
    }

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------
    
    //--------------------------------------------------------------------------
    virtual ~Service()
    {
        Service* pointer = this;

        ServiceManager::remove(*pointer);
    }

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    Error request(const RequestType& request, ResponseType& response)
    {
        if (isNullPointer(myCallback))
        {
            return Error(ERROR_CODE_SERVICE_NOT_INITIALIZED);
        }

        ServiceRequest<RequestType> serviceRequest(request);

        ServiceResponse<ResponseType> serviceResponse(response);

        Error error = myCallback->call(serviceRequest, serviceResponse);

        return error;
    }

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    ServiceCallback* myCallback;

    //--------------------------------------------------------------------------
    // Private static methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    static Service* findPrivate(const ServiceBase::Id id)
    {
        Service* service = 0;

        ServiceBase* serviceBase = ServiceManager::find(id);

        if (isValidPointer(serviceBase))
        {
            service = dynamic_cast<Service*>(serviceBase);

            if (isNullPointer(service))
            {
                Error error(ERROR_CODE_SERVICE_TYPE_ID_MISMATCH);
            }
        }

        return service;
    }

    //--------------------------------------------------------------------------
    static Service* createPrivate(const ServiceBase::Id id)
    {
        void* memoryPointer = AllocationMemory::allocate(sizeof(Service));

        Service* service = new(memoryPointer) Service(id);

        return service;
    }

    //--------------------------------------------------------------------------
    static Service* createPrivate(const ServiceBase::Id id,
                                  ServiceCallback& callback)
    {
        void* memoryPointer = AllocationMemory::allocate(sizeof(Service));

        Service* service = new(memoryPointer) Service(id, callback);

        return service;
    }

    //--------------------------------------------------------------------------
    static Service* findOrCreate(const ServiceBase::Id id)
    {
        Service* service = findPrivate(id);

        if (isNullPointer(service))
        {
            service = createPrivate(id);
        }

        return service;
    }

    //--------------------------------------------------------------------------
    // Private constructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    Service(const ServiceBase::Id id) :
        ServiceBase(id),
        myCallback(0)
    {
        Service* pointer = this;

        ServiceManager::add(*pointer);
    }

    //--------------------------------------------------------------------------
    Service(const ServiceBase::Id id, ServiceCallback& callback) :
        ServiceBase(id),
        myCallback(&callback)
    {
        Service* pointer = this;

        ServiceManager::add(*pointer);
    }

    //--------------------------------------------------------------------------
    Service(const Service& service) :
        ServiceBase(service),
        myCallback(service.myCallback)
    {
    }
};

//------------------------------------------------------------------------------
// Namespace functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template <typename TClass, typename RequestType, typename ResponseType>
Service<RequestType, ResponseType>& createService(
      const ServiceBase::Id id,
      TClass* object,
      ServiceBase::Error (TClass::*callback)(const RequestType&, ResponseType&))
{
    return (Service<RequestType, ResponseType>::create(
                                             id,
                                             createCallback(object, callback)));
}

//------------------------------------------------------------------------------
template <typename RequestType, typename ResponseType>
Service<RequestType, ResponseType>& createService(
    const ServiceBase::Id id,
    ServiceBase::Error (*callback)
           (const ServiceRequest<RequestType>&, ServiceResponse<ResponseType>&))
{
    return (Service<RequestType, ResponseType>::create(
                                                     id,
                                                     createCallback(callback)));
}

}; // namespace Plat4m

#endif // PLAT4M_SERVICE_H
