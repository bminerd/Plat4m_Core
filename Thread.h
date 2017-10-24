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
 * @file Thread.h
 * @author Ben Minerd
 * @date 6/14/2013
 * @brief Thread class.
 */

#ifndef THREAD_H
#define THREAD_H

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <Module.h>
#include <Callback.h>

namespace Plat4m
{

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

class Thread : public Module
{
public:
    
    /*--------------------------------------------------------------------------
     * Public typedefs
     *------------------------------------------------------------------------*/
    
    typedef Callback<> RunCallback;
    
    /*--------------------------------------------------------------------------
     * Public constructors and destructors
     *------------------------------------------------------------------------*/
    
    Thread(RunCallback& callback, const uint32_t timeDelayMs = 0);
    
    /*--------------------------------------------------------------------------
     * Public methods
     *------------------------------------------------------------------------*/

    void run();
    
    uint32_t getTimeDelayMs();
    
private:
    
    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/

    RunCallback& myRunCallback;
    
    const unsigned int myTimeDelayMs;
};

}; // namespace Plat4m

#endif // THREAD_H
