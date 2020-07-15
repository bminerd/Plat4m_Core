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
/// @file ApplicationUnitTestApp.h
/// @author Ben Minerd
/// @date 6/4/2019
/// @brief ApplicationUnitTestApp class header file.
///

#ifndef PLAT4M_APPLICATION_UNIT_TEST_APP_H
#define PLAT4M_APPLICATION_UNIT_TEST_APP_H

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include <Plat4m_Core/Application.h>
#include <Plat4m_Core/UnitTest/UnitTest.h>
#include <Plat4m_Core/AllocationMemoryLite/AllocationMemoryLite.h>
#include <Plat4m_Core/List.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class ApplicationUnitTestApp : public Application
{
public:

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    ApplicationUnitTestApp(const char* name,
                           const char* productName,
                           const char* version);

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~ApplicationUnitTestApp();

protected:

    //--------------------------------------------------------------------------
    // Protected methods
    //--------------------------------------------------------------------------

    void addUnitTest(UnitTest& unitTest);

    void runParentApplication();

    void runTests();

    UnitTest::Error runTest(const uint32_t moduleIndex,
                            const uint32_t testIndex,
                            bool& passed);

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    // Constants

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    AllocationMemoryLite<4096> myAllocationMemory;

    List<UnitTest*> myUnitTestList;

    //--------------------------------------------------------------------------
    // Private methods implemented from Application
    //--------------------------------------------------------------------------

    // virtual void driverRun();

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------

    void initializeSystem();
};

}; // namespace Plat4m

#endif // PLAT4M_APPLICATION_UNIT_TEST_APP_H
