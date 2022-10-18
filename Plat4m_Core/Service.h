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
// Copyright (c) 2021 Benjamin Minerd
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
/// @date 9/22/2021
/// @brief Service class header file.
///

#ifndef PLAT4M_SERVICE_H
#define PLAT4M_SERVICE_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/ServiceBase.h>
#include <Plat4m_Core/ErrorTemplate.h>
#include <Plat4m_Core/Callback.h>
#include <Plat4m_Core/List.h>
#include <Plat4m_Core/System.h>

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

    typedef Callback<Error, const RequestType&, ResponseType&> ServiceCallback;

    //--------------------------------------------------------------------------
    // Public static methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    static Service* find(const Id id)
    {
        typename List<Service*>::Iterator iterator = myServiceList.iterator();

        while (iterator.hasCurrent())
        {
            Service* service = iterator.current();

            if (service->getId() == id)
            {
                return service;
            }

            iterator.next();
        }

        return 0;
    }

    //--------------------------------------------------------------------------
    static Error request(const Id id,
                         const RequestType& request,
                         ResponseType& response)
    {
        Service* service = find(id);

        if (isValidPointer(service))
        {
            return Error(ERROR_CODE_SERVICE_INVALID);
        }

        Error error = service->request(request, response);

        return error;
    }

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    Service(const Id id, ServiceCallback& callback) :
        myId(id),
        myCallback(callback)
    {
        Service* pointer = this;

        myServiceList.append(pointer);
    }

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------
    
    //--------------------------------------------------------------------------
    virtual ~Service()
    {
        Service* pointer = this;

        myServiceList.remove(pointer);
    }

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    Id getId() const
    {
        return myId;
    }

    //--------------------------------------------------------------------------
    Error request(const RequestType& request, ResponseType& response)
    {
        return (myCallback.call(request, response));
    }

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    static List<Service*> myServiceList;

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    const Id myId;

    ServiceCallback& myCallback;
};

template <typename RequestType, typename ResponseType>
List< Service<RequestType, ResponseType>* >
                              Service<RequestType, ResponseType>::myServiceList;

}; // namespace Plat4m

#endif // PLAT4M_SERVICE_H
