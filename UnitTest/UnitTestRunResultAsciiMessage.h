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
/// @file UnitTestRunResultAsciiMessage.h
/// @author Ben Minerd
/// @date 5/4/16
/// @brief UnitTestRunResultAsciiMessage class header file.
///

#ifndef UNIT_TEST_ASCII_RESPONSE_MESSAGE_H
#define UNIT_TEST_ASCII_RESPONSE_MESSAGE_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m.h>
#include <AsciiMessage.h>
#include <UnitTestRunResultMessage.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class UnitTestRunResultAsciiMessage : public AsciiMessage
{
public:

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------
    
    UnitTestRunResultAsciiMessage(UnitTestRunResultMessage& message);

    //--------------------------------------------------------------------------
    // Public destructors
    //--------------------------------------------------------------------------

    virtual ~UnitTestRunResultAsciiMessage();

    //--------------------------------------------------------------------------
    // Public virtual methods implemented from AsciiMessage
    //--------------------------------------------------------------------------

    virtual void stringParametersUpdated();

    virtual void messageParametersUpdated();

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    static const char* myName;
    static ByteArray myParameterNameStrings[1];
    static const Array<ByteArray> myParameterNames;
    
    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    UnitTestRunResultMessage& myMessage;
};

}; // namespace Plat4m

#endif // UNIT_TEST_ASCII_MESSAGE_RESPONSE_H