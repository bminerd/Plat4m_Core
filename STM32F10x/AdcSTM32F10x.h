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
/// @file AdcSTM32F10x.h
/// @author Ben Minerd
/// @date 4/10/2013
/// @brief AdcSTM32F10x class header file.
///

#ifndef PLAT4M_ADC_STM32F10X_H
#define PLAT4M_ADC_STM32F10X_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m.h>
#include <Module.h>
#include <ErrorTemplate.h>
#include <Array.h>
#include <InterruptSTM32F10x.h>
#include <DmaChannelSTM32F10x.h>

#include <stm32f10x.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class AdcSTM32F10x : public Module
{
public:
    
    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------
    
    enum Id
    {
        ID_1 = 0,
        ID_2,
        ID_3
    };
    
    enum ChannelId
    {
        CHANNEL_ID_0 = 0,
        CHANNEL_ID_1,
        CHANNEL_ID_2,
        CHANNEL_ID_3,
        CHANNEL_ID_4,
        CHANNEL_ID_5,
        CHANNEL_ID_6,
        CHANNEL_ID_7,
        CHANNEL_ID_8,
        CHANNEL_ID_9,
        CHANNEL_ID_10,
        CHANNEL_ID_11,
        CHANNEL_ID_12,
        CHANNEL_ID_13,
        CHANNEL_ID_14,
        CHANNEL_ID_15
    };

    enum Alignment
    {
        ALIGNMENT_RIGHT = 0,
        ALIGNMENT_LEFT
    };

    enum ConversionType
    {
        CONVERSION_TYPE_REGULAR = 0,
        CONVERSION_TYPE_INJECTED
    };

    enum RegularExternalTrigger
    {
        REGULAR_EXTERNAL_TRIGGER_0                          = 0,
        REGULAR_EXTERNAL_TRIGGER_ADC_1_2_TIM_1_CC_1         = REGULAR_EXTERNAL_TRIGGER_0,
        REGULAR_EXTERNAL_TRIGGER_ADC_3_TIM_3_CC_1           = REGULAR_EXTERNAL_TRIGGER_0,
        REGULAR_EXTERNAL_TRIGGER_1                          = 1,
        REGULAR_EXTERNAL_TRIGGER_ADC_1_2_TIM_1_CC_2         = REGULAR_EXTERNAL_TRIGGER_1,
        REGULAR_EXTERNAL_TRIGGER_ADC_3_TIM_2_CC_3           = REGULAR_EXTERNAL_TRIGGER_1,
        REGULAR_EXTERNAL_TRIGGER_2                          = 2,
        REGULAR_EXTERNAL_TRIGGER_ADC_1_2_TIM_1_CC_3         = REGULAR_EXTERNAL_TRIGGER_2,
        REGULAR_EXTERNAL_TRIGGER_ADC_3_TIM_1_CC_3           = REGULAR_EXTERNAL_TRIGGER_2,
        REGULAR_EXTERNAL_TRIGGER_3                          = 3,
        REGULAR_EXTERNAL_TRIGGER_ADC_1_2_TIM_2_CC_2         = REGULAR_EXTERNAL_TRIGGER_3,
        REGULAR_EXTERNAL_TRIGGER_ADC_3_TIM_8_CC_1           = REGULAR_EXTERNAL_TRIGGER_3,
        REGULAR_EXTERNAL_TRIGGER_4                          = 4,
        REGULAR_EXTERNAL_TRIGGER_ADC_1_2_TIM_3_TRGO         = REGULAR_EXTERNAL_TRIGGER_4,
        REGULAR_EXTERNAL_TRIGGER_ADC_3_TIM_8_TRGO           = REGULAR_EXTERNAL_TRIGGER_4,
        REGULAR_EXTERNAL_TRIGGER_5                          = 5,
        REGULAR_EXTERNAL_TRIGGER_ADC_1_2_TIM_4_CC_4         = REGULAR_EXTERNAL_TRIGGER_5,
        REGULAR_EXTERNAL_TRIGGER_ADC_3_TIM_5_CC_1           = REGULAR_EXTERNAL_TRIGGER_5,
        REGULAR_EXTERNAL_TRIGGER_6                          = 6,
        REGULAR_EXTERNAL_TRIGGER_ADC_1_2_EXTI_11_TIM_8_TRGO = REGULAR_EXTERNAL_TRIGGER_6,
        REGULAR_EXTERNAL_TRIGGER_ADC_3_TIM_5_CC_3           = REGULAR_EXTERNAL_TRIGGER_6,
        REGULAR_EXTERNAL_TRIGGER_7                          = 7,
        REGULAR_EXTERNAL_TRIGGER_SOFTWARE_START             = REGULAR_EXTERNAL_TRIGGER_7
    };

    enum InjectedExternalTrigger
    {
        INJECTED_EXTERNAL_TRIGGER_0                          = 0,
        INJECTED_EXTERNAL_TRIGGER_ADC_1_2_3_TIM_1_TRGO       = INJECTED_EXTERNAL_TRIGGER_0,
        INJECTED_EXTERNAL_TRIGGER_1                          = 1,
        INJECTED_EXTERNAL_TRIGGER_ADC_1_2_3_TIM_1_CC_4       = INJECTED_EXTERNAL_TRIGGER_1,
        INJECTED_EXTERNAL_TRIGGER_2                          = 2,
        INJECTED_EXTERNAL_TRIGGER_ADC_1_2_TIM_2_TRGO         = INJECTED_EXTERNAL_TRIGGER_2,
        INJECTED_EXTERNAL_TRIGGER_ADC_3_TIM_4_CC_3           = INJECTED_EXTERNAL_TRIGGER_2,
        INJECTED_EXTERNAL_TRIGGER_3                          = 3,
        INJECTED_EXTERNAL_TRIGGER_ADC_1_2_TIM_2_CC_1         = INJECTED_EXTERNAL_TRIGGER_3,
        INJECTED_EXTERNAL_TRIGGER_ADC_3_TIM_8_CC_2           = INJECTED_EXTERNAL_TRIGGER_3,
        INJECTED_EXTERNAL_TRIGGER_4                          = 4,
        INJECTED_EXTERNAL_TRIGGER_ADC_1_2_TIM_3_CC_4         = INJECTED_EXTERNAL_TRIGGER_4,
        INJECTED_EXTERNAL_TRIGGER_ADC_3_TIM_8_CC_4           = INJECTED_EXTERNAL_TRIGGER_4,
        INJECTED_EXTERNAL_TRIGGER_5                          = 5,
        INJECTED_EXTERNAL_TRIGGER_ADC_1_2_TIM_4_TRGO         = INJECTED_EXTERNAL_TRIGGER_5,
        INJECTED_EXTERNAL_TRIGGER_ADC_3_TIM_5_TRGO           = INJECTED_EXTERNAL_TRIGGER_5,
        INJECTED_EXTERNAL_TRIGGER_6                          = 6,
        INJECTED_EXTERNAL_TRIGGER_ADC_1_2_EXTI_15_TIM_8_CC_4 = INJECTED_EXTERNAL_TRIGGER_6,
        INJECTED_EXTERNAL_TRIGGER_ADC_3_TIM_5_CC_4           = INJECTED_EXTERNAL_TRIGGER_6,
        INJECTED_EXTERNAL_TRIGGER_7                          = 7,
        INJECTED_EXTERNAL_TRIGGER_SOFTWARE_START             = INJECTED_EXTERNAL_TRIGGER_7
    };

    enum InterruptEvent
    {
        INTERRUPT_EVENT_END_OF_REGULAR_CONVERSION = 0,
        INTERRUPT_EVENT_ANALOG_WATCHDOG,
        INTERRUPT_EVENT_END_OF_INJECTED_CONVERSION
    };

    enum SampleTime
    {
        SAMPLE_TIME_1_5_CLOCK_CYCLES = 0,
        SAMPLE_TIME_7_5_CLOCK_CYCLES,
        SAMPLE_TIME_13_5_CLOCK_CYCLES,
        SAMPLE_TIME_28_5_CLOCK_CYCLES,
        SAMPLE_TIME_41_5_CLOCK_CYCLES,
        SAMPLE_TIME_55_5_CLOCK_CYCLES,
        SAMPLE_TIME_71_5_CLOCK_CYCLES,
        SAMPLE_TIME_239_5_CLOCK_CYCLES
    };

    enum DualMode
    {
        DUAL_MODE_INDEPENDENT = 0,
        DUAL_MODE_COMBINED_REGULAR_SIMULTANEOUS_AND_INJECTED_SIMULTANEOUS,
        DUAL_MODE_COMBINED_REGULAR_SIMULTANEOUS_AND_ALTERNATE_TRIGGER,
        DUAL_MODE_COMBINED_INJECTED_SIMULTANEOUS_AND_FAST_INTERLEAVED,
        DUAL_MODE_COMBINED_INJECTED_SIMULTANEOUS_AND_SLOW_INTERLEAVED,
        DUAL_MODE_INJECTED_SIMULTANEOUS_ONLY,
        DUAL_MODE_REGULAR_SIMULTANEOUS_ONLY,
        DUAL_MODE_FAST_INTERLEAVED_ONLY,
        DUAL_MODE_SLOW_INTERLEAVED_ONLY,
        DUAL_MODE_ALTERNATE_TRIGGER_ONLY
    };

    typedef ErrorTemplate<ErrorCode> Error;

    typedef Callback<> EndOfRegularConversionInterruptCallback;

    typedef Callback<> EndOfInjectedConversionInterruptCallback;

    typedef Callback<> AnalogWatchdogInterruptCallback;

    struct Config
    {
        Alignment alignment;
        bool scanModeEnabled;
        bool dmaEnabled;
    };

    struct RegularSequenceConfig
    {
        Array<ChannelId> sequenceChannels;
        bool externalTriggerEnabled;
        RegularExternalTrigger externalTrigger;
        bool discontinuousModeEnabled;
        uint32_t discontinuousSequenceLength;
    };

    struct InjectedSequenceConfig
    {
        Array<ChannelId> sequenceChannels;
        bool externalTriggerEnabled;
        InjectedExternalTrigger externalTrigger;
        bool discontinuousModeEnabled;
        uint32_t discontinuousSequenceLength;
    };

    struct ChannelConfig
    {
        SampleTime sampleTime;
    };

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    AdcSTM32F10x(const Id id);

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~AdcSTM32F10x();
    
    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    ADC_TypeDef* getAdc();

    void setConfig(const Config& config);

    Config getConfig() const;

    void setRegularSequenceConfig(const RegularSequenceConfig& config);

    void setInjectedSequenceConfig(const InjectedSequenceConfig& cofig);

    void setChannelConfig(const ChannelId channelId,
                          const ChannelConfig& channelConfig);

    ChannelConfig getChannelConfig(const ChannelId channelId) const;

    void setDmaChannel(DmaChannelSTM32F10x& dmaChannel);

    Error readChannelVoltageV(const ChannelId channelId, VoltageV& voltageV);

    VoltageV readChannelVoltageVFast(const ChannelId channelId);

    Error readAllVoltages();
    
    void setEndOfRegularConversionInterruptCallback(
                             EndOfRegularConversionInterruptCallback& callback);

    void setEndOfInjectedConversionInterruptCallback(
                            EndOfInjectedConversionInterruptCallback& callback);

    void setAnalogWatchdogInterruptCallback(
                                     AnalogWatchdogInterruptCallback& callback);

    void setDualMode(const DualMode dualMode);

private:

    //--------------------------------------------------------------------------
    // Private types
    //--------------------------------------------------------------------------

    struct InjectedConversion
    {
        ChannelId channelId;
    };
    
    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    // Constants

    static const ProcessorSTM32F10x::Peripheral myPeripheralMap[];

    static const InterruptSTM32F10x::Id myInterruptIdMap[];

    // Variables

    static ADC_TypeDef* myAdcMap[];

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------
    
    const Id myId;
    
    ADC_TypeDef* myAdc;
    
    InterruptSTM32F10x myInterrupt;

    DmaChannelSTM32F10x* myDmaChannel;

    Config myConfig;

    RegularSequenceConfig myRegularSequenceConfig;

    InjectedSequenceConfig myInjectedSequenceConfig;

    ChannelConfig myChannelConfigs[16];

    VoltageV myLevelsToVolts;

    uint16_t myRegularValues[16];

    uint8_t myRegularSequenceLength;

    uint8_t myCurrentRegularValueIndex;

    uint8_t myRegularChannelToValueMap[16];

    uint8_t myInjectedChannelToValueMap[16];
    
    EndOfRegularConversionInterruptCallback*
                                      myEndOfRegularConversionInterruptCallback;

    EndOfInjectedConversionInterruptCallback*
                                     myEndOfInjectedConversionInterruptCallback;

    AnalogWatchdogInterruptCallback* myAnalogWatchdogInterruptCallback;

    ConversionType myLastConversionType;

    //--------------------------------------------------------------------------
    // Private methods implemented from Module
    //--------------------------------------------------------------------------

    Module::Error driverSetEnabled(const bool enabled);
    
    //--------------------------------------------------------------------------
    // Private inline methods
    //--------------------------------------------------------------------------

    inline void privateSetEnabled(const bool enabled);

    inline void setAlignment(const Alignment alignment);

    inline void setScanModeEnabled(const bool enabled);

    inline void setRegularSequenceLength(const uint32_t sequenceLength);

    inline void setRegularSequenceConversionChannel(const uint32_t conversion,
                                                    const ChannelId channelId);

    inline void setInjectedSequenceLength(const uint32_t sequenceLength);

    inline void setInjectedSequenceConversionChannel(const uint32_t conversion,
                                                     const ChannelId channelId);

    inline void setRegularExternalTriggerEnabled(const bool enabled);

    inline void setRegularExternalTrigger(
                           const RegularExternalTrigger regularExternalTrigger);

    inline void setInjectedExternalTriggerEnabled(const bool enabled);

    inline void setInjectedExternalTrigger(
                         const InjectedExternalTrigger injectedExternalTrigger);

    inline void setDmaEnabled(const bool enabled);

    inline void setInterruptEventEnabled(const InterruptEvent interruptEvent,
                                         const bool enabled);

    inline bool isInterruptEventEnabled(const InterruptEvent interruptEvent);

    inline bool isInterruptEventPending(const InterruptEvent interruptEvent);

    inline void setChannelSampleTime(const ChannelId channelId,
                                     const SampleTime sampleTime);

    inline void startRegularConversion();

    inline bool isConversionComplete();

    inline uint16_t getRegularData();

    inline uint16_t getInjectedData(const uint32_t conversionIndex);

    inline void setInjectedDiscontinuousModeEnabled(const bool enabled);

    inline void endOfRegularConversionInterruptEventHandler();

    inline void endOfInjectedConversionInterruptEventHandler();

    inline void analogWatchdogInterruptEventHandler();

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------

    void interruptHandler();
};

}; // namespace Plat4m

#endif // PLAT4M_ADC_STM32F10X_H
