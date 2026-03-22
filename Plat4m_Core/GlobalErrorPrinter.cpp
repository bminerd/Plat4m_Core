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
/// @date 4/18/2023
/// @brief ErrorBase class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdio>
#include <stdexcept>

#include <Plat4m_Core/GlobalErrorPrinter.h>
#include <Plat4m_Core/ErrorManager.h>
#include <Plat4m_Core/ByteArrayN.h>
#include <Plat4m_Core/Printer.h>
#include <Plat4m_Core/CallbackMethod.h>

using namespace Plat4m;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GlobalErrorPrinter::GlobalErrorPrinter(ErrorManager& errorManager)
{
    errorManager.setGlobalErrorCallback(
                createCallback(this, &GlobalErrorPrinter::globalErrorCallback));
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void GlobalErrorPrinter::globalErrorCallback(
                                        const ErrorManager::ErrorReport& report)
{
    bool isError = (report.severity == ErrorBase::SEVERITY_CRITICAL);

    Printer::printFormat<1024>(
                          "\n\n"
                          "*** ERROR ***\n"
                          "--------------------------------------------------\n"
                          "Type:\t\t%s\n"
                          "Code:\t\t%s (%u)\n"
                          "Severity:\t%s (%u)\n"
                          "Module:\t\t%s\n"
                          "Instance:\t%p\n"
                          "File:\t\t%s:%u\n"
                          "Function:\t%s\n"
                          "--------------------------------------------------\n"
                          "\n",
                          false,
                          isError,
                          report.errorString,
                          report.codeString,
                          report.code,
                          report.severityString,
                          report.severity,
                          report.moduleString,
                          reinterpret_cast<void*>(report.instanceId),
                          report.fileString,
                          report.lineNumber,
                          report.functionString);

    if (isError)
    {
        const char* exitString =
                              "*** CRITICAL ERROR, EXITING APPLICATION ***\n\n";

        Printer::print(exitString, true, true);

        throw std::runtime_error(exitString);
    }
}
