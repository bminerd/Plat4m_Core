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
// Copyright (c) 2022 Benjamin Minerd
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
/// @file ServiceManager.cpp
/// @author Ben Minerd
/// @date 10/25/2022
/// @brief ServiceManager class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/ServiceManager.h>

using namespace Plat4m;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

ServiceManager* ServiceManager::myInstance = 0;

//------------------------------------------------------------------------------
// Public static methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ServiceManager::add(ServiceBase& service)
{
    if (isNullPointer(myInstance))
    {
        PLAT4M_REPORT_ERROR_STATIC(
                                ServiceManager::Error,
                                ServiceManager::ERROR_CODE_INSTANCE_NOT_CREATED,
                                ErrorBase::SEVERITY_CRITICAL,
                                ServiceManager);
    }

    myInstance->addPrivate(service);
}

//------------------------------------------------------------------------------
void ServiceManager::remove(ServiceBase& service)
{
    if (isNullPointer(myInstance))
    {
        PLAT4M_REPORT_ERROR_STATIC(
                                ServiceManager::Error,
                                ServiceManager::ERROR_CODE_INSTANCE_NOT_CREATED,
                                ErrorBase::SEVERITY_CRITICAL,
                                ServiceManager);
    }

    myInstance->removePrivate(service);
}

//------------------------------------------------------------------------------
ServiceBase* ServiceManager::find(const Id id)
{
    if (isNullPointer(myInstance))
    {
        PLAT4M_REPORT_ERROR_STATIC(
                                ServiceManager::Error,
                                ServiceManager::ERROR_CODE_INSTANCE_NOT_CREATED,
                                ErrorBase::SEVERITY_CRITICAL,
                                ServiceManager);
    }

    return (myInstance->findPrivate(id));
}

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ServiceManager::ServiceManager()
{
    if (isValidPointer(myInstance))
    {
        PLAT4M_REPORT_ERROR_STATIC(
                            ServiceManager::Error,
                            ServiceManager::ERROR_CODE_INSTANCE_ALREADY_CREATED,
                            ErrorBase::SEVERITY_CRITICAL,
                            ServiceManager);
    }

    myInstance = this;
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ServiceManager::~ServiceManager()
{
    typename List<ServiceBase*>::Iterator iterator = myServiceList.iterator();

    while (iterator.hasCurrent())
    {
        ServiceBase* service = iterator.current();
        
        service->~ServiceBase();

        iterator.next();
    }

    myInstance = 0;
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ServiceManager::addPrivate(ServiceBase& service)
{
    ServiceBase* pointer = &service;

    myServiceList.append(pointer);
}

//------------------------------------------------------------------------------
void ServiceManager::removePrivate(ServiceBase& service)
{
    ServiceBase* pointer = &service;

    myServiceList.remove(pointer);
}

//------------------------------------------------------------------------------
ServiceBase* ServiceManager::findPrivate(const Id id)
{
    ServiceBase* existingService = 0;

    typename List<ServiceBase*>::Iterator iterator = myServiceList.iterator();

    while (iterator.hasCurrent())
    {
        ServiceBase* service = iterator.current();
        
        if (service->getId() == id)
        {
            existingService = service;

            break;
        }

        iterator.next();
    }

    return existingService;
}
