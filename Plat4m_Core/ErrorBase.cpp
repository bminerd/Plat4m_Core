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
/// @file ErrorBase.cpp
/// @author Ben Minerd
/// @date 3/16/2023
/// @brief ErrorBase class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/ErrorBase.h>
#include <Plat4m_Core/ErrorManager.h>

using namespace Plat4m;

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ErrorBase::~ErrorBase()
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ErrorBase& ErrorBase::operator=(const ErrorBase& errorBase)
{
    myCode = errorBase.myCode;
    myModuleId = errorBase.myModuleId;
    myInstanceId = errorBase.myInstanceId;

    return (*this);
}

//------------------------------------------------------------------------------
ErrorBase::Code ErrorBase::getCode() const
{
    return myCode;
}

//------------------------------------------------------------------------------
ErrorBase::Severity ErrorBase::getSeverity() const
{
    return mySeverity;
}

//------------------------------------------------------------------------------
ErrorBase::ModuleId ErrorBase::getModuleId() const
{
    return myModuleId;
}

//------------------------------------------------------------------------------
ErrorBase::InstanceId ErrorBase::getInstanceId() const
{
    return myInstanceId;
}

//------------------------------------------------------------------------------
// Protected constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ErrorBase::ErrorBase() :
    myCode(0),
    mySeverity(SEVERITY_LOW),
    myModuleId(0),
    myInstanceId(0)
{
}

//------------------------------------------------------------------------------
ErrorBase::ErrorBase(const Code& code) :
    myCode(),
    mySeverity(SEVERITY_LOW),
    myModuleId(0),
    myInstanceId(0)
{
    setCode(code);
}

//------------------------------------------------------------------------------
ErrorBase::ErrorBase(const Code& code,
                     const Severity& severity,
                     const ModuleId& moduleId,
                     const InstanceId& instanceId) :
    myCode(),
    mySeverity(severity),
    myModuleId(moduleId),
    myInstanceId(instanceId)
{
    setCode(code);
}

//------------------------------------------------------------------------------
ErrorBase::ErrorBase(const ErrorBase& error) :
    myCode(error.myCode),
    mySeverity(error.mySeverity),
    myModuleId(error.myModuleId),
    myInstanceId(error.myInstanceId)
{
}

//------------------------------------------------------------------------------
// Protected methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ErrorBase::setCode(const Code& code)
{
    myCode = code;
}

//------------------------------------------------------------------------------
void ErrorBase::setSeverity(const Severity& severity)
{
    mySeverity = severity;
}

//------------------------------------------------------------------------------
void ErrorBase::setModuleId(const ModuleId& id)
{
    myModuleId = id;
}

//------------------------------------------------------------------------------
void ErrorBase::setInstanceId(const InstanceId& id)
{
    myInstanceId = id;
}
