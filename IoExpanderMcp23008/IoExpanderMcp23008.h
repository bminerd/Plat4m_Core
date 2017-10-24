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
 * @file IoExpanderMcp23008.h
 * @author Ben Minerd
 * @date 3/26/2013
 * @brief IoExpanderMcp23008 class.
 */

#ifndef _IO_EXPANDER_MCP23008_H_
#define _IO_EXPANDER_MCP23008_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <IoExpander.h>
#include <I2cDevice.h>
#include <Interrupt.h>
#include <GpioPin.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

#define IO_EXPANDER_MCP23008_IO_PORT_COUNT  1
#define IO_EXPANDER_MCP23008_IO_PIN_COUNT   8

#define IO_EXPANDER_MCP23008_REGISTER_COUNT      11

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Structures
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

class IoExpanderMcp23008 : public IoExpander
{
public:
    
    /*--------------------------------------------------------------------------
     * Public enumerations
     *------------------------------------------------------------------------*/
    
    /**
     * @brief Enumeration of IO expander registers.
     */
    enum Register
    {
        REGISTER_IODIR = 0,
        REGISTER_IPOL,
        REGISTER_GPINTEN,
        REGISTER_DEFVAL,
        REGISTER_INTCON,
        REGISTER_IOCON,
        REGISTER_GPPU,
        REGISTER_INTF,
        REGISTER_INTCAP,
        REGISTER_GPIO,
        REGISTER_OLAT
    };
    
    enum PinLevel
    {
        PIN_LEVEL_LOW = 0,
        PIN_LEVEL_HIGH
    };
    
    /*--------------------------------------------------------------------------
     * Public structures
     *------------------------------------------------------------------------*/
    
    struct Config
    {
        int a; // Place holder
    };
    
    /*--------------------------------------------------------------------------
     * Public constructors and destructors
     *------------------------------------------------------------------------*/
    
    IoExpanderMcp23008(const PinLevel address0PinLevel,
                       const PinLevel address1PinLevel,
                       const PinLevel address2PinLevel,
                       I2c& i2c,
                       Interrupt* interrupt = NULL_POINTER);
    
private:
    
    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/
    
    I2cDevice myI2cDevice;
    
    uint8_t myRegisterValueMap[IO_EXPANDER_MCP23008_REGISTER_COUNT];
    
    Interrupt* myInterrupt;
    
    volatile bool myIsInterrupt;
    
    /*--------------------------------------------------------------------------
     * Private implemented methods
     *------------------------------------------------------------------------*/
    
    Error driverEnable(const bool enable);
    
    Error driverRegisterRead(const id_t registerId, uint8_t& value);
    
    Error driverRegisterWrite(const id_t registerId, const uint8_t value);
    
    Error driverRegisterReadBit(const id_t registerId,
                                const unsigned int bit,
                                Plat4m::BitValue& bitValue);
    
    Error driverRegisterWriteBit(const id_t registerId,
                                 const unsigned int bit,
                                 const Plat4m::BitValue bitValue);
    
    Error driverPinConfigure(const id_t pinId, const GpioPin::Config& config);
    
    Error driverPinSetLevel(const id_t pinId, const GpioPin::Level level);
    
    Error driverPinGetLevel(const id_t pinId, GpioPin::Level& level);
    
    /*--------------------------------------------------------------------------
     * Private methods
     *------------------------------------------------------------------------*/
    
    void interruptCallback(const bool isActive);
};

#endif // _IO_EXPANDER_MCP23008_H_
