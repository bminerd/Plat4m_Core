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
/// @file UnitTestRunAsciiMessage.cpp
/// @author Ben Minerd
/// @date 4/25/16
/// @brief UnitTestRunAsciiMessage class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <UnitTestRunAsciiMessage.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

using Plat4m::UnitTestRunAsciiMessage;
using Plat4m::ByteArray;
using Plat4m::Array;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const char* UnitTestRunAsciiMessage::myName = "UNIT_TEST_RUN";

ByteArray UnitTestRunAsciiMessage::myParameterNameStrings[3] =
{
    ByteArray("MODULE_INDEX"),
    ByteArray("TEST_INDEX"),
    ByteArray("OUTPUT")
};

const Array<ByteArray> UnitTestRunAsciiMessage::myParameterNames(
                                            myParameterNameStrings,
                                            ARRAY_SIZE(myParameterNameStrings));

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
UnitTestRunAsciiMessage::UnitTestRunAsciiMessage(UnitTestRunMessage& message) :
    AsciiMessage(myName, &myParameterNames),
    myMessage(message)
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
UnitTestRunAsciiMessage::~UnitTestRunAsciiMessage()
{
}

//------------------------------------------------------------------------------
// Public virtual methods implemented from AsciiMessage
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void UnitTestRunAsciiMessage::stringParametersUpdated()
{
    const Array<ByteArray>* parameterValues = getParameterValues();
    const char* string = 0;

    // Module index
    string = (const char*) (parameterValues->getItem(0).getItems());
    uint32_t moduleIndex = atoi(string);
    myMessage.setModuleIndex(moduleIndex);

    // Test index
    string = (const char*) (parameterValues->getItem(1).getItems());
    uint32_t testIndex = atoi(string);
    myMessage.setTestIndex(testIndex);

    // Output
    string = (const char*) (parameterValues->getItem(2).getItems());
    bool output;

    if (strcmp(string, "TRUE") == 0)
    {
        output = true;
    }
    else if (strcmp(string, "FALSE") == 0)
    {
        output = false;
    }

    myMessage.setOutput(output);
}

//------------------------------------------------------------------------------
void UnitTestRunAsciiMessage::messageParametersUpdated()
{
    const Array<ByteArray>* parameterValues = getParameterValues();

    char string[20];

    // Module index
    sprintf(string, "%u", myMessage.getModuleIndex());
    parameterValues->getItem(0).clear();
    parameterValues->getItem(0).append(string);

    // Test index
    sprintf(string, "%u", myMessage.getModuleIndex());
    parameterValues->getItem(1).clear();
    parameterValues->getItem(1).append(string);


    // Output
    parameterValues->getItem(2).clear();

    if (myMessage.getOutput())
    {
        strcpy(string, "TRUE");
        parameterValues->getItem(1).append(string);
    }
    else
    {
        strcpy(string, "FALSE");
        parameterValues->getItem(1).append(string);
    }
}
