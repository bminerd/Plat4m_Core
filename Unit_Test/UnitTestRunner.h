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
// Copyright (c) 2016 Benjamin Minerd
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
/// @file UnitTestRunner.h
/// @author Ben Minerd
/// @date 4/15/2016
/// @brief UnitTestRunner class header file.
///

#ifndef UNIT_TEST_RUNNER_H
#define UNIT_TEST_RUNNER_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

// Plat4m includes
#include <Plat4m.h>
#include <Application.h>
#include <List.h>
#include <UnitTest.h>
#include <AllocationMemoryLite.h>
#include <SystemLite.h>
#include <Thread.h>
#include <ComLink.h>
#include <ComProtocolPlat4mAscii.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

///
/// @brief
///
class UnitTestRunner : public Application
{
public:

protected:

    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------

    ///
    /// @brief Constructor for UnitTestRunner class.
    ///
    UnitTestRunner(const char* name,
                   const char* productName,
                   const char* version);

    //--------------------------------------------------------------------------
    // Protected virtual destructors
    //--------------------------------------------------------------------------

    ///
    /// @brief Virtual destructor for UnitTestRunner class.
    ///
    virtual ~UnitTestRunner();

    //--------------------------------------------------------------------------
    // Protected methods
    //--------------------------------------------------------------------------

    void addUnitTest(UnitTest& unitTest);

    void runParentApplication();

    UnitTest::Error runTest(const uint32_t moduleIndex,
                            const uint32_t testIndex,
                            bool& passed);

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    static const char* myName;

    static const char* myProductName;

    static const char* myVersion;

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    AllocationMemoryLite<1024> myAllocationMemory;

    SystemLite mySystem;

    Thread& myTestThread;

    List<UnitTest*> myUnitTestList;

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------

	void initializeSystem();

	void testThreadCallback();
};

}; // namespace Plat4m

#endif // UNIT_TEST_RUNNER_H
