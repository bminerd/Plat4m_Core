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
// Copyright (c) 2021 Benjamin Minerd
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
/// @file BoardStevalMKI160V1.cpp
/// @author Ben Minerd
/// @date 11/19/2015
/// @brief BoardStevalMKI160V1 class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/BoardStevalMKI160V1/BoardStevalMKI160V1.h>
#include <Plat4m_Core/MemoryAllocator.h>

using Plat4m::BoardStevalMKI160V1;
using Plat4m::ImuLSM6DS3;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
BoardStevalMKI160V1::BoardStevalMKI160V1() :
    Board(),
	myImuLSM6DS3(0)
{
}

//------------------------------------------------------------------------------
// Public destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
BoardStevalMKI160V1::~BoardStevalMKI160V1()
{
	if (isValidPointer(myImuLSM6DS3))
	{
		MemoryAllocator::deallocate(myImuLSM6DS3);
	}
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ImuLSM6DS3& BoardStevalMKI160V1::getImu(
                                       Plat4m::I2c& i2c,
                                       ExternalInterrupt* int1ExternalInterrupt,
                                       ExternalInterrupt* int2ExternalInterrupt)
{
	if (isNullPointer(myImuLSM6DS3))
	{
        const ImuLSM6DS3::PinLevel pinLevel = ImuLSM6DS3::PIN_LEVEL_LOW;

		myImuLSM6DS3 = MemoryAllocator::allocate<ImuLSM6DS3>(
                                                         pinLevel,
                                                         i2c,
                                                         int1ExternalInterrupt,
                                                         int2ExternalInterrupt);
	}

	return (*myImuLSM6DS3);
}
