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
// Copyright (c) 2017 Benjamin Minerd
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
/// @file DigitalFilterFir.h
/// @author Ben Minerd
/// @date 4/5/2017
/// @brief DigitalFilterFir class.
///

#ifndef PLAT4M_DIGITAL_FILTER_FIR_H
#define PLAT4M_DIGITAL_FILTER_FIR_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <DigitalFilter.h>
#include <ArrayN.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

template <uint32_t Order>
class DigitalFilterFir : public DigitalFilter
{
public:
    
    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    DigitalFilterFir(const FrequencyHz cutoffFrequencyHz,
                     const FrequencyHz updateFrequencyHz) :
        DigitalFilter(),
		myCoefficients(),
		myInputs(true)
    {
    	reset();
    }
    
    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual ~DigitalFilterFir()
    {
    }

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    void setCoefficients(const ArrayN<RealNumber, (Order + 1)>& coefficients)
    {
    	myCoefficients = coefficients;
    }

    //--------------------------------------------------------------------------
    void reset()
    {
    }

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    ArrayN<RealNumber, (Order + 1)> myCoefficients;

    ArrayN<RealNumber, (Order + 1)> myInputs;

    //--------------------------------------------------------------------------
    // Private virtual methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    RealNumber driverUpdate(const RealNumber input)
    {
    	// Advance inputs

    	uint32_t size = myInputs.getSize();

    	for (uint32_t i = (size - 1); i > 0; i--)
    	{
    		myInputs[i] = myInputs[(i - 1)];
    	}

    	myInputs[0] = input;

        RealNumber output = 0.0;

        size = myCoefficients.getSize();

        for (uint32_t i = 0; i < size; i++)
        {
        	output += myCoefficients[i] * myInputs[i];
        }

        return output;
    }
};

}; // namespace Plat4m

#endif // PLAT4M_DIGITAL_FILTER_FIR_H
