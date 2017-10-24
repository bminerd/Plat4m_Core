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
 * @file Application.cpp
 * @author Ben Minerd
 * @date 6/4/2013
 * @brief Application namespace.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Application.h>

/*------------------------------------------------------------------------------
 * Static data members
 *----------------------------------------------------------------------------*/
     
Application* Application::myDriver      = NULL_POINTER;
const char* Application::myName         = NULL_POINTER;
const char* Application::myProductName  = NULL_POINTER;
const char* Application::myVersion      = NULL_POINTER;

/*------------------------------------------------------------------------------
 * Public static methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
const char* Application::getName()
{
    return myName;
}

//------------------------------------------------------------------------------
const char* Application::getProductName()
{
    return myProductName;
}

//------------------------------------------------------------------------------
const char* Application::getVersion()
{
    return myVersion;
}

//------------------------------------------------------------------------------
float Application::getInputVoltage()
{
    return myDriver->driverGetInputVoltage();
}

//------------------------------------------------------------------------------
void Application::run()
{
    return myDriver->driverRun();
}

/*------------------------------------------------------------------------------
 * Protected constructors and destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Application::Application(const char* name,
                         const char* productName,
                         const char* version)
{
    if (IS_NULL_POINTER(myDriver))
    {
        myName          = name;
        myProductName   = productName;
        myVersion       = version;
        myDriver        = this;
    }
}