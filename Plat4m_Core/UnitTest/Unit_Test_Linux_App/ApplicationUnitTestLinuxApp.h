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
// Copyright (c) 2019 Benjamin Minerd
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
/// @file ApplicationUnitTestLinuxApp.h
/// @author Ben Minerd
/// @date 6/4/2019
/// @brief ApplicationUnitTestLinuxApp class header file.
///

#ifndef PLAT4M_APPLICATION_UNIT_TEST_LINUX_APP_H
#define PLAT4M_APPLICATION_UNIT_TEST_LINUX_APP_H

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include <Plat4m_Core/UnitTest/ApplicationUnitTestApp.h>
#include <Plat4m_Core/Linux/SystemLinux.h>
#include <Plat4m_Core/Linux/ProcessorLinux.h>
#include <Plat4m_Core/UnitTest/ArrayUnitTest.h>
#include <Plat4m_Core/UnitTest/ArrayNUnitTest.h>
#include <Plat4m_Core/UnitTest/BufferUnitTest.h>
#include <Plat4m_Core/UnitTest/ByteArrayUnitTest.h>
#include <Plat4m_Core/UnitTest/ModuleUnitTest.h>
#include <Plat4m_Core/UnitTest/TimeStampUnitTest.h>
#include <Plat4m_Core/UnitTest/ListUnitTest.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class ApplicationUnitTestLinuxApp : public ApplicationUnitTestApp
{
public:

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    ApplicationUnitTestLinuxApp();

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~ApplicationUnitTestLinuxApp();

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    SystemLinux mySystem;

    ProcessorLinux myProcessor;

    ArrayUnitTest myArrayUnitTest;
    ArrayNUnitTest myArrayNUnitTest;
    BufferUnitTest myBufferUnitTest;
    ByteArrayUnitTest myByteArrayUnitTest;
    ModuleUnitTest myModuleUnitTest;
    TimeStampUnitTest myTimeStampUnitTest;
    ListUnitTest myListUnitTest;

    //--------------------------------------------------------------------------
    // Private methods implemented from Application
    //--------------------------------------------------------------------------

    void driverRun();

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------

    void initializeProcessor();

    void initializeSystem();
};

}; // namespace Plat4m

#endif // PLAT4M_APPLICATION_UNIT_TEST_LINUX_APP_H
