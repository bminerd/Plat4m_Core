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
/// @file AsciiMessage.h
/// @author Ben Minerd
/// @date 4/29/2016
/// @brief AsciiMessage class header file.
///

#ifndef PLAT4M_ASCII_MESSAGE_H
#define PLAT4M_ASCII_MESSAGE_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Array.h>
#include <Plat4m_Core/ByteArray.h>
#include <Plat4m_Core/ComProtocol.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class AsciiMessage
{
public:
    
    //--------------------------------------------------------------------------
    // Public structures
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    AsciiMessage();

    AsciiMessage(const ByteArray& name, const Array<ByteArray>* parameterNames);

    //--------------------------------------------------------------------------
    // Public destructors
    //--------------------------------------------------------------------------

    virtual ~AsciiMessage();

    //--------------------------------------------------------------------------
    // Public virtual methods
    //--------------------------------------------------------------------------

    virtual void stringParametersUpdated();

    virtual void messageParametersUpdated();

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    const ByteArray* getName() const;

    bool hasParameters() const;

    const Array<ByteArray>* getParameterNames() const;

    Array<ByteArray>* getParameterValues();

    Array<ByteArray>* getParameterValues() const;

    void setParameterValues(Array<ByteArray>* parameterValues);

    bool toByteArray(ByteArray& byteArray) const;

    bool match(const AsciiMessage& message) const;

    // TODO: Does it make sense to return ComProtocl::ParseStatus?
    ComProtocol::ParseStatus parseByteArray(const ByteArray& byteArray,
                                            ByteArray& name,
                                            Array<ByteArray>& parameterNames,
                                            Array<ByteArray>& parameterValues);

    uint8_t getUint8Parameter(const uint32_t index) const;

    void setUint8Parameter(const uint32_t index, const uint8_t value);

    uint32_t getUint32Parameter(const uint32_t index) const;

    void setUint32Parameter(const uint32_t index, const uint32_t value);

    float getFloatParameter(const uint32_t index) const;

    void setFloatParameter(const uint32_t index,
                           const float value,
                           const uint8_t precision = 5);

    const char* getStringParameter(const uint32_t index) const;

    void setStringParameter(const uint32_t index, const char* string);

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    static const char myStartCharacter;

    static const char myEndCharacter;

    static const char myCommandSeparatorCharacter;

    static const char myAssignmentCharacter;

    static const char myParameterSeparatorCharacter;
    
    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    const ByteArray* myName;

    bool myHasParameters;

    const Array<ByteArray>* myParameterNames;

    Array<ByteArray>* myParameterValues;
};

}; // namespace Plat4m

#endif // PLAT4M_ASCII_MESSAGE_H
