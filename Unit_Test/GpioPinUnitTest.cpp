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
/// @file GpioPinUnitTest.cpp
/// @author Ben Minerd
/// @date 4/14/16
/// @brief GpioPinUnitTest class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <GpioPinUnitTest.h>

using Plat4m::GpioPinUnitTest;
using Plat4m::GpioPin;
using Plat4m::UnitTest;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const UnitTest::TestCallbackFunction GpioPinUnitTest::myTestCallbackFunctions[] =
{
    &GpioPinUnitTest::configureTest1,
    &GpioPinUnitTest::configureTest2,
    &GpioPinUnitTest::configureTest3,

    &GpioPinUnitTest::setLevelTest1,
    &GpioPinUnitTest::setLevelTest2,
    &GpioPinUnitTest::setLevelTest3,
    &GpioPinUnitTest::setLevelTest4,
    &GpioPinUnitTest::setLevelTest5,

    &GpioPinUnitTest::getLevelTest1,
    &GpioPinUnitTest::getLevelTest2,
    &GpioPinUnitTest::getLevelTest3,
    &GpioPinUnitTest::getLevelTest4,
    &GpioPinUnitTest::getLevelTest5,
    &GpioPinUnitTest::getLevelTest6,

    &GpioPinUnitTest::readLevelTest1,
    &GpioPinUnitTest::readLevelTest2,
    &GpioPinUnitTest::readLevelTest3,
    &GpioPinUnitTest::readLevelTest4,
    &GpioPinUnitTest::readLevelTest5,
    &GpioPinUnitTest::readLevelTest6,

    &GpioPinUnitTest::toggleLevelTest1,
    &GpioPinUnitTest::toggleLevelTest2,
    &GpioPinUnitTest::toggleLevelTest3,
    &GpioPinUnitTest::toggleLevelTest4,
    &GpioPinUnitTest::toggleLevelTest5
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GpioPinUnitTest::GpioPinUnitTest() :
    GpioPin(),
    UnitTest("GpioPinUnitTest",
             myTestCallbackFunctions,
             ARRAY_SIZE(myTestCallbackFunctions)),
    myWasDriverConfigureCalled(false),
    myDriverConfigureError(GpioPin::ERROR_CODE_NONE),
    myDriverSetLevelError(GpioPin::ERROR_CODE_NONE),
    myDriverGetLevelError(GpioPin::ERROR_CODE_NONE),
    myDriverReadLevelError(GpioPin::ERROR_CODE_NONE),
    myDriverToggleLevelError(GpioPin::ERROR_CODE_NONE),
    myLevel(LEVEL_LOW)
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GpioPinUnitTest::~GpioPinUnitTest()
{
}

//------------------------------------------------------------------------------
// Public virtual methods implemented from GpioPin
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void GpioPinUnitTest::setLevelFast(const Level level)
{
    myLevel = level;
}

//------------------------------------------------------------------------------
GpioPin::Level GpioPinUnitTest::getLevelFast()
{
    return myLevel;
}

//------------------------------------------------------------------------------
GpioPin::Level GpioPinUnitTest::readLevelFast()
{
    return myLevel;
}

//------------------------------------------------------------------------------
void GpioPinUnitTest::toggleLevelFast()
{
    if (myLevel == LEVEL_HIGH)
    {
        myLevel = LEVEL_LOW;
    }
    else
    {
        myLevel = LEVEL_HIGH;
    }
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool GpioPinUnitTest::configureTest1()
{
    //
    // Procedure: call configure()
    //
    // Test: error code should be ERROR_CODE_NONE
    //

    GpioPinUnitTest gpioPin;

    GpioPin::Config config;
    GpioPin::Error error = gpioPin.configure(config);

    return UNIT_TEST_REPORT(
               UNIT_TEST_CASE_EQUAL(error.getCode(), GpioPin::ERROR_CODE_NONE));
}

//------------------------------------------------------------------------------
bool GpioPinUnitTest::configureTest2()
{
    //
    // Procedure: call setLevel() with driver error
    //
    // Test: gpioPin.myWasDriverConfigureCalled should be true
    //

    GpioPinUnitTest gpioPin;

    GpioPin::Config config;
    gpioPin.configure(config);

    return UNIT_TEST_REPORT(
                UNIT_TEST_CASE_EQUAL(gpioPin.myWasDriverConfigureCalled, true));
}

//------------------------------------------------------------------------------
bool GpioPinUnitTest::configureTest3()
{
    //
    // Procedure: call setLevel() with driver error
    //
    // Test: gpioPin.myLevel should be LEVEL_LOW (GpioPin::driverSetLevel()
    // should not have been called)
    //

    GpioPinUnitTest gpioPin;
    gpioPin.myDriverConfigureError =
                              GpioPin::Error(GpioPin::ERROR_CODE_COMMUNICATION);

    GpioPin::Config config;
    GpioPin::Error error = gpioPin.configure(config);

    return UNIT_TEST_REPORT(
                       UNIT_TEST_CASE_EQUAL(error.getCode(),
                                            GpioPin::ERROR_CODE_COMMUNICATION));
}

//------------------------------------------------------------------------------
bool GpioPinUnitTest::setLevelTest1()
{
    //
    // Procedure: call setLevel() without first calling enable(true)
    //
    // Test: error code should be ERROR_CODE_NOT_ENABLED
    //

    GpioPinUnitTest gpioPin;

    GpioPin::Error error = gpioPin.setLevel(LEVEL_HIGH);

    return UNIT_TEST_REPORT(
        UNIT_TEST_CASE_EQUAL(error.getCode(), GpioPin::ERROR_CODE_NOT_ENABLED));
}

//------------------------------------------------------------------------------
bool GpioPinUnitTest::setLevelTest2()
{
    //
    // Procedure: call setLevel() without first calling enable(true)
    //
    // Test: gpioPin.myLevel should be LEVEL_LOW (GpioPin::driverSetLevel()
    // should not have been called)
    //

    GpioPinUnitTest gpioPin;
    gpioPin.myLevel = LEVEL_LOW;

    gpioPin.setLevel(LEVEL_HIGH);

    return UNIT_TEST_REPORT(
                     UNIT_TEST_CASE_EQUAL(gpioPin.myLevel, GpioPin::LEVEL_LOW));
}

//------------------------------------------------------------------------------
bool GpioPinUnitTest::setLevelTest3()
{
    //
    // Procedure: call setLevel() after first calling enable(true)
    //
    // Test: error code should be ERROR_CODE_NONE
    //

    GpioPinUnitTest gpioPin;

    gpioPin.enable(true);
    GpioPin::Error error = gpioPin.setLevel(LEVEL_HIGH);

    return UNIT_TEST_REPORT(
               UNIT_TEST_CASE_EQUAL(error.getCode(), GpioPin::ERROR_CODE_NONE));
}

//------------------------------------------------------------------------------
bool GpioPinUnitTest::setLevelTest4()
{
    //
    // Procedure: call setLevel() after first calling enable(true)
    //
    // Test: gpioPin.myLevel should be LEVEL_HIGH (driverSetLevel() should have
    // been called)
    //

    GpioPinUnitTest gpioPin;
    gpioPin.myLevel = LEVEL_LOW;

    gpioPin.enable(true);
    gpioPin.setLevel(LEVEL_HIGH);

    return UNIT_TEST_REPORT(
                    UNIT_TEST_CASE_EQUAL(gpioPin.myLevel, GpioPin::LEVEL_HIGH));
}

//------------------------------------------------------------------------------
bool GpioPinUnitTest::setLevelTest5()
{
    //
    // Procedure: call setLevel() with driver error
    //
    // Test: error.getCode() should be GpioPin::ERROR_CODE_COMMUNICATION
    //

    GpioPinUnitTest gpioPin;
    gpioPin.myDriverSetLevelError =
                              GpioPin::Error(GpioPin::ERROR_CODE_COMMUNICATION);

    gpioPin.enable(true);
    GpioPin::Error error = gpioPin.setLevel(LEVEL_HIGH);

    return UNIT_TEST_REPORT(
                       UNIT_TEST_CASE_EQUAL(error.getCode(),
                                            GpioPin::ERROR_CODE_COMMUNICATION));
}

//------------------------------------------------------------------------------
bool GpioPinUnitTest::getLevelTest1()
{
    //
    // Procedure: call getLevel() without first calling enable(true)
    //
    // Test: error code should be ERROR_CODE_NOT_ENABLED
    //

    GpioPinUnitTest gpioPin;

    GpioPin::Level level;
    GpioPin::Error error = gpioPin.getLevel(level);

    return UNIT_TEST_REPORT(
        UNIT_TEST_CASE_EQUAL(error.getCode(), GpioPin::ERROR_CODE_NOT_ENABLED));
}

//------------------------------------------------------------------------------
bool GpioPinUnitTest::getLevelTest2()
{
    //
    // Procedure: call getLevel() without first calling enable(true)
    //
    // Test: level should be LEVEL_HIGH (driverGetLevel() should not have been
    // called)
    //

    GpioPinUnitTest gpioPin;
    gpioPin.myLevel = LEVEL_LOW;

    GpioPin::Level level = LEVEL_HIGH;
    gpioPin.getLevel(level);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(level, GpioPin::LEVEL_HIGH));
}

//------------------------------------------------------------------------------
bool GpioPinUnitTest::getLevelTest3()
{
    //
    // Procedure: call getLevel() after first calling enable(true)
    //
    // Test: error code should be ERROR_CODE_NONE
    //

    GpioPinUnitTest gpioPin;

    gpioPin.enable(true);

    GpioPin::Level level;
    GpioPin::Error error = gpioPin.getLevel(level);

    return UNIT_TEST_REPORT(
               UNIT_TEST_CASE_EQUAL(error.getCode(), GpioPin::ERROR_CODE_NONE));
}

//------------------------------------------------------------------------------
bool GpioPinUnitTest::getLevelTest4()
{
    //
    // Procedure: call getLevel() after first calling enable(true)
    //
    // Test: level should be LEVEL_LOW (driverGetLevel() should have been
    // called)
    //

    GpioPinUnitTest gpioPin;
    gpioPin.myLevel = LEVEL_LOW;

    gpioPin.enable(true);

    GpioPin::Level level = LEVEL_HIGH;
    gpioPin.getLevel(level);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(level, GpioPin::LEVEL_LOW));
}

//------------------------------------------------------------------------------
bool GpioPinUnitTest::getLevelTest5()
{
    //
    // Procedure: call getLevel() with driver error
    //
    // Test: error.getCode() should be GpioPin::ERROR_CODE_COMMUNICATION
    //

    GpioPinUnitTest gpioPin;
    gpioPin.myDriverGetLevelError =
                              GpioPin::Error(GpioPin::ERROR_CODE_COMMUNICATION);

    gpioPin.enable(true);

    GpioPin::Level level;
    GpioPin::Error error = gpioPin.getLevel(level);

    return UNIT_TEST_REPORT(
                       UNIT_TEST_CASE_EQUAL(error.getCode(),
                                            GpioPin::ERROR_CODE_COMMUNICATION));
}

//------------------------------------------------------------------------------
bool GpioPinUnitTest::getLevelTest6()
{
    //
    // Procedure: call getLevel() with driver error
    //
    // Test: error.getCode() should be GpioPin::ERROR_CODE_COMMUNICATION
    //

    GpioPinUnitTest gpioPin;
    gpioPin.myLevel = LEVEL_LOW;
    gpioPin.myDriverGetLevelError =
                              GpioPin::Error(GpioPin::ERROR_CODE_COMMUNICATION);

    gpioPin.enable(true);

    GpioPin::Level level = LEVEL_HIGH;
    gpioPin.getLevel(level);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(level, LEVEL_HIGH));
}

//------------------------------------------------------------------------------
bool GpioPinUnitTest::readLevelTest1()
{
    //
    // Procedure: call readLevel() without first calling enable(true)
    //
    // Test: error code should be ERROR_CODE_NOT_ENABLED
    //

    GpioPinUnitTest gpioPin;

    GpioPin::Level level;
    GpioPin::Error error = gpioPin.readLevel(level);

    return UNIT_TEST_REPORT(
        UNIT_TEST_CASE_EQUAL(error.getCode(), GpioPin::ERROR_CODE_NOT_ENABLED));
}

//------------------------------------------------------------------------------
bool GpioPinUnitTest::readLevelTest2()
{
    //
    // Procedure: call readLevel() without first calling enable(true)
    //
    // Test: level should be LEVEL_HIGH (driverReadLevel() should not have been
    // called)
    //

    GpioPinUnitTest gpioPin;
    gpioPin.myLevel = LEVEL_LOW;

    GpioPin::Level level = LEVEL_HIGH;
    gpioPin.readLevel(level);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(level, GpioPin::LEVEL_HIGH));
}

//------------------------------------------------------------------------------
bool GpioPinUnitTest::readLevelTest3()
{
    //
    // Procedure: call readLevel() after first calling enable(true)
    //
    // Test: Error code should be ERROR_CODE_NONE
    //

    GpioPinUnitTest gpioPin;

    gpioPin.enable(true);

    GpioPin::Level level;
    GpioPin::Error error = gpioPin.readLevel(level);

    return UNIT_TEST_REPORT(
               UNIT_TEST_CASE_EQUAL(error.getCode(), GpioPin::ERROR_CODE_NONE));
}

//------------------------------------------------------------------------------
bool GpioPinUnitTest::readLevelTest4()
{
    //
    // Procedure: call readLevel() after first calling enable(true)
    //
    // Test: level should be LEVEL_LOW (GpioPin::driverReadLevel() should have
    // been called)
    //

    GpioPinUnitTest gpioPin;
    gpioPin.myLevel = LEVEL_LOW;

    gpioPin.enable(true);

    GpioPin::Level level = LEVEL_HIGH;
    gpioPin.readLevel(level);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(level, GpioPin::LEVEL_LOW));
}

//------------------------------------------------------------------------------
bool GpioPinUnitTest::readLevelTest5()
{
    //
    // Procedure: call readLevel() with driver error
    //
    // Test: error.getCode() should be GpioPin::ERROR_CODE_COMMUNICATION
    //

    GpioPinUnitTest gpioPin;
    gpioPin.myDriverReadLevelError =
                              GpioPin::Error(GpioPin::ERROR_CODE_COMMUNICATION);

    gpioPin.enable(true);

    GpioPin::Level level;
    GpioPin::Error error = gpioPin.readLevel(level);

    return UNIT_TEST_REPORT(
                       UNIT_TEST_CASE_EQUAL(error.getCode(),
                                            GpioPin::ERROR_CODE_COMMUNICATION));
}

//------------------------------------------------------------------------------
bool GpioPinUnitTest::readLevelTest6()
{
    //
    // Procedure: call readLevel() with driver error
    //
    // Test: error.getCode() should be GpioPin::ERROR_CODE_COMMUNICATION
    //

    GpioPinUnitTest gpioPin;
    gpioPin.myLevel = LEVEL_LOW;
    gpioPin.myDriverReadLevelError =
                              GpioPin::Error(GpioPin::ERROR_CODE_COMMUNICATION);

    gpioPin.enable(true);

    GpioPin::Level level = LEVEL_HIGH;
    gpioPin.readLevel(level);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(level, GpioPin::LEVEL_HIGH));
}

//------------------------------------------------------------------------------
bool GpioPinUnitTest::toggleLevelTest1()
{
    //
    // Procedure: call toggleLevel() without first calling enable(true)
    //
    // Test: error code should be ERROR_CODE_NOT_ENABLED
    //

    GpioPinUnitTest gpioPin;

    GpioPin::Error error = gpioPin.toggleLevel();

    return UNIT_TEST_REPORT(
        UNIT_TEST_CASE_EQUAL(error.getCode(), GpioPin::ERROR_CODE_NOT_ENABLED));
}

//------------------------------------------------------------------------------
bool GpioPinUnitTest::toggleLevelTest2()
{
    //
    // Procedure: call toggleLevel() without first calling enable(true)
    //
    // Test: gpioPin.myLevel should be LEVEL_LOW (driverToggleLevel() should not
    // have been called)
    //

    GpioPinUnitTest gpioPin;
    gpioPin.myLevel = LEVEL_LOW;

    gpioPin.toggleLevel();

    return UNIT_TEST_REPORT(
                     UNIT_TEST_CASE_EQUAL(gpioPin.myLevel, GpioPin::LEVEL_LOW));
}

//------------------------------------------------------------------------------
bool GpioPinUnitTest::toggleLevelTest3()
{
    //
    // Procedure: call toggleLevel() after first calling enable(true)
    //
    // Test: Error code should be ERROR_CODE_NONE
    //

    GpioPinUnitTest gpioPin;

    gpioPin.enable(true);

    GpioPin::Error error = gpioPin.toggleLevel();

    return UNIT_TEST_REPORT(
               UNIT_TEST_CASE_EQUAL(error.getCode(), GpioPin::ERROR_CODE_NONE));
}

//------------------------------------------------------------------------------
bool GpioPinUnitTest::toggleLevelTest4()
{
    //
    // Procedure: call toggleLevel() after first calling enable(true)
    //
    // Test: gpioPin.myLevel should be LEVEL_HIGH (GpioPin::driverToggleLevel()
    // should have been called)
    //

    GpioPinUnitTest gpioPin;
    gpioPin.myLevel = LEVEL_LOW;

    gpioPin.enable(true);

    gpioPin.toggleLevel();

    return UNIT_TEST_REPORT(
                    UNIT_TEST_CASE_EQUAL(gpioPin.myLevel, GpioPin::LEVEL_HIGH));
}

//------------------------------------------------------------------------------
bool GpioPinUnitTest::toggleLevelTest5()
{
    //
    // Procedure: call toggleLevel() with driver error
    //
    // Test: error.getCode() should be GpioPin::ERROR_CODE_COMMUNICATION
    //

    GpioPinUnitTest gpioPin;
    gpioPin.myDriverToggleLevelError =
                              GpioPin::Error(GpioPin::ERROR_CODE_COMMUNICATION);

    gpioPin.enable(true);

    GpioPin::Error error = gpioPin.toggleLevel();

    return UNIT_TEST_REPORT(
                       UNIT_TEST_CASE_EQUAL(error.getCode(),
                                            GpioPin::ERROR_CODE_COMMUNICATION));
}

//------------------------------------------------------------------------------
// Private virtual methods implemented from GpioPin
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GpioPin::Error GpioPinUnitTest::driverConfigure(const Config& config)
{
    myWasDriverConfigureCalled = true;

    return myDriverConfigureError;
}

//------------------------------------------------------------------------------
GpioPin::Error GpioPinUnitTest::driverSetLevel(const Level level)
{
    myLevel = level;

    return myDriverSetLevelError;
}

//------------------------------------------------------------------------------
GpioPin::Error GpioPinUnitTest::driverGetLevel(Level& level)
{
    level = myLevel;

    return myDriverGetLevelError;
}

//------------------------------------------------------------------------------
GpioPin::Error GpioPinUnitTest::driverReadLevel(Level& level)
{
    level = myLevel;

    return myDriverReadLevelError;
}

//------------------------------------------------------------------------------
GpioPin::Error GpioPinUnitTest::driverToggleLevel()
{
    if (myLevel == LEVEL_HIGH)
    {
        myLevel = LEVEL_LOW;
    }
    else
    {
        myLevel = LEVEL_HIGH;
    }

    return myDriverToggleLevelError;
}
