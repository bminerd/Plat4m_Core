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
/// @file UnitTestRunner.cpp
/// @author Ben Minerd
/// @date 4/12/2016
/// @brief UnitTestRunner class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <UnitTestRunner.h>
#include <ByteArrayN.h>
#include <System.h>

using Plat4m::UnitTestRunner;
using Plat4m::UnitTest;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const char* UnitTestRunner::myName        = "UNIT_TEST_RUNNER";
const char* UnitTestRunner::myProductName = "PLAT4M";
const char* UnitTestRunner::myVersion     = "0.0.1";

//------------------------------------------------------------------------------
// Protected constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
UnitTestRunner::UnitTestRunner(const char* name,
                                   const char* productName,
                                   const char* version) :
    Application(name, productName, version),
    myAllocationMemory(),
    mySystem(),
    myTestThread(System::createThread(
                    createCallback(this, &UnitTestRunner::testThreadCallback))),
    myUnitTestList()
{
}

//------------------------------------------------------------------------------
// Protected virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
UnitTestRunner::~UnitTestRunner()
{
}

//------------------------------------------------------------------------------
// Protected methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void UnitTestRunner::addUnitTest(UnitTest& unitTest)
{
    UnitTest* pointer = &unitTest;

    myUnitTestList.append(pointer);
}

//------------------------------------------------------------------------------
void UnitTestRunner::runParentApplication()
{
	initializeSystem();

	mySystem.run();
}

//------------------------------------------------------------------------------
UnitTest::Error UnitTestRunner::runTest(const uint32_t moduleIndex,
                                        const uint32_t testIndex,
                                        bool& passed)
{
    if (moduleIndex >= myUnitTestList.size())
    {
        return UnitTest::ERROR_CODE_INVALID_TEST_INDEX;
    }

    uint32_t currentIndex = 0;

    List<UnitTest*>::Iterator iterator = myUnitTestList.iterator();

    while (iterator.hasCurrent() && (currentIndex != moduleIndex))
    {
        iterator.next();
        currentIndex++;
    }

    UnitTest* unitTest = iterator.current();
    return unitTest->runTest(testIndex, passed);
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void UnitTestRunner::initializeSystem()
{
    myTestThread.enable(true);
}

//------------------------------------------------------------------------------
void UnitTestRunner::testThreadCallback()
{
    uint32_t nTotalTests = 0;
    uint32_t nTotalPassedTests = 0;

    List<UnitTest*>::Iterator iterator = myUnitTestList.iterator();

    while (iterator.hasCurrent())
    {
        UnitTest* unitTest = iterator.current();
        nTotalTests += unitTest->getTestCount();

        nTotalPassedTests += unitTest->runTests();

        iterator.next();
    }

    printf("\nTotal results\n");
    printf("------------------------------\n");
    printf("%d/%d tests passed\n", nTotalPassedTests, nTotalTests);

    myTestThread.enable(false);
}
