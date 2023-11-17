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
// Copyright (c) 2021-2023 Benjamin Minerd
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

    enum OutputType
    {
        OUTPUT_TYPE_PUSH_PULL   = 0x00000000 << 8,
        OUTPUT_TYPE_OPEN_DRAIN  = 0x00000006 << 8
    };

    enum SubsystemSelect
    {
        SUBSYSTEM_SELECT_APPLICATION_MCU = 0,
        SUBSYSTEM_SELECT_NETWORK_MCU,
        SUBSYSTEM_SELECT_PERIPHERAL,
        SUBSYSTEM_SELECT_TRACE_AND_DEBUG
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
    // Public virtual methods overridden for GpioPin
    //--------------------------------------------------------------------------

    virtual void setLevelFast(const Level level) override;

    virtual Level getLevelFast() override;

    virtual Level readLevelFast() override;

    virtual void toggleLevelFast() override;

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    GpioPortNRF5340& getGpioPort();

    Id getId() const;

    void setNRF5340Config(NRF5340Config& config);

    void setOutputType(const OutputType outputType);

    void setSubsystemSelect(const SubsystemSelect subsystemSelect);

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    static const std::uint32_t myModeMap[];

    static const std::uint32_t myResistorMap[];

    static const std::uint32_t mySubsystemSelectMap[];

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    GpioPortNRF5340& myGpioPort;

    const Id myId;

    const std::uint32_t myPinBitMask;

    NRF5340Config myNRF5340Config;

    //--------------------------------------------------------------------------
    // Private virtual methods overridden for Module
    //--------------------------------------------------------------------------

    virtual Module::Error driverSetEnabled(const bool enable) override;

    //--------------------------------------------------------------------------
    // Private virtual methods overridden for GpioPin
    //--------------------------------------------------------------------------

    virtual GpioPin::Error driverConfigure(const Config& config) override;

    virtual GpioPin::Error driverSetLevel(const Level level) override;

    virtual GpioPin::Error driverGetLevel(Level& level) override;

    virtual GpioPin::Error driverReadLevel(Level& level) override;

    virtual GpioPin::Error driverToggleLevel() override;
};

}; // namespace Plat4m

#endif // PLAT4M_GPIO_PIN_NRF5340_H
