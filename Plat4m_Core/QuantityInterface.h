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
// Copyright (c) 2023 Benjamin Minerd
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
/// @file QuantityInterface.h
/// @author Ben Minerd
/// @date 5/8/2023
/// @brief QuantityInterface class header file.
///

#ifndef PLAT4M_QUANTITY_INTERFACE_H
#define PLAT4M_QUANTITY_INTERFACE_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>

#include <Plat4m_Core/QuantityBase.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Structs
//------------------------------------------------------------------------------

template <typename ValueType>
class QuantityInterface : public QuantityBase
{
public:

    //--------------------------------------------------------------------------
    // Public operator overloads
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    QuantityInterface& operator=(const QuantityInterface& quantity)
    {
        myValue = quantity.myValue;

        return (*this);
    }

    //--------------------------------------------------------------------------
    bool operator>(const QuantityInterface& quantity) const
    {
        return (myValue > quantity.myValue);
    }

    //--------------------------------------------------------------------------
    bool operator<(const QuantityInterface& quantity) const
    {
        return (myValue < quantity.myValue);
    }

    //--------------------------------------------------------------------------
    bool operator==(const QuantityInterface& quantity) const
    {
        return equals(myValue, quantity.myValue);
    }

    //--------------------------------------------------------------------------
    bool operator!=(const QuantityInterface& quantity) const
    {
        return (myValue != quantity.myValue);
    }

    //--------------------------------------------------------------------------
    bool operator>=(const QuantityInterface& quantity) const
    {
        return (myValue >= quantity.myValue);
    }

    //--------------------------------------------------------------------------
    bool operator<=(const QuantityInterface& quantity) const
    {
        return (myValue <= quantity.myValue);
    }

    //--------------------------------------------------------------------------
    QuantityInterface operator+(const QuantityInterface& quantity) const
    {
        QuantityInterface result(myValue + quantity.myValue);

        return result;
    }

    //--------------------------------------------------------------------------
    QuantityInterface& operator+=(const QuantityInterface& quantity)
    {
        myValue += quantity.myValue;

        return (*this);
    }

    //--------------------------------------------------------------------------
    QuantityInterface operator-(const QuantityInterface& quantity) const
    {
        QuantityInterface result(myValue - quantity.myValue);

        return result;
    }

    //--------------------------------------------------------------------------
    QuantityInterface& operator-=(const QuantityInterface& quantity)
    {
        myValue -= quantity.myValue;

        return (*this);
    }

    //--------------------------------------------------------------------------
    QuantityInterface operator%(const QuantityInterface& quantity) const
    {
        QuantityInterface result(myValue % quantity.myValue);

        return result;
    }

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    const ValueType& get() const
    {
        return myValue;
    }

    //--------------------------------------------------------------------------
    ValueType& get()
    {
        return myValue;
    }

    //--------------------------------------------------------------------------
    void set(const ValueType& value)
    {
        myValue = value;
    }

    //--------------------------------------------------------------------------
    void reset()
    {
        myValue = 0;
    }

protected:

    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    QuantityInterface() :
        QuantityBase(),
        myValue(0)
    {
    }

    //--------------------------------------------------------------------------
    explicit QuantityInterface(const ValueType& value) :
        QuantityBase(),
        myValue(value)
    {
    }

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    ValueType myValue;
};

}; // namespace Plat4m

#endif // PLAT4M_QUANTITY_INTERFACE_H
