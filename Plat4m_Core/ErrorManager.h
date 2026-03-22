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
/// @file ErrorManager.h
/// @author Ben Minerd
/// @date 3/16/2023
/// @brief ErrorManager class header file.
///

#ifndef PLAT4M_ERROR_MANAGER_H
#define PLAT4M_ERROR_MANAGER_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>

#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/ErrorBase.h>
#include <Plat4m_Core/Callback.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Forward class declarations
//------------------------------------------------------------------------------

// Declared here to avoid #include loop
template <typename TCode>
class ErrorTemplate;

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class ErrorManager
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

    using Error = ErrorTemplate<ErrorCode>;

    typedef Callback<void, std::uint32_t> ErrorCallback;

    struct ErrorReport
    {
        std::uint32_t code;
        ErrorBase::Severity severity;
        ErrorBase::InstanceId instanceId;
        const char* errorString;
        const char* codeString;
        const char* severityString;
        const char* fileString;
        const char* moduleString;
        const char* functionString;
        std::uint32_t lineNumber;
    };

    typedef Callback<void, const ErrorReport&> GlobalErrorCallback;

    struct Config
    {
        ErrorBase::Severity minReportingSeverity;

        //----------------------------------------------------------------------
        // Public constructors
        //----------------------------------------------------------------------

        Config();

        Config(const ErrorBase::Severity& minReportingSeverity);
    };

    //--------------------------------------------------------------------------
    // Public static methods
    //--------------------------------------------------------------------------

    static void reportGlobalError(const ErrorReport& report);

    static bool isInitialized();

    static ErrorBase::Severity getMinReportingSeverity();

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    ErrorManager();

    ErrorManager(GlobalErrorCallback& callback);

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~ErrorManager();

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    void setGlobalErrorCallback(GlobalErrorCallback& callback);

    void setConfig(const Config& config);

    Config getConfig() const;

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    static ErrorManager* myInstance;

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    GlobalErrorCallback* myGlobalErrorCallback;

    Config myConfig;

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------

    void reportGlobalErrorPrivate(const ErrorReport& report);
};

}; // namespace Plat4m

#endif // PLAT4M_ERROR_MANAGER_H
