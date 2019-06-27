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
/// @file UnitTestRunResultAsciiMessage.cpp
/// @author Ben Minerd
/// @date 5/4/16
/// @brief UnitTestRunResultAsciiMessage class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <UnitTestRunResultAsciiMessage.h>

#include <string.h>

using Plat4m::UnitTestRunResultAsciiMessage;
using Plat4m::ByteArray;
using Plat4m::Array;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const char* UnitTestRunResultAsciiMessage::myName = "UNIT_TEST_RUN_RESULT";

ByteArray UnitTestRunResultAsciiMessage::myParameterNameStrings[] =
{
    ByteArray("PASSED")
};

const Array<ByteArray> UnitTestRunResultAsciiMessage::myParameterNames(
                                            myParameterNameStrings,
                                            ARRAY_SIZE(myParameterNameStrings));

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
UnitTestRunResultAsciiMessage::UnitTestRunResultAsciiMessage(
                                            UnitTestRunResultMessage& message) :
    AsciiMessage(myName, &myParameterNames),
    myMessage(message)
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
UnitTestRunResultAsciiMessage::~UnitTestRunResultAsciiMessage()
{
}

//------------------------------------------------------------------------------
// Public virtual methods implemented from AsciiMessage
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void UnitTestRunResultAsciiMessage::stringParametersUpdated()
{
    const Array<ByteArray>* parameterValues = getParameterValues();
    const char* string = 0;

    // Passed
    string = (const char*) parameterValues->getItem(0).getItems();
    bool passed;

    if (strcmp(string, "TRUE") == 0)
    {
        passed = true;
    }
    else if (strcmp(string, "FALSE") == 0)
    {
        passed = false;
    }

    myMessage.setPassed(passed);
}

//------------------------------------------------------------------------------
void UnitTestRunResultAsciiMessage::messageParametersUpdated()
{
    Array<ByteArray>* parameterValues = getParameterValues();
    parameterValues->clear();

    char string[20];

    // Output
    parameterValues->getItem(0).clear();

    if (myMessage.getPassed())
    {
        strcpy(string, "TRUE");
        parameterValues->getItem(0).append((const char*) string);
    }
    else
    {
        strcpy(string, "FALSE");
        parameterValues->getItem(0).append((const char*) string);
    }

    parameterValues->setSize(1);
}
