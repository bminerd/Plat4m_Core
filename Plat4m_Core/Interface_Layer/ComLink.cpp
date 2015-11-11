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
 * @file ComLink.cpp
 * @author Ben Minerd
 * @date 4/22/13
 * @brief ComLink class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <ComLink.h>
#include <ComInterface.h>
#include <System.h>

using Plat4m::ComLink;

/*------------------------------------------------------------------------------
 * Public constructors and destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
ComLink::ComLink(ComInterface& comInterface) :
    Module(),
    myComInterface(comInterface),
    myCurrentComProtocol(NULL_POINTER),
    myCurrentComProtocolTimeoutTimeMs(0),
    myThread(Plat4m::createCallback(this, &ComLink::taskCallback))
{
}

/*------------------------------------------------------------------------------
 * Public methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
ComLink::Error ComLink::addComProtocol(ComProtocol* comProtocol)
{
    myComProtocolList.append(comProtocol);
    
    return Error(ERROR_CODE_NONE);
}

/*------------------------------------------------------------------------------
 * Private methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void ComLink::taskCallback()
{
    if (myComInterface.rxBytesAvailable() == 0)
    {
        return;
    }
    
    myComInterface.getRxBytes(myRxByteArray);
    
    if (IS_VALID_POINTER(myCurrentComProtocol))
    {
        tryProtocol(myCurrentComProtocol);
    }
    else
    {
        
        List<ComProtocol*>::Iterator iterator = myComProtocolList.iterator();
        
        while (iterator.hasCurrent())
        {
            ComProtocol* comProtocol = iterator.current();
            tryProtocol(comProtocol);
            
            iterator.next();
        }
        
    }
    
    if (isNullPointer(myCurrentComProtocol))
    {
        myRxByteArray.clear();
    }
}

//------------------------------------------------------------------------------
void ComLink::tryProtocol(ComProtocol* comProtocol)
{
    switch (comProtocol->parseData(myRxByteArray, myTxByteArray))
    {
        case ComProtocol::PARSE_STATUS_MID_MESSAGE:
        {
            if (isNullPointer(myCurrentComProtocol))
            {
                myCurrentComProtocol = comProtocol;
                myCurrentComProtocolTimeoutTimeMs =
                                      System::timeGetMs() +
                                      myCurrentComProtocol->getParseTimeoutMs();
            }
            else
            {
                // Timeout
                if (System::timeCheckMs(myCurrentComProtocolTimeoutTimeMs))
                {
                    myCurrentComProtocol = nullptr;
                }
            }
            
            break;
        }
        case ComProtocol::PARSE_STATUS_FOUND_MESSAGE:
        {
            myComInterface.tx(myTxByteArray);
            
            myCurrentComProtocol = nullptr;
            myTxByteArray.clear();
            
            break;
        }
        default:
        {
            break;
        }
    }
}
