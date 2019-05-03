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
/// @file UnitTest.h
/// @author Ben Minerd
/// @date 4/19/2016
/// @brief UnitTest class header file.
///

#ifndef UNIT_TEST_H
#define UNIT_TEST_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

// Plat4m includes
#include <Array.h>
#include <Callback.h>
#include <ErrorTemplate.h>

#include <stdio.h>
#include <float.h>
#include <math.h>

//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------

#define UNIT_TEST_REPORT(passed) \
    UnitTest::testReport(printf("%s():", __func__) && (passed))

#define UNIT_TEST_CASE_EQUAL(value, correctValue) \
    UnitTest::testCaseEqual(value,                \
                            correctValue,         \
                            #value,               \
                            #correctValue)


#define UNIT_TEST_CASE_EQUAL_MESSAGE(value, correctValue, errorMessage) \
    UnitTest::testCaseEqualMessage(value,                               \
                                   correctValue,                        \
                                   #value,                              \
                                   #correctValue,                       \
                                   errorMessage)

#define UNIT_TEST_CASE_EQUAL_FLOAT(value, correctValue) \
    UnitTest::testCaseEqualFloat(value,                 \
                                 correctValue,          \
                                 #value,                \
                                 #correctValue)


#define UNIT_TEST_CASE_EQUAL_FLOAT_MESSAGE(value, correctValue, errorMessage) \
    UnitTest::testCaseEqualFloatMessage(value,                                \
                                        correctValue,                         \
                                        #value,                               \
                                        #correctValue,                        \
                                        errorMessage)

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

///
/// @brief Base class for module unit tests.
///
class UnitTest
{
public:

    //--------------------------------------------------------------------------
    // Public enumerations
    //--------------------------------------------------------------------------

    enum ErrorCode
    {
        ERROR_CODE_NONE,
        ERROR_CODE_INVALID_TEST_INDEX
    };

    //--------------------------------------------------------------------------
    // Public typedefs
    //--------------------------------------------------------------------------

    typedef ErrorTemplate<ErrorCode> Error;

    typedef bool (*TestCallbackFunction)();

    //--------------------------------------------------------------------------
    // Public static methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    static bool testReport(const bool passed)
    {
        if (passed)
        {
            printf(" PASS\n");
        }
        else
        {
            printf("\nFAIL\n");
        }

        return passed;
    }

    //--------------------------------------------------------------------------
    template <typename TParameter>
    static bool testCaseEqual(const TParameter& value,
                              const TParameter& correctValue,
                              const char* valueName,
                              const char* correctValueName)
    {
        if (value != correctValue)
        {
            printf("\n- %s didn't equal %s", valueName, correctValueName);

            return false;
        }

        return true;
    }

    //--------------------------------------------------------------------------
    template <typename TParameter>
    static bool testCaseEqualMessage(const TParameter& value,
                                     const TParameter& correctValue,
                                     const char* valueName,
                                     const char* correctValueName,
                                     const char* errorMessage)
    {
        if (value != correctValue)
        {
            printf("\n- %s didn't equal %s (%s)",
                   valueName,
                   correctValueName,
                   errorMessage);

            return false;
        }

        return true;
    }

    //--------------------------------------------------------------------------
    static bool testCaseEqualFloat(const float& value,
                                   const float& correctValue,
                                   const char* valueName,
                                   const char* correctValueName)
    {
        if (fabs(correctValue - value) > FLT_EPSILON)
        {
            printf("\n- %s didn't equal %s", valueName, correctValueName);

            return false;
        }

        return true;
    }

    //--------------------------------------------------------------------------
    static bool testCaseEqualFloatMessage(const float& value,
                                          const float& correctValue,
                                          const char* valueName,
                                          const char* correctValueName,
                                          const char* errorMessage)
    {
        if (fabs(correctValue - value) > FLT_EPSILON)
        {
            printf("\n- %s didn't equal %s (%s)",
                   valueName,
                   correctValueName,
                   errorMessage);

            return false;
        }

        return true;
    }

    //--------------------------------------------------------------------------
    static bool testCaseEqualFloat(const double& value,
                                   const double& correctValue,
                                   const char* valueName,
                                   const char* correctValueName)
    {
        if (fabs(correctValue - value) > DBL_EPSILON)
        {
            printf("\n- %s didn't equal %s", valueName, correctValueName);

            return false;
        }

        return true;
    }

    //--------------------------------------------------------------------------
    static bool testCaseEqualFloatMessage(const double& value,
                                          const double& correctValue,
                                          const char* valueName,
                                          const char* correctValueName,
                                          const char* errorMessage)
    {
        if (fabs(correctValue - value) > DBL_EPSILON)
        {
            printf("\n- %s didn't equal %s (%s)",
                   valueName,
                   correctValueName,
                   errorMessage);

            return false;
        }

        return true;
    }

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    const char* getName() const;

    uint32_t getTestCount() const;

    uint32_t runTests();

    Error runTest(const uint32_t index, bool& passed);

protected:

    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------

    ///
    /// @brief Constructor for UnitTest class.
    ///
    UnitTest(const char* name,
             const TestCallbackFunction testCallbackFunctions[],
             const unsigned int nTestCallbackFunctions);

    //--------------------------------------------------------------------------
    // Protected virtual destructors
    //--------------------------------------------------------------------------

    ///
    /// @brief Virtual destructor for UnitTest class.
    ///
    virtual ~UnitTest();

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    const char* myName;

    Array<const TestCallbackFunction> myTestCallbackFunctionArray;
};

}; // namespace Plat4m

#endif // UNIT_TEST_H
