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
/// @file AdcUnitTest.cpp
/// @author Ben Minerd
/// @date 4/21/16
/// @brief AdcUnitTest class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <AdcUnitTest.h>

using Plat4m::AdcUnitTest;
using Plat4m::Adc;
using Plat4m::UnitTest;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const UnitTest::TestCallbackFunction AdcUnitTest::myTestCallbackFunctions[] =
{
    &AdcUnitTest::configureTest1,
    &AdcUnitTest::configureTest2,
    &AdcUnitTest::configureTest3,

    &AdcUnitTest::readVoltageTest1,
    &AdcUnitTest::readVoltageTest2,
    &AdcUnitTest::readVoltageTest3
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
AdcUnitTest::AdcUnitTest() :
    Adc(),
    UnitTest("AdcUnitTest",
             myTestCallbackFunctions,
             ARRAY_SIZE(myTestCallbackFunctions)),
    myWasDriverConfigureCalled(false),
    myDriverConfigureError(Adc::ERROR_CODE_NONE),
    myDriverReadVoltageError(Adc::ERROR_CODE_NONE),
    myVoltage(0.0)
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
AdcUnitTest::~AdcUnitTest()
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool AdcUnitTest::configureTest1()
{
    //
    // Procedure: call configure() without first calling enable(true)
    //
    // Test: error.getCode() should be ERROR_CODE_NOT_ENABLED
    //

    // Setup

    AdcUnitTest accel;

    Adc::Config config;

    // Operation

    Adc::Error error = accel.configure(config);

    // Test

    return UNIT_TEST_REPORT(
            UNIT_TEST_CASE_EQUAL(error.getCode(), Adc::ERROR_CODE_NOT_ENABLED) &
            UNIT_TEST_CASE_EQUAL(accel.myWasDriverConfigureCalled, false));
}

//------------------------------------------------------------------------------
bool AdcUnitTest::configureTest2()
{
    //
    // Procedure: call configure() after first calling enable(true)
    //
    // Test: error.getCode() should be ERROR_CODE_NONE
    //

    // Setup

    AdcUnitTest accel;
    accel.enable(true);

    Adc::Config config;

    // Operation

    Adc::Error error = accel.configure(config);

    // Test

    return UNIT_TEST_REPORT(
                  UNIT_TEST_CASE_EQUAL(error.getCode(), Adc::ERROR_CODE_NONE) &
                  UNIT_TEST_CASE_EQUAL(accel.myWasDriverConfigureCalled, true));
}

//------------------------------------------------------------------------------
bool AdcUnitTest::configureTest3()
{
    //
    // Procedure: call configure() with driver error
    //
    // Test: error.getCode() should be Adc::ERROR_CODE_COMMUNICATION_FAILED
    //

    // Setup

    AdcUnitTest accel;
    accel.enable(true);
    accel.myDriverConfigureError =
                               Adc::Error(Adc::ERROR_CODE_COMMUNICATION_FAILED);

    Adc::Config config;

    // Operation

    Adc::Error error = accel.configure(config);

    // Test

    return UNIT_TEST_REPORT(
                    UNIT_TEST_CASE_EQUAL(error.getCode(),
                                         Adc::ERROR_CODE_COMMUNICATION_FAILED));
}

//------------------------------------------------------------------------------
bool AdcUnitTest::readVoltageTest1()
{
    //
    // Procedure: call readVoltage() without first calling enable(true)
    //
    // Test: error code should be ERROR_CODE_NOT_ENABLED
    //

    // Setup

    AdcUnitTest accel;

    float voltage = 1.0;

    // Operation

    Adc::Error error = accel.readVoltage(voltage);

    // Test

    return UNIT_TEST_REPORT(
            UNIT_TEST_CASE_EQUAL(error.getCode(), Adc::ERROR_CODE_NOT_ENABLED) &
            UNIT_TEST_CASE_EQUAL_FLOAT(voltage, (float) 1.0));
}

//------------------------------------------------------------------------------
bool AdcUnitTest::readVoltageTest2()
{
    //
    // Procedure: call readVoltage() after first calling enable(true)
    //
    // Test: error code should be ERROR_CODE_NONE
    //

    // Setup

    AdcUnitTest accel;
    accel.enable(true);
    accel.myVoltage = 1.0;

    float voltage = 0.0;

    // Operation

    Adc::Error error = accel.readVoltage(voltage);

    // Test

    return UNIT_TEST_REPORT(
                   UNIT_TEST_CASE_EQUAL(error.getCode(), Adc::ERROR_CODE_NONE) &
                   UNIT_TEST_CASE_EQUAL_FLOAT(voltage, (float) 1.0));
}

//------------------------------------------------------------------------------
bool AdcUnitTest::readVoltageTest3()
{
    //
    // Procedure: call readVoltage() with driver error
    //
    // Test: error.getCode() should be Adc::ERROR_CODE_COMMUNICATION_FAILED
    //

    // Setup

    AdcUnitTest accel;
    accel.enable(true);
    accel.myDriverReadVoltageError =
                               Adc::Error(Adc::ERROR_CODE_COMMUNICATION_FAILED);
    accel.myVoltage = 1.0;

    float voltage = 0.0;

    // Operation

    Adc::Error error = accel.readVoltage(voltage);

    // Test

    return UNIT_TEST_REPORT(
                    UNIT_TEST_CASE_EQUAL(error.getCode(),
                                         Adc::ERROR_CODE_COMMUNICATION_FAILED) &
                    UNIT_TEST_CASE_EQUAL_FLOAT(voltage, (float) 0.0));
}

//------------------------------------------------------------------------------
// Private virtual methods implemented from Adc
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Adc::Error AdcUnitTest::driverConfigure(const Config& config)
{
    myWasDriverConfigureCalled = true;

    return myDriverConfigureError;
}

//------------------------------------------------------------------------------
Adc::Error AdcUnitTest::driverReadVoltage(float& voltage)
{
    voltage = myVoltage;

    return myDriverReadVoltageError;
}
