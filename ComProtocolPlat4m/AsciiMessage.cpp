//------------------------------------------------------------------------------
//       _______    __                           ___
//      ||  ___ \  || |             __          //  |
//      || |  || | || |   _______  || |__      //   |    _____  ___
//      || |__|| | || |  // ___  | ||  __|    // _  |   ||  _ \/ _ \
//      ||  ____/  || | || |  || | || |      // /|| |   || |\\  /\\ \
//      || |       || | || |__|| | || |     // /_|| |_  || | || | || |
//      || |       || |  \\____  | || |__  //_____   _| || | || | || |
//      ||_|       ||_|       ||_|  \\___|       ||_|   ||_| ||_| ||_|
//
//
// The MIT License (MIT)
//
// Copyright (c) 2016 Benjamin Minerd
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------------

///
/// @file AsciiMessage.cpp
/// @author Ben Minerd
/// @date 4/29/16
/// @brief AsciiMessage class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <AsciiMessage.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

using Plat4m::AsciiMessage;
using Plat4m::ComProtocol;
using Plat4m::Array;
using Plat4m::ByteArray;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const char AsciiMessage::myStartCharacter              = '(';
const char AsciiMessage::myEndCharacter                = ')';
const char AsciiMessage::myCommandSeparatorCharacter   = '|';
const char AsciiMessage::myAssignmentCharacter         = '=';
const char AsciiMessage::myParameterSeparatorCharacter = ',';

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
AsciiMessage::AsciiMessage() :
    myName(0),
    myHasParameters(false),
    myParameterNames(0),
    myParameterValues(0)
{
}

//------------------------------------------------------------------------------
AsciiMessage::AsciiMessage(const ByteArray& name,
                           const Array<ByteArray>* parameterNames) :
    myName(&name),
    myHasParameters(false),
    myParameterNames(parameterNames),
    myParameterValues(0)
{
    if (isValidPointer(parameterNames))
    {
        myHasParameters = true;
    }
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
AsciiMessage::~AsciiMessage()
{
}

//------------------------------------------------------------------------------
// Private virtual methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void AsciiMessage::stringParametersUpdated()
{
    // Intentionally blank, not implemented by subclass
}

//------------------------------------------------------------------------------
void AsciiMessage::messageParametersUpdated()
{
    // Intentionally blank, not implemented by subclass
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
const ByteArray* AsciiMessage::getName() const
{
    return myName;
}

//------------------------------------------------------------------------------
bool AsciiMessage::hasParameters() const
{
    return myHasParameters;
}

//------------------------------------------------------------------------------
const Array<ByteArray>* AsciiMessage::getParameterNames() const
{
    return myParameterNames;
}

//------------------------------------------------------------------------------
Array<ByteArray>* AsciiMessage::getParameterValues()
{
    return myParameterValues;
}

//------------------------------------------------------------------------------
Array<ByteArray>* AsciiMessage::getParameterValues() const
{
    return myParameterValues;
}

//------------------------------------------------------------------------------
void AsciiMessage::setParameterValues(Array<ByteArray>* parameterValues)
{
    myParameterValues = parameterValues;
    myParameterValues->setSize(myParameterNames->getSize());
}

//------------------------------------------------------------------------------
bool AsciiMessage::toByteArray(ByteArray& byteArray) const
{
    if (!(byteArray.append(myStartCharacter)) |
        !(byteArray.append(*myName))          |
        !(byteArray.append(myCommandSeparatorCharacter)))
    {
        return false;
    }

    int size = myParameterNames->getSize();

    for (int i = 0; i < size; i++)
    {
        if (!(byteArray.append(myParameterNames->getItem(i)))  |
            !(byteArray.append(myAssignmentCharacter)) |
            !(byteArray.append(myParameterValues->getItem(i))))
        {
            return false;
        }

        if (i < (size - 1))
        {
            if (!(byteArray.append(myParameterSeparatorCharacter)))
            {
                return false;
            }
        }
    }

    return (byteArray.append(myEndCharacter));
}

//------------------------------------------------------------------------------
bool AsciiMessage::match(const AsciiMessage& message) const
{
    // Compare name lengths
    if (myName->getSize() != message.getName()->getSize())
    {
        return false;
    }

    // Compare name strings
    if (memcmp(myName->getItems(),
               message.getName()->getItems(),
               myName->getSize())             != 0)
    {
        return false;
    }

    // Compare if message has parameters
    if (hasParameters() != message.hasParameters())
    {
        return false;
    }

    if (hasParameters())
    {
        int size = myParameterNames->getSize();

        // Compare number of parameters
        if (size != message.getParameterNames()->getSize())
        {
            return false;
        }

        // Compare parameters
        for (int i = 0; i < size; i++)
        {
            ByteArray& name1 = myParameterNames->getItem(i);
            ByteArray& name2 = message.getParameterNames()->getItem(i);

            // Compare parameter[i] name length
            if (name1.getSize() != name2.getSize())
            {
                return false;
            }

            // Compare parameter name strings
            if (memcmp(name1.getItems(),
                       name2.getItems(),
                       name1.getSize())  != 0)
            {
                return false;
            }
        }
    }

    // Everything matched
    return true;
}

//------------------------------------------------------------------------------
ComProtocol::ParseStatus AsciiMessage::parseByteArray(
                                              const ByteArray& byteArray,
                                              ByteArray& name,
                                              Array<ByteArray>& parameterNames,
                                              Array<ByteArray>& parameterValues)
{
    // Look for start and end characters before trying to parse message

    int messageStartIndex = 0;
    int messageEndIndex = 0;
    int i = 0;
    int size = byteArray.getSize();

    for (i = 0; i < size; i++)
    {
        if (byteArray[i] == myStartCharacter)
        {
            break;
        }
    }

    // If reached the end of byteArray, end character wasn't found
    if (i == size)
    {
        return ComProtocol::PARSE_STATUS_NOT_A_FRAME;
    }

    messageStartIndex = i;

    for (i = 0; i < size; i++)
    {
        if (byteArray[i] == myEndCharacter)
        {
            break;
        }
    }

    // If reached the end of byteArray, end character wasn't found
    if (i == size)
    {
        return ComProtocol::PARSE_STATUS_MID_FRAME;
    }

    messageEndIndex = i;

    myHasParameters = false;

    for (i = (messageStartIndex + 1); i < messageEndIndex; i++)
    {
        if (byteArray[i] == myCommandSeparatorCharacter)
        {
            myHasParameters = true;

            break;
        }
    }

    int commandSeparatorIndex = i;

    name.clear(true);

    ByteArray nameByteArray;

    if (i == messageEndIndex)
    {
        nameByteArray = byteArray.subArray(messageStartIndex + 1,
                                           messageEndIndex - messageStartIndex - 1);
    }
    else
    {
        nameByteArray = byteArray.subArray(messageStartIndex + 1,
                                           i - 1 - messageStartIndex);
    }

    name.append(nameByteArray);

    int nParameterNames = parameterNames.getMaxSize();

    for (int i = 0; i < nParameterNames; i++)
    {
        parameterNames[i].clear(true);
        parameterValues[i].clear(true);
    }

    parameterNames.clear();
    parameterValues.clear();

    // If this message doesn't have parameters, no need to continue
    if (!myHasParameters)
    {
        myName            = &name;
        myParameterNames  = &parameterNames;
        myParameterValues = &parameterValues;

        return ComProtocol::PARSE_STATUS_FOUND_FRAME;
    }

    bool stillParsing = true;
    int nParameters = 0;
    int parameterStartIndex = commandSeparatorIndex + 1;

    while (stillParsing)
    {
        for (i = parameterStartIndex; i < size; i++)
        {
            if ((byteArray[i] == myParameterSeparatorCharacter) ||
                (byteArray[i] == myEndCharacter))
            {
                if (byteArray[i] == myEndCharacter)
                {
                    stillParsing = false;
                }

                // Look for assignment character

                int assignmentCharacterIndex = 0;
                int j;

                for (j = parameterStartIndex; j < i; j++)
                {
                    if (byteArray[j] == myAssignmentCharacter)
                    {
                        break;
                    }
                }

                if (j == i)
                {
                    // No parameter name
                }

                assignmentCharacterIndex = j;

                ByteArray parameterName =
                                byteArray.subArray(parameterStartIndex,
                                                   (assignmentCharacterIndex -
                                                    parameterStartIndex));

                // Copy parameter name
                parameterNames.getItem(nParameters).append(parameterName);

                ByteArray parameterValue =
                         byteArray.subArray(assignmentCharacterIndex + 1,
                                            (i - assignmentCharacterIndex - 1));

                // Copy parameter value
                parameterValues.getItem(nParameters).append(parameterValue);

                nParameters++;

                parameterNames.setSize(nParameters);
                parameterValues.setSize(nParameters);

                parameterStartIndex = i + 1;

                break;
            }
        }

        if (i == size)
        {
            return ComProtocol::PARSE_STATUS_MID_FRAME;
        }
    }

    myName            = &name;
    myParameterNames  = &parameterNames;
    myParameterValues = &parameterValues;

    return ComProtocol::PARSE_STATUS_FOUND_FRAME;
}

//------------------------------------------------------------------------------
uint8_t AsciiMessage::getUint8Parameter(const uint32_t index) const
{
    const char* string =
                   (const char*) (myParameterValues->getItem(index).getItems());
    return ((uint8_t) strtoul(string, 0, 0));
}

//------------------------------------------------------------------------------
void AsciiMessage::setUint8Parameter(const uint32_t index, const uint8_t value)
{
    char string[20];

    sprintf(string, "%u", value);
    myParameterValues->getItem(index).clear(true);
    myParameterValues->getItem(index).append(string);
}

//------------------------------------------------------------------------------
uint32_t AsciiMessage::getUint32Parameter(const uint32_t index) const
{
    const char* string =
                   (const char*) (myParameterValues->getItem(index).getItems());
    return ((uint32_t) strtoul(string, 0, 0));
}

//------------------------------------------------------------------------------
void AsciiMessage::setUint32Parameter(const uint32_t index,
                                      const uint32_t value)
{
    char string[20];

    sprintf(string, "%u", value);
    myParameterValues->getItem(index).clear(true);
    myParameterValues->getItem(index).append(string);
}

//------------------------------------------------------------------------------
float AsciiMessage::getFloatParameter(const uint32_t index) const
{
    const char* string =
                   (const char*) (myParameterValues->getItem(index).getItems());
    return (strtof(string, 0));
}

//------------------------------------------------------------------------------
void AsciiMessage::setFloatParameter(const uint32_t index,
                                     const float value,
                                     const uint8_t precision)
{
    char formatString[] =
    {
        '%',
        '.',
        uint8ToAscii(precision),
        'f',
        '\0'
    };
    char string[20];

    sprintf(string, formatString, value);
    myParameterValues->getItem(index).clear(true);
    myParameterValues->getItem(index).append(string);
}

//------------------------------------------------------------------------------
const char* AsciiMessage::getStringParameter(const uint32_t index) const
{
    return ((const char*) (myParameterValues->getItem(index).getItems()));
}

//------------------------------------------------------------------------------
void AsciiMessage::setStringParameter(const uint32_t index, const char* string)
{
    myParameterValues->getItem(index).clear(true);
    myParameterValues->getItem(index).append(string);
}
