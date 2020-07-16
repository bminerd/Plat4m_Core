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
/// @file BoardStevalMKI160V1.h
/// @author Ben Minerd
/// @date 11/19/2015
/// @brief BoardStevalMKI160V1 class header file.
///

#ifndef PLAT4M_BOARD_STEVAL_MKI160V1_H
#define PLAT4M_BOARD_STEVAL_MKI160V1_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Board.h>
#include <Plat4m_Core/ImuLSM6DS3/ImuLSM6DS3.h>
#include <Plat4m_Core/I2c.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class BoardStevalMKI160V1 : public Board
{
public:
    
    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------
    
    BoardStevalMKI160V1();
    
    //--------------------------------------------------------------------------
    // Public destructors
    //--------------------------------------------------------------------------

    ~BoardStevalMKI160V1();

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    ImuLSM6DS3& getImu(Plat4m::I2c& i2c,
                       ExternalInterrupt* int1ExternalInterrupt = 0,
                       ExternalInterrupt* int2ExternalInterrupt = 0);
    
private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    ImuLSM6DS3* myImuLSM6DS3;
};

}; // namespace Plat4m

#endif // PLAT4M_BOARD_STEVAL_MKI160V1_H
