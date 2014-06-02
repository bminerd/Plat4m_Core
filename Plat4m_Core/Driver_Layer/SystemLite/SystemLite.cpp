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
 * @file SystemLite.cpp
 * @author Ben Minerd
 * @date 6/4/2013
 * @brief SystemLite class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <SystemLite.h>
#include <MutexLite.h>
#include <Micro.h>

/*------------------------------------------------------------------------------
 * Public constructors and destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
SystemLite::SystemLite() :
    System("SYSTEM_LITE"),
    myTimeMs(0),
    myTaskDepth(0)
{
}

/*------------------------------------------------------------------------------
 * Private implemented methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void SystemLite::driverAddTask(Task& task)
{
    TaskLite* taskLite = new TaskLite(task);

    myTaskLiteList.append(taskLite);
}

//------------------------------------------------------------------------------
void SystemLite::driverRun()
{
    FOREVER
    {
        checkTasks();
    }
}

//------------------------------------------------------------------------------
void SystemLite::driverTimeMsHandler()
{
    ++myTimeMs;
}

//------------------------------------------------------------------------------
uint32_t SystemLite::driverTimeGetMs()
{
    return myTimeMs;
}

//------------------------------------------------------------------------------
void SystemLite::driverTimeDelayMs(const uint32_t timeMs)
{
    const uint32_t delayTimeMs = myTimeMs + timeMs;

    while (myTimeMs < delayTimeMs)
    {
        if (isRunning())
        {
            checkTasks(timeMs);
        }
    }
}

//------------------------------------------------------------------------------
Mutex& SystemLite::driverGetMutex()
{
    return *(new MutexLite());
}

/*------------------------------------------------------------------------------
 * Private methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void SystemLite::checkTasks(const uint32_t timeWindowMs)
{
    List<TaskLite*>::Iterator iterator = myTaskLiteList.iterator();
    uint32_t timeWindowEndTimeMs = myTimeMs + timeWindowMs;

    while (iterator.hasCurrent())
    {
        TaskLite* taskLite = iterator.current();

        if (taskLite->task.isEnabled()                  &&
            !taskLite->isActive                         &&
            System::timeCheckMs(taskLite->callTimeMs)   &&
            ((timeWindowMs == 0) ||
             ((myTimeMs + taskLite->worstCaseMs) < timeWindowEndTimeMs)))
        {
            callTask(taskLite);
        }

        iterator.next();
    }

    Micro::setPowerMode(Micro::POWER_MODE_SLEEP);
}

//------------------------------------------------------------------------------
void SystemLite::callTask(TaskLite* taskLite)
{
    myTaskDepth++;
    taskLite->isActive = true;

    uint32_t startTimeMs = myTimeMs;

    // Call the task code
    taskLite->task.run();

    uint32_t dTimeMs = myTimeMs - startTimeMs;

    if ((taskLite->worstCaseMs == 0xFFFFFFFF) ||
        (dTimeMs > taskLite->worstCaseMs))
    {
        taskLite->worstCaseMs = dTimeMs;
    }

    taskLite->callTimeMs = startTimeMs + taskLite->task.getTimeDelayMs();

    taskLite->isActive = false;
    myTaskDepth--;
}
