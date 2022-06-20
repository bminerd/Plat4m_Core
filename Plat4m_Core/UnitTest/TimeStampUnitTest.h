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
// Copyright (c) 2022 Benjamin Minerd
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
/// @file TimeStampUnitTest.h
/// @author Ben Minerd
/// @date 6/15/2022
/// @brief TimeStampUnitTest class header file.
///

#ifndef PLAT4M_TIME_STAMP_UNIT_TEST_H
#define PLAT4M_TIME_STAMP_UNIT_TEST_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/TimeStamp.h>
#include <Plat4m_Core/UnitTest/UnitTest.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class TimeStampUnitTest : public UnitTest
{
public:
    
    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    TimeStampUnitTest();

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~TimeStampUnitTest();

    //--------------------------------------------------------------------------
    // Public static methods
    //--------------------------------------------------------------------------

    static bool operatorGreaterThanTest();

    static bool operatorLessThanTest();

    static bool operatorEqualsTest();

    static bool operatorGreaterThanOrEqualToTest();

    static bool operatorLessThanOrEqualToTest();

    static bool operatorAddTest1();

    static bool operatorAddTest2();

    static bool operatorAddTest3();

    static bool operatorAddEqualsTest1();

    static bool operatorAddEqualsTest2();

    static bool operatorSubtractTest1();

    static bool operatorSubtractTest2();

    static bool operatorSubtractTest3();

    static bool operatorSubtractEqualsTest1();

    static bool operatorSubtractEqualsTest2();

    static bool fromTimeMsTest();

    static bool fromTimeUsTest();

    static bool fromTimeNsTest();

    static bool toTimeMsTest1();

    static bool toTimeMsTest2();

    static bool toTimeMsTest3();

    static bool toTimeMsTest4();

    static bool toTimeUsTest1();

    static bool toTimeUsTest2();

    static bool toTimeUsTest3();

    static bool toTimeUsTest4();

    static bool toTimeNsTest1();

    static bool toTimeNsTest2();

    static bool toTimeNsTest3();

    static bool toTimeNsTest4();

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    static const UnitTest::TestCallbackFunction myTestCallbackFunctions[];
};

}; // namespace Plat4m

#endif // PLAT4M_TIME_STAMP_UNIT_TEST_H
