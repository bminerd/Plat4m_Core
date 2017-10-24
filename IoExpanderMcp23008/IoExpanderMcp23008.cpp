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
 * @file IoExpanderMcp23008.cpp
 * @author Ben Minerd
 * @date 3/26/2013
 * @brief IoExpanderMcp23008 class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <IoExpanderMcp23008.h>
#include <ByteArrayN.h>

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

static const uint8_t i2cBaseAddress = 0x20;

/*------------------------------------------------------------------------------
 * Public constructors and destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
IoExpanderMcp23008::IoExpanderMcp23008(const PinLevel address0PinLevel,
                                       const PinLevel address1PinLevel,
                                       const PinLevel address2PinLevel,
                                       I2c& i2c,
                                       Interrupt* interrupt) :
    IoExpander(),
    myI2cDevice((i2cBaseAddress             |
                 (address2PinLevel << 2)    |
                 (address1PinLevel << 1)    |
                 address0PinLevel),
                i2c),
    myInterrupt(interrupt),
    myIsInterrupt(false)
{
    if (IS_VALID_POINTER(myInterrupt))
    {
        myInterrupt->setHandlerCallback(
                Plat4m::createCallback(this,
                                       &IoExpanderMcp23008::interruptCallback));
    }
}

/*------------------------------------------------------------------------------
 * Private implemented methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
IoExpander::Error IoExpanderMcp23008::driverEnable(const bool enable)
{
    if (enable)
    {
        memset(myRegisterValueMap, 0, ARRAY_SIZE(myRegisterValueMap));

        myRegisterValueMap[REGISTER_IODIR] = 0xFF;
        myRegisterValueMap[REGISTER_IODIR] = 0xFF;
    }
    
    if (IS_VALID_POINTER(myInterrupt))
    {
        myInterrupt->enable(enable);
        
        if (enable)
        {
            Interrupt::Config interruptConfig;
            interruptConfig.trigger     = Interrupt::TRIGGER_RISING;
            interruptConfig.activeLevel = Interrupt::ACTIVE_LEVEL_HIGH;
            
            myInterrupt->configure(interruptConfig);
            
            if (driverRegisterWriteBit(IoExpanderMcp23008::REGISTER_IOCON,
                                       1,
                                       Plat4m::BIT_VALUE_1) !=
                IoExpander::ERROR_NONE)
            {
                return ERROR_COMMUNICATION;
            }
            
            // Clear interrupt
            uint8_t regValue;
            driverRegisterRead(REGISTER_INTCAP, regValue);
            driverRegisterRead(REGISTER_GPIO, regValue);
        }
    }

    return ERROR_NONE;
}

//------------------------------------------------------------------------------
IoExpander::Error IoExpanderMcp23008::driverRegisterRead(const id_t registerId,
                                                         uint8_t& value)
{
    ByteArrayN<1> txByteArray;
    txByteArray.append((uint8_t) registerId);
    
    ByteArrayN<1> rxByteArray;

    if (myI2cDevice.txRx(txByteArray, rxByteArray) != SlaveDevice::ERROR_NONE)
    {
        return ERROR_COMMUNICATION;
    }
    
    value = rxByteArray[0];
    myRegisterValueMap[registerId] = value;

    return ERROR_NONE;
}

//------------------------------------------------------------------------------
IoExpander::Error IoExpanderMcp23008::driverRegisterWrite(const id_t registerId,
                                                          const uint8_t value)
{
    ByteArrayN<2> byteArray;
    byteArray.append((uint8_t) registerId);
    byteArray.append(value);

    if (myI2cDevice.tx(byteArray) != SlaveDevice::ERROR_NONE)
    {
        return ERROR_COMMUNICATION;
    }
    
    myRegisterValueMap[registerId] = value;

    return ERROR_NONE;
}

//------------------------------------------------------------------------------
IoExpander::Error IoExpanderMcp23008::driverRegisterReadBit(
                                                     const id_t registerId,
                                                     const unsigned int bit,
                                                     Plat4m::BitValue& bitValue)
{
    uint8_t regValue;
    uint8_t mask = ~(1 << bit);
    Error error;
    
    if ((registerId == REGISTER_GPIO) && (IS_VALID_POINTER(myInterrupt)))
    {
        error = driverRegisterRead(REGISTER_INTCAP, regValue);
    }
    else
    {
        error = driverRegisterRead(registerId, regValue);
    }

    if (error == ERROR_NONE)
    {
        bitValue = (Plat4m::BitValue) ((regValue & mask) == mask);
    }

    return error;
}

//------------------------------------------------------------------------------
IoExpander::Error IoExpanderMcp23008::driverRegisterWriteBit(
                                                const id_t registerId,
                                                const unsigned int bit,
                                                const Plat4m::BitValue bitValue)
{
    uint8_t regValue;

    switch (bitValue)
    {
        case Plat4m::BIT_VALUE_0:
        {
            regValue = myRegisterValueMap[registerId] & ~(1 << bit);

            break;
        }
        case Plat4m::BIT_VALUE_1:
        {
            regValue = myRegisterValueMap[registerId] | (1 << bit);

            break;
        }
    }

    return driverRegisterWrite(registerId, regValue);
}

//------------------------------------------------------------------------------
IoExpander::Error IoExpanderMcp23008::driverPinConfigure(
                                                  const id_t pinId,
                                                  const GpioPin::Config& config)
{
    Error error;
    
    // Set IO direction
    // GPIO_MODE_DIGITAL_OUTPUT = 0 = output,
    // GPIO_MODE_DIGITAL_INPUT  = 1 = input
    error = driverRegisterWriteBit(REGISTER_IODIR,
                                   pinId,
                                   (Plat4m::BitValue) (config.mode));
    
    if (error != ERROR_NONE)
    {
        return error;
    }

    if (config.mode == GpioPin::MODE_DIGITAL_INPUT)
    {
        if (config.resistor == GpioPin::RESISTOR_PULL_UP)
        {
            // Set resistor
            // GPIO_RESISTOR_NONE    = 0 = pull-up disabled
            // GPIO_RESISTOR_PULL_UP = 1 = pull-up enabled
            error = registerWriteBit(REGISTER_GPPU, pinId, Plat4m::BIT_VALUE_1);
            
            if (error != ERROR_NONE)
            {
                return error;
            }
        }
        
        if (IS_VALID_POINTER(myInterrupt))
        {
            error = driverRegisterWriteBit(REGISTER_GPINTEN,
                                           pinId,
                                           Plat4m::BIT_VALUE_1);
            
            if (error != ERROR_NONE)
            {
                return error;
            }
        }
    }
    
    return error;
}

//------------------------------------------------------------------------------
IoExpander::Error IoExpanderMcp23008::driverPinSetLevel(
                                                     const id_t pinId,
                                                     const GpioPin::Level level)
{
    return driverRegisterWriteBit(REGISTER_GPIO,
                                  pinId,
                                  (Plat4m::BitValue) level);
}
                                                          
//------------------------------------------------------------------------------
IoExpander::Error IoExpanderMcp23008::driverPinGetLevel(const id_t pinId,
                                                        GpioPin::Level& level)
{
    if (IS_VALID_POINTER(myInterrupt))
    {
        Error error = ERROR_NONE;
        
        if (myIsInterrupt)
        {
            myIsInterrupt = false;
            
            uint8_t regValue;
            error = driverRegisterRead(REGISTER_INTCAP, regValue);
            error = driverRegisterRead(REGISTER_GPIO, regValue);
        }
        
        uint8_t mask = (1 << pinId);
        
        level = (GpioPin::Level)
                ((myRegisterValueMap[REGISTER_GPIO] & mask) == mask);
        
        return error;
    }

    return driverRegisterReadBit(REGISTER_GPIO,
                                 pinId,
                                 (Plat4m::BitValue&) level);
}

/*------------------------------------------------------------------------------
 * Private methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void IoExpanderMcp23008::interruptCallback(const bool isActive)
{
    myIsInterrupt = true;
}
