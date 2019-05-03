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
/// @file UnitTest.cpp
/// @author Ben Minerd
/// @date 4/19/2016
/// @brief UnitTest class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <UnitTest.h>

using Plat4m::UnitTest;

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
const char* UnitTest::getName() const
{
    return myName;
}

//------------------------------------------------------------------------------
uint32_t UnitTest::getTestCount() const
{
    return (myTestCallbackFunctionArray.getSize());
}

//------------------------------------------------------------------------------
uint32_t UnitTest::runTests()
{
    uint32_t nTests = myTestCallbackFunctionArray.getSize();
    uint32_t nPassedTests = 0;

    printf("%s\n", myName);
    printf("------------------------------\n");

    for (int i = 0; i < nTests; i++)
    {
        printf("Test %d/%d: ", (i + 1), nTests);

        bool passed = (*(myTestCallbackFunctionArray[i]))();

        if (passed)
        {
            nPassedTests++;
        }
    }

    printf("------------------------------\n");
    printf("%d/%d tests passed\n\n", nPassedTests, nTests);

    return nPassedTests;
}

//------------------------------------------------------------------------------
UnitTest::Error UnitTest::runTest(const uint32_t index, bool& passed)
{
    if (index >= myTestCallbackFunctionArray.getSize())
    {
        return ERROR_CODE_INVALID_TEST_INDEX;
    }

    passed = (*(myTestCallbackFunctionArray[index]))();

    return ERROR_CODE_NONE;
}

//------------------------------------------------------------------------------
// Protected constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
UnitTest::UnitTest(const char* name,
                   const TestCallbackFunction testCallbackFunctions[],
                   const unsigned int nTestCallbackFunctions) :
    myName(name),
    myTestCallbackFunctionArray(testCallbackFunctions, nTestCallbackFunctions)
{
}

//------------------------------------------------------------------------------
// Protected virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
UnitTest::~UnitTest()
{
}
