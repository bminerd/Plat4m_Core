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
 * @file System.h
 * @author Ben Minerd
 * @date 6/4/2013
 * @brief System class.
 */

#ifndef SYSTEM_H
#define SYSTEM_H

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <ErrorTemplate.h>
#include <Mutex.h>
#include <Thread.h>

namespace Plat4m
{

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

/**
 * @brief Handles all core and OS functionality including timing, task
 * scheduling, and mutex creation.
 */
class System
{
public:
    /*--------------------------------------------------------------------------
     * Public enumerations
     *------------------------------------------------------------------------*/

    /**
     * @brief Enumeration of timer errors.
     */
    enum ErrorCode
    {
        ERROR_CODE_NONE,
        ERROR_CODE_PARAMETER_INVALID,
        ERROR_CODE_MODE_INVALID,
        ERROR_CODE_NOT_ENABLED
    };

    /*--------------------------------------------------------------------------
     * Public typedefs
     *------------------------------------------------------------------------*/

    typedef ErrorTemplate<ErrorCode> Error;
    
    /*--------------------------------------------------------------------------
     * Public static methods
     *------------------------------------------------------------------------*/
    
    static const char* getName();

    static void addThread(Thread& thread);

    static void run();
    
    static bool isRunning();

    static void timeMsHandler();

    static uint32_t timeGetMs();

    static void timeDelayMs(const uint32_t timeMs);

    static bool timeCheckMs(const uint32_t timeMs);

    static Mutex& getMutex();
    
protected:
    
    /*--------------------------------------------------------------------------
     * Protected constructors
     *------------------------------------------------------------------------*/

    System(const char* name);

    /*--------------------------------------------------------------------------
     * Protected destructors
     *------------------------------------------------------------------------*/

    virtual ~System();

private:
    
    /*--------------------------------------------------------------------------
     * Private static data members
     *------------------------------------------------------------------------*/
    
    static System* myDriver;
    
    static bool myIsRunning;
    
    static const char* myName;
    
    /*--------------------------------------------------------------------------
     * Private virtual methods
     *------------------------------------------------------------------------*/

    virtual void driverAddThread(Thread& thread) = 0;

    virtual void driverRun() = 0;

    virtual void driverTimeMsHandler() = 0;

    virtual uint32_t driverTimeGetMs() = 0;

    virtual void driverTimeDelayMs(const uint32_t timeMs) = 0;

    virtual Mutex& driverGetMutex() = 0;
};

}; // namespace Plat4m

#endif // SYSTEM_H
