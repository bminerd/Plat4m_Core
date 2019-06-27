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
/// @file UnitTestRunMessage.cpp
/// @author Ben Minerd
/// @date 4/25/16
/// @brief UnitTestRunMessage class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <UnitTestRunMessage.h>

using Plat4m::UnitTestRunMessage;
using Plat4m::Array;
using Plat4m::ByteArray;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
UnitTestRunMessage::UnitTestRunMessage() :
    Message(),
    myModuleIndex(0),
    myTestIndex(0),
    myOutput(false)
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
UnitTestRunMessage::~UnitTestRunMessage()
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
uint32_t UnitTestRunMessage::getModuleIndex() const
{
    return myModuleIndex;
}

//------------------------------------------------------------------------------
void UnitTestRunMessage::setModuleIndex(const uint32_t moduleIndex)
{
    myModuleIndex = moduleIndex;
}

//------------------------------------------------------------------------------
uint32_t UnitTestRunMessage::getTestIndex() const
{
    return myTestIndex;
}

//------------------------------------------------------------------------------
void UnitTestRunMessage::setTestIndex(const uint32_t testIndex)
{
    myTestIndex = testIndex;
}

//------------------------------------------------------------------------------
bool UnitTestRunMessage::getOutput() const
{
    return myOutput;
}

//------------------------------------------------------------------------------
void UnitTestRunMessage::setOutput(const bool output)
{
    myOutput = output;
}
