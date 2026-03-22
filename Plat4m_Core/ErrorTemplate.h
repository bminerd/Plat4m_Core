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
// Copyright (c) 2015-2024 Benjamin Minerd
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
/// @file ErrorTemplate.h
/// @author Ben Minerd
/// @date 5/25/15
/// @brief ErrorTemplate header file.
///

#ifndef PLAT4M_ERROR_TEMPLATE_H
#define PLAT4M_ERROR_TEMPLATE_H

//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------

// Support for MSVC
#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>

#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/ErrorBase.h>
#include <Plat4m_Core/ErrorManager.h>
#include <Plat4m_Core/MemoryAllocator.h>
#include <Plat4m_Core/MemoryManager.h>
#include <Plat4m_Core/Base.h>
#include <Plat4m_Core/Callback.h>
#include <Plat4m_Core/List.h>

//------------------------------------------------------------------------------
// Macros
//------------------------------------------------------------------------------

#define PLAT4M_REPORT_ERROR(errorType, code, severity, instance) \
    errorType(code,                                              \
              severity,                                          \
              reinterpret_cast<std::uintptr_t>(instance),        \
              #errorType,                                        \
              #code,                                             \
              #severity,                                         \
              __FILE__,                                          \
              typeid(*instance).name(),                          \
              __PRETTY_FUNCTION__,                               \
              __LINE__)

#define PLAT4M_REPORT_ERROR_STATIC(errorType, code, severity, module) \
    errorType(code,                                                   \
              severity,                                               \
              0,                                                      \
              #errorType,                                             \
              #code,                                                  \
              #severity,                                              \
              __FILE__,                                               \
              #module,                                                \
              __PRETTY_FUNCTION__,                                    \
              __LINE__)

#define PLAT4M_REPORT_ERROR_BARE(code, severity) \
    ErrorManager::reportGlobalError(code,        \
                                    severity,    \
                                    0,           \
                                    #code,       \
                                    #severity,   \
                                    "(Unknown)", \
                                    "(Unknown)"  \
                                    "(Unknown)", \
                                    0)

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

template <typename TCode>
class ErrorTemplate : public ErrorBase
{
public:

    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------

    typedef Callback<void, const ErrorTemplate<TCode>&> ErrorCallback;

    //--------------------------------------------------------------------------
    // Public static methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    static void registerCallback(ErrorCallback& callback,
                                 const void* instance = 0)
    {
        if (isNullPointer(myErrorCallbackList))
        {
            myErrorCallbackList =
                           MemoryAllocator::allocate<List<ErrorCallbackInfo>>();

            Base* basePointer = myErrorCallbackList;
            Base** basePointer2 = &basePointer;

            MemoryManager::addBasePointerForDeletion(basePointer2);
        }

        ErrorCallbackInfo info;
        info.callback = &callback;
        info.instanceId = reinterpret_cast<uintptr_t>(instance);

        myErrorCallbackList->append(info);
    }

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    ErrorTemplate() :
        ErrorBase()
    {
    }

    //--------------------------------------------------------------------------
    explicit ErrorTemplate(const TCode code) :
        ErrorBase()
    {
        setCode(code);
        setModuleId(computeModuleId());
    }

    //--------------------------------------------------------------------------
    ErrorTemplate(const TCode code,
                  const Severity& severity,
                  const InstanceId& instanceId) :
        ErrorBase(code, severity, computeModuleId(), instanceId)
    {
    }

    //--------------------------------------------------------------------------
    ErrorTemplate(const TCode code,
                  const Severity& severity,
                  const InstanceId& instanceId,
                  const char* errorString,
                  const char* codeString,
                  const char* severityString,
                  const char* fileString,
                  const char* moduleString,
                  const char* functionString,
                  const std::uint32_t lineNumber) :
        ErrorBase(code, severity, computeModuleId(), instanceId)
    {
        if ((code != 0)                   &&
            ErrorManager::isInitialized() &&
            (severity >= ErrorManager::getMinReportingSeverity()))
        {
            callErrorHandlers();

            ErrorManager::ErrorReport report;
            report.code           = code;
            report.severity       = severity;
            report.instanceId     = instanceId;
            report.errorString    = errorString;
            report.codeString     = codeString;
            report.severityString = severityString;
            report.fileString     = fileString;
            report.moduleString   = moduleString;
            report.functionString = functionString;
            report.lineNumber     = lineNumber;

            ErrorManager::reportGlobalError(report);
        }
    }

    //--------------------------------------------------------------------------
    ErrorTemplate(const ErrorTemplate<TCode>& error) :
        ErrorBase(error)
    {
        // Don't log any errors
    }

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    bool operator==(const ErrorTemplate<TCode>& error) const
    {
        return ((getCode()) == (error.getCode()));
    }

    //--------------------------------------------------------------------------
    bool operator==(const TCode& code) const
    {
        return ((getCode()) == code);
    }

    //--------------------------------------------------------------------------
    bool operator!=(const ErrorTemplate<TCode>& error) const
    {
        return (!(operator==(error)));
    }

    //--------------------------------------------------------------------------
    bool operator!=(const TCode& code) const
    {
        return (!(operator==(code)));
    }

    //--------------------------------------------------------------------------
    TCode getCode() const
    {
        return static_cast<TCode>(ErrorBase::getCode());
    }

private:

    //--------------------------------------------------------------------------
    // Private types
    //--------------------------------------------------------------------------

    struct ErrorCallbackInfo
    {
        ErrorCallback* callback;
        ErrorBase::InstanceId instanceId;
    };

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    static List<ErrorCallbackInfo>* myErrorCallbackList;

    //--------------------------------------------------------------------------
    // Private static methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    static void moduleIdMethod()
    {
    }

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    void setCode(const TCode code)
    {
        ErrorBase::setCode(code);
    }

    //--------------------------------------------------------------------------
    ModuleId computeModuleId()
    {
        void (*pointer)() = &ErrorTemplate<TCode>::moduleIdMethod;

        std::uintptr_t intPointer = reinterpret_cast<std::uintptr_t>(pointer);

        return intPointer;
    }

    //--------------------------------------------------------------------------
    void callErrorHandlers()
    {
        if (isValidPointer(myErrorCallbackList))
        {
            typename List<ErrorCallbackInfo>::Iterator iterator =
                                                myErrorCallbackList->iterator();

            while (iterator.hasCurrent())
            {
                ErrorCallbackInfo& info = iterator.current();

                if ((info.instanceId == 0) ||
                    (ErrorBase::getInstanceId() == info.instanceId))
                {
                    info.callback->call(*this);
                }

                iterator.next();
            }
        }
    }
};

template <typename TCode>
List<typename ErrorTemplate<TCode>::ErrorCallbackInfo>*
                                  ErrorTemplate<TCode>::myErrorCallbackList = 0;

}; // namespace Plat4m

#endif // PLAT4M_ERROR_TEMPLATE_H
