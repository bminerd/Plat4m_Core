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
/// @file GpioPinUnitTest.h
/// @author Ben Minerd
/// @date 4/14/16
/// @brief GpioPinUnitTest class header file.
///

#ifndef GPIO_PIN_UNIT_TEST_H
#define GPIO_PIN_UNIT_TEST_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m.h>
#include <GpioPin.h>
#include <UnitTest.h>
#include <ErrorTemplate.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class GpioPinUnitTest : public GpioPin, public UnitTest
{
public:
    
    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    GpioPinUnitTest();

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~GpioPinUnitTest();

    //--------------------------------------------------------------------------
    // Public virtual methods implemented from GpioPin
    //--------------------------------------------------------------------------

    virtual void setLevelFast(const Level level);

    virtual Level getLevelFast();

    virtual Level readLevelFast();

    virtual void toggleLevelFast();

    //--------------------------------------------------------------------------
    // Public static methods
    //--------------------------------------------------------------------------

    static bool configureTest1();

    static bool configureTest2();

    static bool configureTest3();

    static bool setLevelTest1();

    static bool setLevelTest2();

    static bool setLevelTest3();

    static bool setLevelTest4();

    static bool setLevelTest5();

    static bool getLevelTest1();

    static bool getLevelTest2();

    static bool getLevelTest3();

    static bool getLevelTest4();

    static bool getLevelTest5();

    static bool getLevelTest6();

    static bool readLevelTest1();

    static bool readLevelTest2();

    static bool readLevelTest3();

    static bool readLevelTest4();

    static bool readLevelTest5();

    static bool readLevelTest6();

    static bool toggleLevelTest1();

    static bool toggleLevelTest2();

    static bool toggleLevelTest3();

    static bool toggleLevelTest4();

    static bool toggleLevelTest5();

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    static const UnitTest::TestCallbackFunction myTestCallbackFunctions[];

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    bool myWasDriverConfigureCalled;

    GpioPin::Error myDriverConfigureError;

    GpioPin::Error myDriverSetLevelError;

    GpioPin::Error myDriverGetLevelError;

    GpioPin::Error myDriverReadLevelError;

    GpioPin::Error myDriverToggleLevelError;

    Level myLevel;

    //--------------------------------------------------------------------------
    // Private virtual methods implemented from GpioPin
    //--------------------------------------------------------------------------
    
    virtual GpioPin::Error driverConfigure(const Config& config);

    virtual GpioPin::Error driverSetLevel(const Level level);

    virtual GpioPin::Error driverGetLevel(Level& level);

    virtual GpioPin::Error driverReadLevel(Level& level);

    virtual GpioPin::Error driverToggleLevel();
};

}; // namespace Plat4m

#endif // GPIO_PIN_UNIT_TEST_H
