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
// Copyright (c) 2017 Benjamin Minerd
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
/// @file ControllerServer.h
/// @author Ben Minerd
/// @date 2/2/2017
/// @brief ControllerServer class header file.
///

#ifndef PLAT4M_CONTROL_CONTROLLER_SERVER_H
#define PLAT4M_CONTROL_CONTROLLER_SERVER_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

// Plat4m includes
#include <Plat4m.h>
#include <BinaryMessageServer.h>
#include <ErrorTemplate.h>
#include <Controller.h>
#include <Thread.h>
#include <ControllerUpdateMessage.h>
#include <ControllerServerGetCountMessage.h>
#include <ControllerServerGetCountResponseMessage.h>
#include <ControllerServerSetConfigMessage.h>
#include <ControllerServerSetConfigResponseMessage.h>
#include <ComProtocolPlat4mBinary.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

namespace Control
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class ControllerServer : public BinaryMessageServer
{
public:

    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------

    enum ErrorCode
    {
        ERROR_CODE_NONE,
        ERROR_CODE_NOT_ENABLED
    };

    typedef ErrorTemplate<ErrorCode> Error;

    struct Config
    {
        uint32_t outputRateDivisionFactor;
    };

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    ControllerServer(ComProtocolPlat4mBinary& comProtocolPlat4mBinary,
                     BinaryMessageFrameHandler& binaryMessageFrameHandler);

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~ControllerServer();

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    bool addController(Controller& controller);

    Config getConfig() const;

    Error setConfig(const Config& config);

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    static const TimeMs myOutputThreadPeriodMs;

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    Config myConfig;

    WaitCondition& myWaitCondition;

    List<Controller*> myControllerList;

    Thread& myOutputThread;

    // TODO: This only works for one Controller currently

    uint32_t myUpdateCount;

    //--------------------------------------------------------------------------
    // Private virtual methods implemented from Module
    //--------------------------------------------------------------------------

    virtual Module::Error driverSetEnabled(const bool enabled);

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------

    void initialize();

    void controllerUpdateCallback();

    void outputThreadCallback();

    void serverSetConfigMessageCallback(
                            const ControllerServerSetConfigMessage& request,
                            ControllerServerSetConfigResponseMessage& response);

    void serverGetCountMessageCallback(
                             const ControllerServerGetCountMessage& request,
                             ControllerServerGetCountResponseMessage& response);
};

}; // namespace Control

}; // namespace Plat4m

#endif // PLAT4M_CONTROL_CONTROLLER_SERVER_H
