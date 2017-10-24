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
 * Copyright (c) 2013 Benjamin Minerd
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
 * @file SystemLite.h
 * @author Ben Minerd
 * @date 6/4/2013
 * @brief SystemLite class.
 */

#ifndef _SYSTEM_LITE_H_
#define _SYSTEM_LITE_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <System.h>
#include <List.h>

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

class SystemLite : public System
{
public:

    /*--------------------------------------------------------------------------
     * Public constructors and destructors
     *------------------------------------------------------------------------*/

    SystemLite();

private:

    /*--------------------------------------------------------------------------
     * Private structures
     *------------------------------------------------------------------------*/

    struct TaskLite
    {
        bool isActive;
        unsigned int callTimeMs;
        unsigned int worstCaseMs;
        Task& task;

        TaskLite(Task& newTask) :
            isActive(false),
            callTimeMs(0),
            worstCaseMs(0xFFFFFFFF),
            task(newTask)
        {
        }
    };

    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/

    volatile uint32_t myTimeMs;

    List<TaskLite*> myTaskLiteList;

    int myTaskDepth;

    /*--------------------------------------------------------------------------
     * Private implemented methods
     *------------------------------------------------------------------------*/

    void driverAddTask(Task& task);

    void driverRun();

    void driverTimeMsHandler();

    uint32_t driverTimeGetMs();

    void driverTimeDelayMs(const uint32_t timeMs);

    Mutex& driverGetMutex();

    /*--------------------------------------------------------------------------
     * Private methods
     *------------------------------------------------------------------------*/

    void checkTasks(const uint32_t timePeriodMs = 0);

    void callTask(TaskLite* taskLite);
};

#endif // _SYSTEM_LITE_H_
