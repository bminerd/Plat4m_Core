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
 * @file ComProtocol.h
 * @author Ben Minerd
 * @date 4/22/13
 * @brief ComProtocol class.
 */

#ifndef COM_PROTOCOL_H
#define COM_PROTOCOL_H

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <Module.h>
#include <ErrorTemplate.h>
#include <ByteArray.h>

namespace Plat4m
{

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

class ComProtocol : public Module
{
public:
    
    /*--------------------------------------------------------------------------
     * Public enumerations
     *------------------------------------------------------------------------*/

    /**
     * @brief Enumeration of communication errors.
     */
    enum ErrorCode
    {
        ERROR_CODE_NONE,
        ERROR_CODE_PARAMETER_INVALID
    };

    enum ParseStatus
    {
        PARSE_STATUS_NOT_A_MESSAGE,
        PARSE_STATUS_MID_MESSAGE,
        PARSE_STATUS_INVALID_MESSAGE,
        PARSE_STATUS_UNSUPPORTED_MESSAGE,
        PARSE_STATUS_FOUND_MESSAGE
    };

    /*--------------------------------------------------------------------------
	 * Public typedefs
	 *------------------------------------------------------------------------*/

    typedef ErrorTemplate<ErrorCode> Error;

    /*--------------------------------------------------------------------------
     * Public methods
     *------------------------------------------------------------------------*/
    
    uint32_t getParseTimeoutMs();
    
    ParseStatus parseData(const ByteArray& rxByteArray, ByteArray& txByteArray);
    
protected:
    
    /*--------------------------------------------------------------------------
	 * Protected constructors
	 *------------------------------------------------------------------------*/

	ComProtocol(const uint32_t parseTimeoutMs);

    /*--------------------------------------------------------------------------
     * Protected destructors
     *------------------------------------------------------------------------*/

    ~ComProtocol();
    
private:
    
    /*--------------------------------------------------------------------------
     * Private members
     *------------------------------------------------------------------------*/
    
    const uint32_t myParseTimeoutMs;

    /*--------------------------------------------------------------------------
     * Private methods implemented from Module
     *------------------------------------------------------------------------*/

    Module::Error driverEnable(const bool enable) override;

    /*--------------------------------------------------------------------------
	 * Private virtual methods
	 *------------------------------------------------------------------------*/

    virtual ParseStatus driverParseData(const ByteArray& rxByteArray,
    									ByteArray& txByteArray) = 0;
};

}; // namespace Plat4m

#endif // COM_PROTOCOL_H
