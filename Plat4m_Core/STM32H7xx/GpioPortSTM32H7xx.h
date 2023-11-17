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
// Copyright (c) 2022-2023 Benjamin Minerd
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
/// @file GpioPortSTM32H7xx.h
/// @author Ben Minerd
/// @date 6/22/2022
/// @brief GpioPortSTM32H7xx class header file.
///

#ifndef PLAT4M_GPIO_PORT_STM32H7XX_H
#define PLAT4M_GPIO_PORT_STM32H7XX_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>

#include <stm32h7xx.h>

#include <Plat4m_Core/GpioPort.h>
#include <Plat4m_Core/STM32H7xx/ProcessorSTM32H7xx.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class GpioPortSTM32H7xx : public GpioPort<std::uint16_t>
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
        ID_I,
        ID_J,
        ID_K
    };

    struct Registers
    {
        struct BSRR
        {
            enum : std::uint32_t
            {
                BS = bitMask<std::uint32_t>(16,  0),
                BR = bitMask<std::uint32_t>(16, 16)
            };
        };
    };

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    GpioPortSTM32H7xx(const Id id);

    //--------------------------------------------------------------------------
    // Public methods implemented from GpioPort
    //--------------------------------------------------------------------------

    void setValueFast(const std::uint16_t value);

    std::uint16_t getValueFast();

    std::uint16_t readValueFast();

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    Id getId();

    GPIO_TypeDef*& getPort();

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    // Constants

    static const ProcessorSTM32H7xx::Peripheral myPeripheralMap[];

    static const std::uint32_t myModeMap[];

    static const std::uint32_t myResistorMap[];

    // Variables

    static GPIO_TypeDef* myPortMap[];

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    const Id myId;

    GPIO_TypeDef* myPort;

    //--------------------------------------------------------------------------
    // Private virtual methods overridden for Module
    //--------------------------------------------------------------------------

    virtual Module::Error driverSetEnabled(const bool enabled) override;

    //--------------------------------------------------------------------------
    // Private virtual methods overridden for GpioPort
    //--------------------------------------------------------------------------

    virtual Error driverConfigure(const Config& config) override;

    virtual Error driverSetValue(const std::uint16_t value) override;

    virtual Error driverGetValue(uint16_t& value) override;

    virtual Error driverReadValue(uint16_t& value) override;
};

}; // namespace Plat4m

#endif // PLAT4M_GPIO_PORT_STM32H7XX_H
