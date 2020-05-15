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
// Copyright (c) 2020 Benjamin Minerd
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
/// @file main.cpp
/// @author Ben Minerd
/// @date 5/14/2020
/// @brief Main source file for Linux_GCC_Test_Build_App.
///

// Include all system-agnostic interfaces and drivers

#include <Plat4m_Core/Tests/Build_Tests/interfaceIncludes.h>

// Include all Linux interfaces and drivers

#include <Plat4m_Core/Linux/SystemLinux.h>
#include <Plat4m_Core/Linux/ProcessorLinux.h>
#include <Plat4m_Core/Linux/MutexLinux.h>
#include <Plat4m_Core/Linux/QueueDriverLinux.h>
#include <Plat4m_Core/Linux/SerialPortLinux.h>
#include <Plat4m_Core/Linux/ThreadLinux.h>
#include <Plat4m_Core/Linux/WaitConditionLinux.h>

//------------------------------------------------------------------------------
int main()
{
    // Test build, do nothing

    return 0;
}
