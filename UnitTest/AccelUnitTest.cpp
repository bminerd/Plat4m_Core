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
/// @file AccelUnitTest.cpp
/// @author Ben Minerd
/// @date 4/21/16
/// @brief AccelUnitTest class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <AccelUnitTest.h>

using Plat4m::AccelUnitTest;
using Plat4m::Accel;
using Plat4m::UnitTest;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const UnitTest::TestCallbackFunction AccelUnitTest::myTestCallbackFunctions[] =
{
    &AccelUnitTest::configureTest1,
    &AccelUnitTest::configureTest2,
    &AccelUnitTest::configureTest3,

    &AccelUnitTest::getMeasurementTest1,
    &AccelUnitTest::getMeasurementTest2,
    &AccelUnitTest::getMeasurementTest3
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
AccelUnitTest::AccelUnitTest() :
    Accel(),
    UnitTest("AccelUnitTest",
             myTestCallbackFunctions,
             ARRAY_SIZE(myTestCallbackFunctions)),
    myWasDriverConfigureCalled(false),
    myDriverConfigureError(Accel::ERROR_CODE_NONE),
    myDriverGetMeasurementError(Accel::ERROR_CODE_NONE),
    myMeasurement()
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
AccelUnitTest::~AccelUnitTest()
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool AccelUnitTest::configureTest1()
{
    //
    // Procedure: call configure() without first calling enable(true)
    //
    // Test: error.getCode() should be ERROR_CODE_NOT_ENABLED
    //

    // Setup

    AccelUnitTest accel;

    Accel::Config config;

    // Operation

    Accel::Error error = accel.configure(config);

    // Test

    return UNIT_TEST_REPORT(
          UNIT_TEST_CASE_EQUAL(error.getCode(), Accel::ERROR_CODE_NOT_ENABLED) &
          UNIT_TEST_CASE_EQUAL(accel.myWasDriverConfigureCalled, false));
}

//------------------------------------------------------------------------------
bool AccelUnitTest::configureTest2()
{
    //
    // Procedure: call configure() after first calling enable(true)
    //
    // Test: error.getCode() should be ERROR_CODE_NONE
    //

    // Setup

    AccelUnitTest accel;
    accel.enable(true);

    Accel::Config config;

    // Operation

    Accel::Error error = accel.configure(config);

    // Test

    return UNIT_TEST_REPORT(
                 UNIT_TEST_CASE_EQUAL(error.getCode(), Accel::ERROR_CODE_NONE) &
                 UNIT_TEST_CASE_EQUAL(accel.myWasDriverConfigureCalled, true));
}

//------------------------------------------------------------------------------
bool AccelUnitTest::configureTest3()
{
    //
    // Procedure: call configure() with driver error
    //
    // Test: error.getCode() should be Accel::ERROR_CODE_COMMUNICATION_FAILED
    //

    // Setup

    AccelUnitTest accel;
    accel.enable(true);
    accel.myDriverConfigureError =
                           Accel::Error(Accel::ERROR_CODE_COMMUNICATION_FAILED);

    Accel::Config config;

    // Operation

    Accel::Error error = accel.configure(config);

    // Test

    return UNIT_TEST_REPORT(
                  UNIT_TEST_CASE_EQUAL(error.getCode(),
                                       Accel::ERROR_CODE_COMMUNICATION_FAILED));
}

//------------------------------------------------------------------------------
bool AccelUnitTest::getMeasurementTest1()
{
    //
    // Procedure: call getMeasurement() without first calling enable(true)
    //
    // Test: error code should be Accel::ERROR_CODE_NOT_ENABLED
    //

    // Setup

    AccelUnitTest accel;
    accel.myMeasurement.rawSamples[0] = 1;
    accel.myMeasurement.rawSamples[1] = 2;
    accel.myMeasurement.rawSamples[2] = 3;
    accel.myMeasurement.samples[0]    = 1.0;
    accel.myMeasurement.samples[1]    = 2.0;
    accel.myMeasurement.samples[2]    = 3.0;

    Accel::Measurement measurement;
    measurement.rawSamples[0] = 0;
    measurement.rawSamples[1] = 0;
    measurement.rawSamples[2] = 0;
    measurement.samples[0]    = 0.0;
    measurement.samples[1]    = 0.0;
    measurement.samples[2]    = 0.0;

    // Operation

    Accel::Error error = accel.getMeasurement(measurement);

    // Test

    return UNIT_TEST_REPORT(
          UNIT_TEST_CASE_EQUAL(error.getCode(), Accel::ERROR_CODE_NOT_ENABLED) &
          UNIT_TEST_CASE_EQUAL(measurement.rawSamples[0], (int16_t) 0)         &
          UNIT_TEST_CASE_EQUAL(measurement.rawSamples[1], (int16_t) 0)         &
          UNIT_TEST_CASE_EQUAL(measurement.rawSamples[2], (int16_t) 0)         &
          UNIT_TEST_CASE_EQUAL_FLOAT(measurement.samples[0], (float) 0.0)      &
          UNIT_TEST_CASE_EQUAL_FLOAT(measurement.samples[1], (float) 0.0)      &
          UNIT_TEST_CASE_EQUAL_FLOAT(measurement.samples[2], (float) 0.0));
}

//------------------------------------------------------------------------------
bool AccelUnitTest::getMeasurementTest2()
{
    //
    // Procedure: call getMeasurement() after first calling enable(true)
    //
    // Test: measurement.rawSamples[2] should be 3
    //

    // Setup

    AccelUnitTest accel;
    accel.enable(true);
    accel.myMeasurement.rawSamples[0] = 1;
    accel.myMeasurement.rawSamples[1] = 2;
    accel.myMeasurement.rawSamples[2] = 3;
    accel.myMeasurement.samples[0]    = 1.0;
    accel.myMeasurement.samples[1]    = 2.0;
    accel.myMeasurement.samples[2]    = 3.0;

    Accel::Measurement measurement;
    measurement.rawSamples[0] = 0;
    measurement.rawSamples[1] = 0;
    measurement.rawSamples[2] = 0;
    measurement.samples[0]    = 0.0;
    measurement.samples[1]    = 0.0;
    measurement.samples[2]    = 0.0;

    // Operation

    Accel::Error error = accel.getMeasurement(measurement);

    // Test

    return UNIT_TEST_REPORT(
                 UNIT_TEST_CASE_EQUAL(error.getCode(), Accel::ERROR_CODE_NONE) &
                 UNIT_TEST_CASE_EQUAL(measurement.rawSamples[0],
                                      accel.myMeasurement.rawSamples[0])       &
                 UNIT_TEST_CASE_EQUAL(measurement.rawSamples[1],
                                      accel.myMeasurement.rawSamples[1])       &
                 UNIT_TEST_CASE_EQUAL(measurement.rawSamples[2],
                                      accel.myMeasurement.rawSamples[2])       &
                 UNIT_TEST_CASE_EQUAL_FLOAT(measurement.samples[0],
                                            accel.myMeasurement.samples[0])    &
                 UNIT_TEST_CASE_EQUAL_FLOAT(measurement.samples[1],
                                            accel.myMeasurement.samples[1])    &
                 UNIT_TEST_CASE_EQUAL_FLOAT(measurement.samples[2],
                                            accel.myMeasurement.samples[2]));
}

//------------------------------------------------------------------------------
bool AccelUnitTest::getMeasurementTest3()
{
    //
    // Procedure: call getMeasurement() with driver error
    //
    // Test: measurement.rawSamples[2] should be 0
    //

    // Setup

    AccelUnitTest accel;
    accel.enable(true);
    accel.myDriverGetMeasurementError =
                           Accel::Error(Accel::ERROR_CODE_COMMUNICATION_FAILED);
    accel.myMeasurement.rawSamples[0] = 1;
    accel.myMeasurement.rawSamples[1] = 2;
    accel.myMeasurement.rawSamples[2] = 3;
    accel.myMeasurement.samples[0]    = 1.0;
    accel.myMeasurement.samples[1]    = 2.0;
    accel.myMeasurement.samples[2]    = 3.0;

    Accel::Measurement measurement;
    measurement.rawSamples[0] = 0;
    measurement.rawSamples[1] = 0;
    measurement.rawSamples[2] = 0;
    measurement.samples[0]    = 0.0;
    measurement.samples[1]    = 0.0;
    measurement.samples[2]    = 0.0;

    // Operation

    Accel::Error error = accel.getMeasurement(measurement);

    // Test

    return UNIT_TEST_REPORT(
               UNIT_TEST_CASE_EQUAL(error.getCode(),
                                    Accel::ERROR_CODE_COMMUNICATION_FAILED)    &
               UNIT_TEST_CASE_EQUAL(measurement.rawSamples[0], (int16_t) 0)    &
               UNIT_TEST_CASE_EQUAL(measurement.rawSamples[1], (int16_t) 0)    &
               UNIT_TEST_CASE_EQUAL(measurement.rawSamples[2], (int16_t) 0)    &
               UNIT_TEST_CASE_EQUAL_FLOAT(measurement.samples[0], (float) 0.0) &
               UNIT_TEST_CASE_EQUAL_FLOAT(measurement.samples[1], (float) 0.0) &
               UNIT_TEST_CASE_EQUAL_FLOAT(measurement.samples[2], (float) 0.0));
}

//------------------------------------------------------------------------------
// Private virtual methods implemented from Accel
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Accel::Error AccelUnitTest::driverConfigure(const Config& config)
{
    myWasDriverConfigureCalled = true;

    return myDriverConfigureError;
}

//------------------------------------------------------------------------------
Accel::Error AccelUnitTest::driverGetMeasurement(Measurement& measurement)
{
    measurement = myMeasurement;

    return myDriverGetMeasurementError;
}
