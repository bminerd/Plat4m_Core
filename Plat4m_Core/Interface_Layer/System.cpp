/*------------------------------------------------------------------------------
 *       _______    __                           ___
 *      ||  ___ \  || |             __          //  |
 *      || |  || | || |   _______  || |__      //   |    _____  ___
 *      || |__|| | || |  // ___  | ||  __|    // _  |   ||  _ \/ _ \
 *      ||  ____/  || | || |  || | || |      // /|| |   || |\\  /\\ \
 *      || |       || | || |__|| | || |     // /_|| |_  || | || | || |
 *      || |       || |  \\____  | || |__  //_____   _| || | || | || |
 *      ||_|       ||_|       ||_|  \\___|       ||_|   ||_| ||_| ||_|
 *
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Benjamin Minerd
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *----------------------------------------------------------------------------*/

/**
 * @file System.cpp
 * @author Ben Minerd
 * @date 6/4/2013
 * @brief System class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <System.h>
#include <List.h>

using Plat4m::System;

/*------------------------------------------------------------------------------
 * Static data members
 *----------------------------------------------------------------------------*/

System* System::myDriver    = nullptr;
bool System::myIsRunning    = false;
const char* System::myName  = nullptr;

/*------------------------------------------------------------------------------
 * Public static methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
const char* System::getName()
{
    return myName;
}

//------------------------------------------------------------------------------
void System::addThread(Thread& thread)
{
    myDriver->driverAddThread(thread);
}

//------------------------------------------------------------------------------
void System::run()
{
    myDriver->driverRun();
}

//------------------------------------------------------------------------------
bool System::isRunning()
{
    return myIsRunning;
}

//------------------------------------------------------------------------------
void System::timeMsHandler()
{
    myDriver->driverTimeMsHandler();
}

//------------------------------------------------------------------------------
uint32_t System::timeGetMs()
{
    return myDriver->driverTimeGetMs();
}

//------------------------------------------------------------------------------
void System::timeDelayMs(const uint32_t timeMs)
{
    myDriver->driverTimeDelayMs(timeMs);
}

//------------------------------------------------------------------------------
bool System::timeCheckMs(const uint32_t timeMs)
{
    return (timeMs <= timeGetMs());
}

//------------------------------------------------------------------------------
Mutex& System::getMutex()
{
    return myDriver->driverGetMutex();
}

/*------------------------------------------------------------------------------
* Protected constructors and destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
System::System(const char* name)
{
    if (isNullPointer(myDriver))
    {
        myName      = name;
        myDriver    = this;
    }
}
