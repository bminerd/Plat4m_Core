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
/// @file SemaphoreLite.cpp
/// @author Ben Minerd
/// @date 8/27/2021
/// @brief SemaphoreLite class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <limits>

#include <Plat4m_Core/SystemLite/SemaphoreLite.h>

using namespace std;
using namespace Plat4m;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
SemaphoreLite::SemaphoreLite(const uint32_t maxValue,
                             const uint32_t initialValue) :
    Semaphore(maxValue, initialValue),
    myMaxValue(maxValue == 0 ? std::numeric_limits<uint32_t>::max() : maxValue),
    myValue(initialValue)
{

}

//------------------------------------------------------------------------------
// Public destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
SemaphoreLite::~SemaphoreLite()
{
}

//------------------------------------------------------------------------------
// Private virtual methods overridden for Semaphore
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Semaphore::Error SemaphoreLite::driverWait()
{
	if (myValue > 0)
    {
        myValue--;
    }

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
Semaphore::Error SemaphoreLite::driverPost()
{
    if (myValue < myMaxValue)
    {
        myValue++;
    }

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
uint32_t SemaphoreLite::driverGetValue()
{
    return myValue;
}
