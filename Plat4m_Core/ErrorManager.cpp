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
/// @file ErrorManager.cpp
/// @author Ben Minerd
/// @date 3/16/2023
/// @brief ErrorManager class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/ErrorTemplate.h>

#include <Plat4m_Core/ErrorManager.h>

using namespace Plat4m;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

ErrorManager* ErrorManager::myInstance = 0;

//------------------------------------------------------------------------------
// ErrorManager::Config - Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ErrorManager::Config::Config() :
    minReportingSeverity(ErrorBase::SEVERITY_LOW)
{
}

//------------------------------------------------------------------------------
ErrorManager::Config::Config(const ErrorBase::Severity& minReportingSeverity) :
    minReportingSeverity(minReportingSeverity)
{
}

//------------------------------------------------------------------------------
// Public static methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool ErrorManager::isInitialized()
{
    return (isValidPointer(myInstance));
}

//------------------------------------------------------------------------------
void ErrorManager::reportGlobalError(const ErrorReport& report)
{
    if (isValidPointer(myInstance))
    {
        myInstance->reportGlobalErrorPrivate(report);
    }
}

//------------------------------------------------------------------------------
ErrorBase::Severity ErrorManager::getMinReportingSeverity()
{
    if (isValidPointer(myInstance))
    {
        return (myInstance->myConfig.minReportingSeverity);
    }

    return (ErrorManager::Config().minReportingSeverity);
}

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ErrorManager::ErrorManager() :
    myGlobalErrorCallback(0)
{
    if (isValidPointer(myInstance))
    {
        PLAT4M_REPORT_ERROR(ErrorManager::Error,
                            ErrorManager::ERROR_CODE_INSTANCE_ALREADY_CREATED,
                            ErrorBase::SEVERITY_CRITICAL,
                            this);
    }

    myInstance = this;
}

//------------------------------------------------------------------------------
ErrorManager::ErrorManager(GlobalErrorCallback& callback) :
    myGlobalErrorCallback(&callback)
{
    if (isValidPointer(myInstance))
    {
        PLAT4M_REPORT_ERROR(ErrorManager::Error,
                            ErrorManager::ERROR_CODE_INSTANCE_ALREADY_CREATED,
                            ErrorBase::SEVERITY_CRITICAL,
                            this);
    }

    myInstance = this;
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ErrorManager::~ErrorManager()
{
    myInstance = 0;
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ErrorManager::setGlobalErrorCallback(GlobalErrorCallback& callback)
{
    myGlobalErrorCallback = &callback;
}

//------------------------------------------------------------------------------
void ErrorManager::setConfig(const Config& config)
{
    myConfig = config;
}

//------------------------------------------------------------------------------
ErrorManager::Config ErrorManager::getConfig() const
{
    return myConfig;
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ErrorManager::reportGlobalErrorPrivate(const ErrorReport& report)
{
    if (isValidPointer(myGlobalErrorCallback))
    {
        myGlobalErrorCallback->call(report);
    }
}
