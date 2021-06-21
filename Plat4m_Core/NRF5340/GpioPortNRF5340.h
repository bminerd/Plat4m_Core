/**
 * Copyright (c) 2021, SignalQuest LLC
 */

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

class GpioPortNRF5340 : public GpioPort<uint32_t>
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

    virtual void setValueFast(const uint32_t value);

    virtual uint32_t getValueFast();

    virtual uint32_t readValueFast();

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    Id getId();

    NRF_GPIO_Type*& getPort();

    uint8_t getNPins() const;

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    // Constants

    static const uint32_t myBitMaskMap[];

    static const uint8_t myNPinsMap[];

    static const uint32_t myModeMap[];

    static const uint32_t myResistorMap[];

    // Variables

    static NRF_GPIO_Type* myPortMap[];

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------
    
    const Id myId;

    const uint32_t myBitMask;

    const uint8_t myNPins;
    
    NRF_GPIO_Type* myPort;

    //--------------------------------------------------------------------------
    // Private virtual methods implemented from Module
    //--------------------------------------------------------------------------
    
    virtual Module::Error driverSetEnabled(const bool enabled);
    
    //--------------------------------------------------------------------------
    // Private virtual methods implemented from GpioPort
    //--------------------------------------------------------------------------
    
    virtual Error driverConfigure(const Config& config);
    
    virtual Error driverSetValue(const uint32_t value);
    
    virtual Error driverGetValue(uint32_t& value);
    
    virtual Error driverReadValue(uint32_t& value);
};

}; // namespace Plat4m

#endif // PLAT4M_GPIO_PORT_NRF5340_H
