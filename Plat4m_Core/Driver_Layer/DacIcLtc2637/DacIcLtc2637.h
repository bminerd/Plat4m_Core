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
 * @file DacIcLtc2637.h
 * @author Ben Minerd
 * @date 4/3/2013
 * @brief DacIcLtc2637 class.
 */

#ifndef _DAC_IC_LTC2637_H_
#define _DAC_IC_LTC2637_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <DacIc.h>
#include <I2cDevice.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

#define DAC_IC_LTC2637_CHANNEL_COUNT    8

#define DAC_IC_LTC2637_REG_COUNT        8

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

class DacIcLtc2637 : public DacIc
{
public:
    
    /*--------------------------------------------------------------------------
     * Public enumerations
     *------------------------------------------------------------------------*/
    
    enum Command
    {
        COMMAND_WRITE_INPUT_N = 0,
        COMMAND_UPDATE_DAC_N,
        COMMAND_WRITE_N_UPDATE_ALL,
        COMMAND_WRITE_N_UPDATE_N,
        COMMAND_POWER_DOWN_N,
        COMMAND_POWER_DOWN_CHIP,
        COMMAND_INTERNAL_REF,
        COMMAND_EXTERNAL_REF
    };
    
    enum Address
    {
        ADDRESS_DAC_A = 0,
        ADDRESS_DAC_B,
        ADDRESS_DAC_C,
        ADDRESS_DAC_D,
        ADDRESS_DAC_E,
        ADDRESS_DAC_F,
        ADDRESS_DAC_G,
        ADDRESS_DAC_H,
        ADDRESS_DAC_ALL
    };
    
    enum PinLevel
    {
        PIN_LEVEL_LOW = 0,
        PIN_LEVEL_FLOATING,
        PIN_LEVEL_HIGH
    };
    
    /*--------------------------------------------------------------------------
     * Public constructors and destructors
     *------------------------------------------------------------------------*/
    
    DacIcLtc2637(I2c& i2c,
                 const PinLevel address0PinLevel,
                 const PinLevel address1PinLevel = PIN_LEVEL_LOW,
                 const PinLevel address2PinLevel = PIN_LEVEL_LOW);
    
private:
    
    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/
    
    I2cDevice myI2cDevice;
    
    uint16_t myRegisterValueMap[10]; // Modify
    
    /*--------------------------------------------------------------------------
     * Private implemented methods
     *------------------------------------------------------------------------*/
    
    Error driverEnable(const bool enable);
    
    Error driverRegisterRead(const id_t registerId, uint16_t& value);
    
    Error driverRegisterWrite(const id_t registerId, const uint16_t value);
    
    Error driverRegisterReadBit(const id_t registerId,
                                const unsigned int bit,
                                Plat4m::BitValue& bitValue);
    
    Error driverRegisterWriteBit(const id_t registerId,
                                 const unsigned int bit,
                                 const Plat4m::BitValue bitValue);
    
};

#endif // _DAC_IC_LTC2637_H_
