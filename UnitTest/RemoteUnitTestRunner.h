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
/// @file RemoteUnitTestRunner.h
/// @author Ben Minerd
/// @date 5/4/2016
/// @brief RemoteUnitTestRunner class header file.
///

#ifndef REMOTE_UNIT_TEST_RUNNER_H
#define REMOTE_UNIT_TEST_RUNNER_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

// Plat4m includes
#include <Plat4m.h>
#include <UnitTestRunner.h>
#include <List.h>
#include <ComLink.h>
#include <ComInterface.h>
#include <UnitTestRunMessage.h>
#include <UnitTestRunResultMessage.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

///
/// @brief
///
class RemoteUnitTestRunner : public UnitTestRunner
{
public:

protected:

    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------

    ///
    /// @brief Constructor for RemoteUnitTestRunner class.
    ///
    RemoteUnitTestRunner(const char* name,
                         const char* productName,
                         const char* version);

    //--------------------------------------------------------------------------
    // Protected virtual destructors
    //--------------------------------------------------------------------------

    ///
    /// @brief Virtual destructor for RemoteUnitTestRunner class.
    ///
    virtual ~RemoteUnitTestRunner();

    //--------------------------------------------------------------------------
    // Protected methods
    //--------------------------------------------------------------------------

    void runParentApplication(ComInterface& comInterface);

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    static const char* myName;

    static const char* myProductName;

    static const char* myVersion;

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    ComLink<64, 64> myComLink;

    ComProtocolPlat4mAscii myComProtocolPlat4mAscii;

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------

	void initializeSystem();

	void testThreadCallback();

	void unitTestRunMessageHandler(const UnitTestRunMessage& request,
	                               UnitTestRunResultMessage& response);
};

}; // namespace Plat4m

#endif // REMOTE_UNIT_TEST_RUNNER_H
