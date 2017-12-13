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
/// @file GpioPortSTM32F4xx.h
/// @author Ben Minerd
/// @date 3/22/13
/// @brief GpioPortSTM32F4xx class header file.
///

#ifndef PLAT4M_GPIO_PORT_STM32F4XX_H
#define PLAT4M_GPIO_PORT_STM32F4XX_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <stdint.h>

#include <stm32f4xx.h>

#include <GpioPort.h>
#include <ProcessorSTM32F4xx.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class GpioPortSTM32F4xx : public GpioPort<uint16_t>
{
public:
    
    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------

    enum Id
    {
        ID_A = 0,
        ID_B,
        ID_C,
        ID_D,
        ID_E,
        ID_F,
        ID_G,
        ID_H,
        ID_I
    };

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------
    
    GpioPortSTM32F4xx(const Id id);
    
    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~GpioPortSTM32F4xx();

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    Id getId() const;

    GPIO_TypeDef* getPort();

    //--------------------------------------------------------------------------
    // Public methods implemented from GpioPort
    //--------------------------------------------------------------------------

    void setValueFast(const uint16_t value);

    uint16_t getValueFast();

    uint16_t readValueFast();

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    // Constants

    static GPIO_TypeDef* myPortMap[];

    static const ProcessorSTM32F4xx::Peripheral myPeripheralMap[];

    static const uint32_t myModeMap[];

    static const uint32_t myResistorMap[];

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------
    
    const Id myId;
    
    GPIO_TypeDef* myPort;
    
    //--------------------------------------------------------------------------
    // Private methods implemented from Module
    //--------------------------------------------------------------------------

    Module::Error driverSetEnabled(const bool enable);
    
    //--------------------------------------------------------------------------
    // Private methods implemented from GpioPort
    //--------------------------------------------------------------------------
    
    GpioPort<uint16_t>::Error driverConfigure(const Config& config);
    
    GpioPort<uint16_t>::Error driverSetValue(const uint16_t value);
    
    GpioPort<uint16_t>::Error driverGetValue(uint16_t& value);
    
    GpioPort<uint16_t>::Error driverReadValue(uint16_t& value);
};

}; // namespace Plat4m

#endif // PLAT4M_GPIO_PORT_STM32F4XX_H
