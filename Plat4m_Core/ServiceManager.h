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
/// @file ServiceManager.h
/// @author Ben Minerd
/// @date 10/25/2022
/// @brief ServiceManager class header file.
///

#ifndef PLAT4M_SERVICE_MANAGER_H
#define PLAT4M_SERVICE_MANAGER_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/ErrorTemplate.h>
#include <Plat4m_Core/ServiceBase.h>
#include <Plat4m_Core/List.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class ServiceManager
{
public:

    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------

    enum ErrorCode
    {
        ERROR_CODE_NONE = 0,
        ERROR_CODE_INSTANCE_NOT_CREATED,
        ERROR_CODE_INSTANCE_ALREADY_CREATED
    };

    typedef ErrorTemplate<ErrorCode> Error;

    //--------------------------------------------------------------------------
    // Public static methods
    //--------------------------------------------------------------------------

    static void add(ServiceBase& service);

    static void remove(ServiceBase& service);

    static ServiceBase* find(const ServiceBase::Id id);

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    ServiceManager();

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------
    
    virtual ~ServiceManager();

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    static ServiceManager* myInstance;

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    List<ServiceBase*> myServiceList;

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------

    void addPrivate(ServiceBase& service);

    void removePrivate(ServiceBase& service);

    ServiceBase* findPrivate(const ServiceBase::Id id);
};

}; // namespace Plat4m

#endif // PLAT4M_SERVICE_MANAGER_H
