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
// Copyright (c) 2022 Benjamin Minerd
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
/// @file GpioPortNRF5340.h
/// @author Ben Minerd
/// @date 1/6/2021
/// @brief GpioPortNRF5340 class header file.
///

#ifndef PLAT4M_GPIO_PORT_NRF5340_H
#define PLAT4M_GPIO_PORT_NRF5340_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>

#include <nrf5340_application.h>
#include <nrf5340_application_peripherals.h>
#include <nrf5340_application_bitfields.h>

#include <Plat4m_Core/GpioPort.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class GpioPortNRF5340 : public GpioPort<std::uint32_t>
{
public:
    
    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------

    enum Id
    {
        ID_P0 = 0,
        ID_P1
    };

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------
    
    GpioPortNRF5340(const Id id);
    
    //--------------------------------------------------------------------------
    // Public methods implemented from GpioPort
    //--------------------------------------------------------------------------

    virtual void setValueFast(const std::uint32_t value);

    virtual std::uint32_t getValueFast();

    virtual std::uint32_t readValueFast();

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    Id getId();

    NRF_GPIO_Type*& getPort();

    std::uint8_t getNPins() const;

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    // Constants

    static const std::uint32_t myBitMaskMap[];

    static const std::uint8_t myNPinsMap[];

    static const std::uint32_t myModeMap[];

    static const std::uint32_t myResistorMap[];

    // Variables

    static NRF_GPIO_Type* myPortMap[];

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------
    
    const Id myId;

    const std::uint32_t myBitMask;

    const std::uint8_t myNPins;
    
    NRF_GPIO_Type* myPort;

    //--------------------------------------------------------------------------
    // Private virtual methods implemented from Module
    //--------------------------------------------------------------------------
    
    virtual Module::Error driverSetEnabled(const bool enabled);
    
    //--------------------------------------------------------------------------
    // Private virtual methods implemented from GpioPort
    //--------------------------------------------------------------------------
    
    virtual Error driverConfigure(const Config& config);
    
    virtual Error driverSetValue(const std::uint32_t value);
    
    virtual Error driverGetValue(std::uint32_t& value);
    
    virtual Error driverReadValue(std::uint32_t& value);
};

}; // namespace Plat4m

#endif // PLAT4M_GPIO_PORT_NRF5340_H
