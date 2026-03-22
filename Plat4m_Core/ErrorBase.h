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
/// @file ErrorBase.h
/// @author Ben Minerd
/// @date 3/16/2023
/// @brief ErrorBase header file.
///

#ifndef PLAT4M_ERROR_BASE_H
#define PLAT4M_ERROR_BASE_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>
#include <typeinfo>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class ErrorBase
{
public:

    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------

    enum Severity : std::uint8_t
    {
        SEVERITY_LOW = 0,
        SEVERITY_HIGH,
        SEVERITY_CRITICAL
    };

    enum Type
    {
        TYPE_EVENT = 0,
        TYPE_CONDITION
    };

    using Code = std::uint32_t;

    using ModuleId = std::uintptr_t;

    using InstanceId = std::uintptr_t;

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    ErrorBase& operator=(const ErrorBase& error);

    Code getCode() const;

    Severity getSeverity() const;

    ModuleId getModuleId() const;

    InstanceId getInstanceId() const;

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~ErrorBase();

protected:

    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------

    ErrorBase();

    explicit ErrorBase(const Code& code);

    ErrorBase(const Code& code,
              const Severity& severity,
              const ModuleId& moduleId,
              const InstanceId& instanceId);

    //--------------------------------------------------------------------------
    // Protected copy constructors
    //--------------------------------------------------------------------------

    ErrorBase(const ErrorBase& error);

    //--------------------------------------------------------------------------
    // Protected methods
    //--------------------------------------------------------------------------

    void setCode(const Code& code);

    void setSeverity(const Severity& severity);

    void setModuleId(const ModuleId& id);

    void setInstanceId(const InstanceId& id);

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    Code myCode;

    Severity mySeverity;

    ModuleId myModuleId;

    InstanceId myInstanceId;
};

}; // End namespace Plat4m

#endif // PLAT4M_ERROR_BASE_H
