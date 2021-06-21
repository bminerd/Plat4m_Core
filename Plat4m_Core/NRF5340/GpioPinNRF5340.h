/**
 * Copyright (c) 2021, SignalQuest LLC
 */

///
/// @file GpioPinNRF5340.h
/// @author Ben Minerd
/// @date 1/6/2021
/// @brief GpioPinNRF5340 class header file.
///

#ifndef PLAT4M_GPIO_PIN_NRF5340_H
#define PLAT4M_GPIO_PIN_NRF5340_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/GpioPin.h>
#include <Plat4m_Core/NRF5340/GpioPortNRF5340.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class GpioPinNRF5340 : public GpioPin
{
public:
    
    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------

    enum Id
    {
        ID_0 = 0,
        ID_1,
        ID_2,
        ID_3,
        ID_4,
        ID_5,
        ID_6,
        ID_7,
        ID_8,
        ID_9,
        ID_10,
        ID_11,
        ID_12,
        ID_13,
        ID_14,
        ID_15,
        ID_16,
        ID_17,
        ID_18,
        ID_19,
        ID_20,
        ID_21,
        ID_22,
        ID_23,
        ID_24,
        ID_25,
        ID_26,
        ID_27,
        ID_28,
        ID_29,
        ID_30,
        ID_31
    };

    enum AlternateFunction
    {
        ALTERNATE_FUNCTION_0 = 0,
        ALTERNATE_FUNCTION_1,
        ALTERNATE_FUNCTION_2,
        ALTERNATE_FUNCTION_3,
        ALTERNATE_FUNCTION_4,
        ALTERNATE_FUNCTION_5,
        ALTERNATE_FUNCTION_6,
        ALTERNATE_FUNCTION_7,
        ALTERNATE_FUNCTION_8,
        ALTERNATE_FUNCTION_9,
        ALTERNATE_FUNCTION_10,
        ALTERNATE_FUNCTION_11,
        ALTERNATE_FUNCTION_12,
        ALTERNATE_FUNCTION_13,
        ALTERNATE_FUNCTION_15,
    };

    enum OutputType
    {
        OUTPUT_TYPE_PUSH_PULL   = 0x00000000 << 8,
        OUTPUT_TYPE_OPEN_DRAIN  = 0x00000006 << 8
    };

    struct NRF5340Config
    {
        OutputType outputType;
    };

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    GpioPinNRF5340(GpioPortNRF5340& gpioPort, const Id id);

    //--------------------------------------------------------------------------
    // Public methods implemented from GpioPin
    //--------------------------------------------------------------------------

    void setLevelFast(const Level level);

    Level getLevelFast();

    Level readLevelFast();

    void toggleLevelFast();
    
    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------
    
    GpioPortNRF5340& getGpioPort();
    
    Id getId() const;
    
    void setNRF5340Config(NRF5340Config& config);
    
    void setOutputType(const OutputType outputType);

private:
    
    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    static const uint32_t myModeMap[];

    static const uint32_t myResistorMap[];

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------
    
    GpioPortNRF5340& myGpioPort;
    
    const Id myId;
    
    const uint32_t myPinBitMask;

    NRF5340Config myNRF5340Config;

    //--------------------------------------------------------------------------
    // Private methods implemented from Module
    //--------------------------------------------------------------------------
    
    Module::Error driverSetEnabled(const bool enable);

    //--------------------------------------------------------------------------
    // Private methods implemented from GpioPin
    //--------------------------------------------------------------------------
    
    GpioPin::Error driverConfigure(const Config& config);
    
    GpioPin::Error driverSetLevel(const Level level);
    
    GpioPin::Error driverGetLevel(Level& level);
    
    GpioPin::Error driverReadLevel(Level& level);
    
    GpioPin::Error driverToggleLevel();
};

}; // namespace Plat4m

#endif // PLAT4M_GPIO_PIN_NRF5340_H
