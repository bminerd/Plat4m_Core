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
// Copyright (c) 2024 Benjamin Minerd
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
/// @file CanPcan.cpp
/// @author Ben Minerd
/// @date 7/25/2024
/// @brief CanPcan class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/CanPcan/CanPcan.h>
#include <Plat4m_Core/System.h>

using namespace Plat4m;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
CanPcan::CanPcan() :
    Can(),
    myPortHandle(0),
    myChannelHandle(0),
    myReceiveThread(System::createThread(
                          createCallback(this, &CanPcan::receiveThreadCallback),
                          0,
                          0,
                          false,
                          "CanPcan Receive Thread")),
    myIsInitialConfigurationSet(false)
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
CanPcan::~CanPcan()
{
    myReceiveThread.~Thread();
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
CanPcan::Error CanPcan::setCanPcanConfig(const CanPcanConfig& config)
{
    return (Error(ERROR_CODE_NONE));
}

//------------------------------------------------------------------------------
// Protected virtual methods overridden for Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error CanPcan::subclassSetEnabled(const bool enabled)
{
    return (Module::Error(Module::ERROR_CODE_NONE));
}

//------------------------------------------------------------------------------
// Protected virtual methods overridden for Can
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Can::Error CanPcan::subclassSetConfig(const Config& config)
{
    return (Can::Error(Can::ERROR_CODE_NONE));
}

//------------------------------------------------------------------------------
Can::Error CanPcan::subclassSendMessage(const Message& message)
{
    return (Can::Error(Can::ERROR_CODE_NONE));
}

//------------------------------------------------------------------------------
Can::Error CanPcan::subclassAddAcceptanceFilter(const IdType idType,
                                                 const std::uint32_t filter,
                                                 const std::uint32_t mask)
{
    return (Can::Error(Can::ERROR_CODE_NONE));
}

//------------------------------------------------------------------------------
Can::Error CanPcan::subclassClearReceivedMessages()
{
    return (Can::Error(Can::ERROR_CODE_NONE));
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void CanPcan::receiveThreadCallback()
{
}
