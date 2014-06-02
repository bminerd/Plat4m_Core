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
 * @file DacIcLtc2637.cpp
 * @author Ben Minerd
 * @date 10/23/2012
 * @brief DacIcLtc2637 class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <DacIcLtc2637.h>
#include <ByteArrayN.h>

/*------------------------------------------------------------------------------
 * Public constructors and destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
DacIcLtc2637::DacIcLtc2637(I2c& i2c,
                           const PinLevel address0PinLevel,
                           const PinLevel address1PinLevel,
                           const PinLevel address2PinLevel) :
    DacIc(),
    myI2cDevice((0x10 | address0PinLevel), i2c) // Other address pins?
{
}

/*------------------------------------------------------------------------------
 * Private implemented methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
DacIc::Error DacIcLtc2637::driverEnable(const bool enable)
{
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
DacIc::Error DacIcLtc2637::driverRegisterRead(const id_t registerId,
                                              uint16_t& value)
{
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
DacIc::Error DacIcLtc2637::driverRegisterWrite(const id_t registerId,
                                               const uint16_t value)
{
    ByteArrayN<3> data;
    data.append((uint8_t) registerId);
    data.append((uint8_t) (value >> 4));
    data.append((uint8_t) (value << 4));

    if ((myI2cDevice.tx(data)) != SlaveDevice::ERROR_NONE)
    {
        return ERROR_COMMUNICATION;
    }
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
DacIc::Error DacIcLtc2637::driverRegisterReadBit(const id_t registerId,
                                                 const unsigned int bit,
                                                 Plat4m::BitValue& bitValue)
{
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
DacIc::Error DacIcLtc2637::driverRegisterWriteBit(
                                                const id_t registerId,
                                                const unsigned int bit,
                                                const Plat4m::BitValue bitValue)
{
    return ERROR_NONE;
}
