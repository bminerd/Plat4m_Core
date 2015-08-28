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
 * @file ComProtocolNmea.h
 * @author Ben Minerd
 * @date 5/28/15
 * @brief ComProtocolNmea class.
 */

#ifndef _COM_PROTOCOL_NMEA_H_
#define _COM_PROTOCOL_NMEA_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <ComProtocol.h>
#include <ErrorTemplate.h>
#include <Callback.h>
#include <ArrayN.h>
#include <ByteArray.h>

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

class ComProtocolNmea : public ComProtocol
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
        ERROR_CODE_NONE
    };

    enum Message
	{
    	MESSAGE_DTM = 0,
    	MESSAGE_GBS,
    	MESSAGE_GGA,
    	MESSAGE_GLL,
    	MESSAGE_GLQ,
    	MESSAGE_GNQ,
    	MESSAGE_GNS,
    	MESSAGE_GPQ,
    	MESSAGE_GRS,
    	MESSAGE_GSA,
    	MESSAGE_GST,
    	MESSAGE_GSV,
    	MESSAGE_RMC,
    	MESSAGE_TXT,
    	MESSAGE_VTG,
    	MESSAGE_ZDA
	};

    /*--------------------------------------------------------------------------
	 * Public structs
	 *------------------------------------------------------------------------*/

    struct GsvMessage
    {
        ArrayN<uint8_t, 12> satellites;
    };

    struct GsaMessage
    {
        ArrayN<uint8_t, 12> satellites;
        Plat4m::RealNumber positionDilutionOfPrecision;
        Plat4m::RealNumber horizontalDilutionOfPrecision;
        Plat4m::RealNumber verticalDilutionOfPreceision;
    };

    struct RmcMessage
    {
    	Plat4m::RealNumber time;
    	bool isActive;
    	Plat4m::RealNumber latitude;
    	Plat4m::RealNumber longitude;
    	Plat4m::RealNumber groundSpeed;
    	Plat4m::RealNumber trackAngle;
    	uint32_t date;
    	Plat4m::RealNumber magneticVariation;
    };

    /*--------------------------------------------------------------------------
	 * Public typedefs
	 *------------------------------------------------------------------------*/

    typedef ErrorTemplate<ErrorCode> Error;

    typedef Callback<void, GsaMessage&> GsaMessageCallback;

    typedef Callback<void, RmcMessage&> RmcMessageCallback;

    /*--------------------------------------------------------------------------
     * Public constructors
     *------------------------------------------------------------------------*/
    
    ComProtocolNmea();
    
    /*--------------------------------------------------------------------------
     * Public destructors
     *------------------------------------------------------------------------*/

    virtual ~ComProtocolNmea();

    /*--------------------------------------------------------------------------
     * Public methods
     *------------------------------------------------------------------------*/
    
    void setGsaMessageCallback(GsaMessageCallback& gsaMessageCallback);

    void setRmcMessageCallback(RmcMessageCallback& rmcMessageCallback);
    
private:
    
    /*--------------------------------------------------------------------------
     * Private members
     *------------------------------------------------------------------------*/
    
    GsaMessageCallback* myGsaMessageCallback;

    RmcMessageCallback* myRmcMessageCallback;

    /*--------------------------------------------------------------------------
	 * Private methods implemented from ComProtocol
	 *------------------------------------------------------------------------*/

	ParseStatus driverParseData(const ByteArray& rxByteArray,
	                            ByteArray& txByteArray,
	                            unsigned int& bytesConsumed);

	/*--------------------------------------------------------------------------
	 * Private methods
	 *------------------------------------------------------------------------*/

	ParseStatus parseGsaMessage(const ByteArray& byteArray);

	ParseStatus parseRmcMessage(const ByteArray& byteArray);
};

#endif // _COM_PROTOCOL_NMEA_H_
