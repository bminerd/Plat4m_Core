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
 * @file ComLink.h
 * @author Ben Minerd
 * @date 4/22/13
 * @brief ComLink class.
 */

#ifndef _COM_LINK_H_
#define _COM_LINK_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <ComProtocol.h>
#include <List.h>
#include <ByteArrayN.h>
#include <ComInterface.h>
#include <Task.h>

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

class ComLink
{
public:

    /*--------------------------------------------------------------------------
     * Public enumerations
     *------------------------------------------------------------------------*/

    /**
     * @brief Enumeration of communication errors.
     */
    enum Error
    {
        ERROR_NONE,
        ERROR_PARAMETER_INVALID,
        ERROR_NOT_ENABLED
    };

    /*--------------------------------------------------------------------------
     * Public constructors and destructors
     *------------------------------------------------------------------------*/

    ComLink(ComInterface& comInterface);

    /*--------------------------------------------------------------------------
     * Public methods
     *------------------------------------------------------------------------*/

    Error enable(const bool enable);

    Error addComProtocol(ComProtocol* comProtocol);

private:

    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/

    bool myIsEnabled;

    ComInterface& myComInterface;

    List<ComProtocol*> myComProtocolList;

    ComProtocol* myCurrentComProtocol;

    uint32_t myCurrentComProtocolTimeoutTimeMs;

    ByteArrayN<256> myRxByteArray;

    ByteArrayN<256> myTxByteArray;

    Task myTask;

    /*--------------------------------------------------------------------------
     * Private methods
     *------------------------------------------------------------------------*/

    void taskCallback();

    void tryProtocol(ComProtocol* comProtocol);
};

#endif // _COM_LINK_H_