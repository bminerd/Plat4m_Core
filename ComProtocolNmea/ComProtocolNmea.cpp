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
 * @file ComProtocolNmea.cpp
 * @author Ben Minerd
 * @date 5/28/15
 * @brief ComProtocolNmea class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <ComProtocolNmea.h>

using namespace Plat4m;

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

const char* startFlag 			 = "$";
const int startFlagSize = strlen(startFlag);
const char* endFlag   		     = "\r\n";
const int endFlagSize   = strlen(endFlag);

const char* messageStringMap[] =
{
    "GPDTM", // MESSAGE_DTM
    "GPGBS", // MESSAGE_GBS
    "GPGGA", // MESSAGE_GGA
    "GPGLL", // MESSAGE_GLL
    "GPGLQ", // MESSAGE_GLQ
    "GPGNQ", // MESSAGE_GNQ
    "GPGNS", // MESSAGE_GNS
    "GPGPQ", // MESSAGE_GPQ
    "GPGRS", // MESSAGE_GRS
    "GPGSA", // MESSAGE_GSA
    "GPGST", // MESSAGE_GST
    "GPGSV", // MESSAGE_GSV
    "GPRMC", // MESSAGE_RMC
    "GPTXT", // MESSAGE_TXT
    "GPVTG", // MESSAGE_VTG
    "GPZDA"  // MESSAGE_ZDA
};

/*------------------------------------------------------------------------------
 * Public constructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
ComProtocolNmea::ComProtocolNmea() :
    ComProtocol(100),
    myGsaMessageCallback(NULL_POINTER),
	myRmcMessageCallback(NULL_POINTER)
{
}

/*------------------------------------------------------------------------------
 * Public destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
ComProtocolNmea::~ComProtocolNmea()
{
}

/*------------------------------------------------------------------------------
 * Public methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void ComProtocolNmea::setGsaMessageCallback(
										 GsaMessageCallback& gsaMessageCallback)
{
	myGsaMessageCallback = &gsaMessageCallback;
}

//------------------------------------------------------------------------------
void ComProtocolNmea::setRmcMessageCallback(
                                         RmcMessageCallback& rmcMessageCallback)
{
    myRmcMessageCallback = &rmcMessageCallback;
}

/*------------------------------------------------------------------------------
 * Private methods implemented from ComProtocol
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
ComProtocol::ParseStatus ComProtocolNmea::driverParseData(
                                                   const ByteArray& rxByteArray,
                                                   ByteArray& txByteArray,
                                                   unsigned int& bytesConsumed)
{
	int startIndex = 0;
	int i;
	int nRxBytes = rxByteArray.getSize();

	ParseStatus parseStatus = PARSE_STATUS_MID_MESSAGE;
	bytesConsumed = 0;

	if (nRxBytes < startFlagSize)
	{
	    return PARSE_STATUS_MID_MESSAGE;
	}

	// Look through all incoming bytes for start flag
	while (startIndex < nRxBytes)
	{
		// Look for start flag
		for (i = startIndex; i < nRxBytes; i++)
		{
			if (memcmp(&(rxByteArray[i]), startFlag, startFlagSize) == 0)
			{
				break;
			}
		}

		// If true, we've searched beyond the end of the message
		if (i == nRxBytes)
		{
		    bytesConsumed = rxByteArray.getSize();

			return PARSE_STATUS_NOT_A_MESSAGE;
		}

		startIndex = i + startFlagSize;

		// Look for end flag
		for (i = startIndex; i < nRxBytes; i++)
		{
			if (memcmp(&(rxByteArray[i]), endFlag, endFlagSize) == 0)
			{
				break;
			}
		}

		// If true, we've searched beyond the end of the message
		if (i == nRxBytes)
		{
			return PARSE_STATUS_MID_MESSAGE;
		}

		int endIndex = i;

		Message message;

		// Look for message string
		for (i = 0; i < ARRAY_SIZE(messageStringMap); i++)
		{
			if (memcmp(&(rxByteArray[startIndex]),
			           messageStringMap[i],
			           strlen(messageStringMap[i])) == 0)
			{
				break;
			}
		}

		// Verify message string was found
		if (i == ARRAY_SIZE(messageStringMap))
		{
			parseStatus = PARSE_STATUS_INVALID_MESSAGE;

			bytesConsumed += endIndex - startIndex + endFlagSize + 1;
		}
		else
		{
			message = (Message) i;

			// Bytes are consumed whether message is supported or not
			bytesConsumed += endIndex - startIndex + 1;

			switch (message)
			{
			    case MESSAGE_GSA:
			    {
			        ByteArray messageByteArray =
                        rxByteArray.subArray(startIndex + 6,
                                             endIndex - startIndex - 6 + 1);
                    parseStatus = parseGsaMessage(messageByteArray);

                    break;
			    }
				case MESSAGE_RMC:
				{
                    ByteArray messageByteArray =
                        rxByteArray.subArray(startIndex + 6,
                                             endIndex - startIndex - 6 + 1);
                    parseStatus = parseRmcMessage(messageByteArray);

					break;
				}
				default:
				{
				    parseStatus = PARSE_STATUS_UNSUPPORTED_MESSAGE;

				    break;
				}
			}
		}

		startIndex = endIndex + endFlagSize + 1;
	}

	return parseStatus;
}

/*------------------------------------------------------------------------------
 * Private methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
ComProtocol::ParseStatus ComProtocolNmea::parseGsaMessage(
                                                     const ByteArray& byteArray)
{
    if (isNullPointer(myGsaMessageCallback))
    {
        return PARSE_STATUS_UNSUPPORTED_MESSAGE;
    }

    GsaMessage gsaMessage;

    char opMode;
    uint8_t navMode;
    char checksum[4];

    sscanf((char*) &(byteArray[0]),
           "%c,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%lf,%lf,%lf,%s",
           &opMode,
           &navMode,
           &(gsaMessage.satellites[0]),
           &(gsaMessage.satellites[1]),
           &(gsaMessage.satellites[2]),
           &(gsaMessage.satellites[3]),
           &(gsaMessage.satellites[4]),
           &(gsaMessage.satellites[5]),
           &(gsaMessage.satellites[6]),
           &(gsaMessage.satellites[7]),
           &(gsaMessage.satellites[8]),
           &(gsaMessage.satellites[9]),
           &(gsaMessage.satellites[10]),
           &(gsaMessage.satellites[11]),
           &(gsaMessage.positionDilutionOfPrecision),
           &(gsaMessage.horizontalDilutionOfPrecision),
           &(gsaMessage.verticalDilutionOfPreceision),
           checksum);

    gsaMessage.satellites.setSize(12);

    myGsaMessageCallback->call(gsaMessage);

    return PARSE_STATUS_FOUND_MESSAGE;
}

//------------------------------------------------------------------------------
ComProtocol::ParseStatus ComProtocolNmea::parseRmcMessage(
												     const ByteArray& byteArray)
{
    if (isNullPointer(myRmcMessageCallback))
    {
        return PARSE_STATUS_UNSUPPORTED_MESSAGE;
    }

    RmcMessage rmcMessage;

	char active;
	char latitudeDirection;
	char longitudeDirection;
	char magneticVariationDirection;
	char checksum[4];

	sscanf((char*) &(byteArray[0]),
		   "%lf,%c,%lf,%c,%lf,%c,%lf,%lf,%u,%lf,%c%s",
		   &(rmcMessage.time),
		   &active,
		   &(rmcMessage.latitude),
		   &latitudeDirection,
		   &(rmcMessage.longitude),
		   &longitudeDirection,
		   &(rmcMessage.groundSpeed),
		   &(rmcMessage.trackAngle),
		   &(rmcMessage.date),
		   &(rmcMessage.magneticVariation),
		   &magneticVariationDirection,
		   checksum);

	if (active == 'A')
	{
		rmcMessage.isActive = true;
	}

	if (latitudeDirection == 'S')
	{
		rmcMessage.latitude *= -1;
	}

	if (longitudeDirection == 'W')
	{
		rmcMessage.longitude *= -1;
	}

	if (magneticVariationDirection == 'W')
	{
		rmcMessage.magneticVariation *= -1;
	}

	myRmcMessageCallback->call(rmcMessage);

	return PARSE_STATUS_FOUND_MESSAGE;
}
