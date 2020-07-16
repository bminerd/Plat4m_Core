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
// Copyright (c) 2016 Benjamin Minerd
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
/// @file AdcSTM32F30x.h
/// @author Ben Minerd
/// @date 7/12/2016
/// @brief AdcSTM32F30x class header file.
///

#ifndef PLAT4M_ADC_STM32F30X_H
#define PLAT4M_ADC_STM32F30X_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <stm32f30x.h>

#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/Module.h>
#include <Plat4m_Core/ErrorTemplate.h>
#include <Plat4m_Core/Array.h>
#include <Plat4m_Core/STM32F30x/InterruptSTM32F30x.h>
#include <Plat4m_Core/STM32F30x/DmaChannelSTM32F30x.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class AdcSTM32F30x : public Module
{
public:
    
    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------
    
    enum ErrorCode
    {
        ERROR_CODE_NONE = 0
    };

    enum Id
    {
        ID_1 = 0,
        ID_2,
        ID_3,
        ID_4
    };
    
    enum ChannelId
    {
        CHANNEL_ID_1 = 0,
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
        CHANNEL_ID_15,
        CHANNEL_ID_16,
        CHANNEL_ID_17,
        CHANNEL_ID_18
    };
    
    enum CommonId
	{
    	COMMON_ID_1_2 = 0,
		COMMON_ID_3_4
	};

    enum Resolution
    {
        RESOLUTION_6_BIT = 0,
        RESOLUTION_8_BIT,
        RESOLUTION_10_BIT,
        RESOLUTION_12_BIT
    };

    enum Alignment
    {
        ALIGNMENT_RIGHT = 0,
        ALIGNMENT_LEFT  = ADC_CFGR_ALIGN
    };

    enum ConversionType
    {
        CONVERSION_TYPE_REGULAR = 0,
        CONVERSION_TYPE_INJECTED
    };

    enum ExternalTriggerMode
    {
        EXTERNAL_TRIGGER_MODE_DISABLED = 0,
        EXTERNAL_TRIGGER_MODE_RISING_EDGE,
        EXTERNAL_TRIGGER_MODE_FALLING_EDGE,
        EXTERNAL_TRIGGER_MODE_RISING_FALLING_EDGE
    };

    enum RegularExternalTrigger
    {
        REGULAR_EXTERNAL_TRIGGER_0                                = 0,
        REGULAR_EXTERNAL_TRIGGER_ADC_1_2_TIM_1_CC_1               = REGULAR_EXTERNAL_TRIGGER_0,
        REGULAR_EXTERNAL_TRIGGER_ADC_3_4_TIM_3_CC_1               = REGULAR_EXTERNAL_TRIGGER_0,
        REGULAR_EXTERNAL_TRIGGER_1                                = 1,
        REGULAR_EXTERNAL_TRIGGER_ADC_1_2_TIM_1_CC_2               = REGULAR_EXTERNAL_TRIGGER_1,
        REGULAR_EXTERNAL_TRIGGER_ADC_3_4_TIM_2_CC_3               = REGULAR_EXTERNAL_TRIGGER_1,
        REGULAR_EXTERNAL_TRIGGER_2                                = 2,
        REGULAR_EXTERNAL_TRIGGER_ADC_1_2_TIM_1_CC_3_TIM_20_TRGO   = REGULAR_EXTERNAL_TRIGGER_2,
        REGULAR_EXTERNAL_TRIGGER_ADC_3_4_TIM_1_CC_3               = REGULAR_EXTERNAL_TRIGGER_2,
        REGULAR_EXTERNAL_TRIGGER_3                                = 3,
        REGULAR_EXTERNAL_TRIGGER_ADC_1_2_TIM_2_CC_2_TIM_20_TRGO_2 = REGULAR_EXTERNAL_TRIGGER_3,
        REGULAR_EXTERNAL_TRIGGER_ADC_3_4_TIM_8_CC_1               = REGULAR_EXTERNAL_TRIGGER_3,
        REGULAR_EXTERNAL_TRIGGER_4                                = 4,
        REGULAR_EXTERNAL_TRIGGER_ADC_1_2_TIM_3_TRGO               = REGULAR_EXTERNAL_TRIGGER_4,
        REGULAR_EXTERNAL_TRIGGER_ADC_3_4_TIM_8_TRGO               = REGULAR_EXTERNAL_TRIGGER_4,
        REGULAR_EXTERNAL_TRIGGER_5                                = 5,
        REGULAR_EXTERNAL_TRIGGER_ADC_1_2_TIM_4_CC_4_TIM_20_CC_1   = REGULAR_EXTERNAL_TRIGGER_5,
        REGULAR_EXTERNAL_TRIGGER_ADC_3_4_EXTI_2_TIM_20_TRGO       = REGULAR_EXTERNAL_TRIGGER_5,
        REGULAR_EXTERNAL_TRIGGER_6                                = 6,
        REGULAR_EXTERNAL_TRIGGER_ADC_1_2_EXTI_11                  = REGULAR_EXTERNAL_TRIGGER_6,
        REGULAR_EXTERNAL_TRIGGER_ADC_3_4_TIM_4_CC_1_TIM_20_TRGO_2 = REGULAR_EXTERNAL_TRIGGER_6,
        REGULAR_EXTERNAL_TRIGGER_7                                = 7,
        REGULAR_EXTERNAL_TRIGGER_ADC_1_2_TIM_8_TRGO               = REGULAR_EXTERNAL_TRIGGER_7,
        REGULAR_EXTERNAL_TRIGGER_ADC_3_4_TIM_2_TRGO               = REGULAR_EXTERNAL_TRIGGER_7,
        REGULAR_EXTERNAL_TRIGGER_8                                = 8,
        REGULAR_EXTERNAL_TRIGGER_ADC_1_2_TIM_8_TRGO_2             = REGULAR_EXTERNAL_TRIGGER_8,
        REGULAR_EXTERNAL_TRIGGER_ADC_3_4_TIM_8_TRGO_2             = REGULAR_EXTERNAL_TRIGGER_8,
        REGULAR_EXTERNAL_TRIGGER_9                                = 9,
        REGULAR_EXTERNAL_TRIGGER_ADC_1_2_TIM_1_TRGO               = REGULAR_EXTERNAL_TRIGGER_9,
        REGULAR_EXTERNAL_TRIGGER_ADC_3_4_TIM_1_TRGO               = REGULAR_EXTERNAL_TRIGGER_9,
        REGULAR_EXTERNAL_TRIGGER_10                               = 10,
        REGULAR_EXTERNAL_TRIGGER_ADC_1_2_TIM_1_TRGO_2             = REGULAR_EXTERNAL_TRIGGER_10,
        REGULAR_EXTERNAL_TRIGGER_ADC_3_4_TIM_1_TRGO_2             = REGULAR_EXTERNAL_TRIGGER_10,
        REGULAR_EXTERNAL_TRIGGER_11                               = 11,
        REGULAR_EXTERNAL_TRIGGER_ADC_1_2_TIM_2_TRGO               = REGULAR_EXTERNAL_TRIGGER_11,
        REGULAR_EXTERNAL_TRIGGER_ADC_3_4_TIM_3_TRGO               = REGULAR_EXTERNAL_TRIGGER_11,
        REGULAR_EXTERNAL_TRIGGER_12                               = 12,
        REGULAR_EXTERNAL_TRIGGER_ADC_1_2_TIM_4_TRGO               = REGULAR_EXTERNAL_TRIGGER_12,
        REGULAR_EXTERNAL_TRIGGER_ADC_3_4_TIM_4_TRGO               = REGULAR_EXTERNAL_TRIGGER_12,
        REGULAR_EXTERNAL_TRIGGER_13                               = 13,
        REGULAR_EXTERNAL_TRIGGER_ADC_1_2_TIM_6_TRGO_TIM_20_CC_2   = REGULAR_EXTERNAL_TRIGGER_13,
        REGULAR_EXTERNAL_TRIGGER_ADC_3_4_TIM_7_TRGO               = REGULAR_EXTERNAL_TRIGGER_13,
        REGULAR_EXTERNAL_TRIGGER_14                               = 14,
        REGULAR_EXTERNAL_TRIGGER_ADC_1_2_TIM_15_TRGO              = REGULAR_EXTERNAL_TRIGGER_14,
        REGULAR_EXTERNAL_TRIGGER_ADC_3_4_TIM_15_TRGO              = REGULAR_EXTERNAL_TRIGGER_14,
        REGULAR_EXTERNAL_TRIGGER_15                               = 15,
        REGULAR_EXTERNAL_TRIGGER_ADC_1_2_TIM_3_CC_4_TIM_20_CC_3   = REGULAR_EXTERNAL_TRIGGER_15,
        REGULAR_EXTERNAL_TRIGGER_ADC_3_4_TIM_2_CC_1_TIM_20_CC_1   = REGULAR_EXTERNAL_TRIGGER_15
    };

    enum InjectedExternalTrigger
    {
        INJECTED_EXTERNAL_TRIGGER_0 = 0,
        INJECTED_EXTERNAL_TRIGGER_ADC_1_2_TIM_1_TRGO = INJECTED_EXTERNAL_TRIGGER_0,
        INJECTED_EXTERNAL_TRIGGER_ADC_3_4_TIM_1_TRGO = INJECTED_EXTERNAL_TRIGGER_0,
        INJECTED_EXTERNAL_TRIGGER_1 = 1,
        INJECTED_EXTERNAL_TRIGGER_ADC_1_2_TIM_1_CC_4 = INJECTED_EXTERNAL_TRIGGER_1,
        INJECTED_EXTERNAL_TRIGGER_ADC_3_4_TIM_1_CC_4 = INJECTED_EXTERNAL_TRIGGER_1,
        INJECTED_EXTERNAL_TRIGGER_2 = 2,
        INJECTED_EXTERNAL_TRIGGER_ADC_1_2_TIM_2_TRGO = INJECTED_EXTERNAL_TRIGGER_2,
        INJECTED_EXTERNAL_TRIGGER_ADC_3_4_TIM_4_CC_3 = INJECTED_EXTERNAL_TRIGGER_2,
        INJECTED_EXTERNAL_TRIGGER_3 = 3,
        INJECTED_EXTERNAL_TRIGGER_ADC_1_2_TIM_2_CC_1_TIM_20_TRGO = INJECTED_EXTERNAL_TRIGGER_3,
        INJECTED_EXTERNAL_TRIGGER_ADC_3_4_TIM_8_CC_2 = INJECTED_EXTERNAL_TRIGGER_3,
        INJECTED_EXTERNAL_TRIGGER_4 = 4,
        INJECTED_EXTERNAL_TRIGGER_ADC_1_2_TIM_3_CC_4 = INJECTED_EXTERNAL_TRIGGER_4,
        INJECTED_EXTERNAL_TRIGGER_ADC_3_4_TIM_8_CC_4 = INJECTED_EXTERNAL_TRIGGER_4,
        INJECTED_EXTERNAL_TRIGGER_5 = 5,
        INJECTED_EXTERNAL_TRIGGER_ADC_1_2_TIM_4_TRGO = INJECTED_EXTERNAL_TRIGGER_5,
        INJECTED_EXTERNAL_TRIGGER_ADC_3_4_TIM_4_CC_3_TIM_20_TRGO = INJECTED_EXTERNAL_TRIGGER_5,
        INJECTED_EXTERNAL_TRIGGER_6 = 6,
        INJECTED_EXTERNAL_TRIGGER_ADC_1_2_EXTI_15_TIM_29_TRGO_2 = INJECTED_EXTERNAL_TRIGGER_6,
        INJECTED_EXTERNAL_TRIGGER_ADC_3_4_TIM_4_CC_4 = INJECTED_EXTERNAL_TRIGGER_6,
        INJECTED_EXTERNAL_TRIGGER_7 = 7,
        INJECTED_EXTERNAL_TRIGGER_ADC_1_2_TIM_8_CC_4 = INJECTED_EXTERNAL_TRIGGER_7,
        INJECTED_EXTERNAL_TRIGGER_ADC_3_4_TIM_4_TRGO = INJECTED_EXTERNAL_TRIGGER_7,
        INJECTED_EXTERNAL_TRIGGER_8 = 8,
        INJECTED_EXTERNAL_TRIGGER_ADC_1_2_TIM_1_TRGO_2 = INJECTED_EXTERNAL_TRIGGER_8,
        INJECTED_EXTERNAL_TRIGGER_ADC_3_4_TIM_1_TRGO_2 = INJECTED_EXTERNAL_TRIGGER_8,
        INJECTED_EXTERNAL_TRIGGER_9 = 9,
        INJECTED_EXTERNAL_TRIGGER_ADC_1_2_TIM_8_TRGO = INJECTED_EXTERNAL_TRIGGER_9,
        INJECTED_EXTERNAL_TRIGGER_ADC_3_4_TIM_8_TRGO = INJECTED_EXTERNAL_TRIGGER_9,
        INJECTED_EXTERNAL_TRIGGER_10 = 10,
        INJECTED_EXTERNAL_TRIGGER_ADC_1_2_TIM_8_TRGO_2 = INJECTED_EXTERNAL_TRIGGER_10,
        INJECTED_EXTERNAL_TRIGGER_ADC_3_4_TIM_8_TRGO_2 = INJECTED_EXTERNAL_TRIGGER_10,
        INJECTED_EXTERNAL_TRIGGER_11 = 11,
        INJECTED_EXTERNAL_TRIGGER_ADC_1_2_TIM_3_CC_3 = INJECTED_EXTERNAL_TRIGGER_11,
        INJECTED_EXTERNAL_TRIGGER_ADC_3_4_TIM_1_CC_3_TIM_20_TRGO_2 = INJECTED_EXTERNAL_TRIGGER_11,
        INJECTED_EXTERNAL_TRIGGER_12 = 12,
        INJECTED_EXTERNAL_TRIGGER_ADC_1_2_TIM_3_TRGO = INJECTED_EXTERNAL_TRIGGER_12,
        INJECTED_EXTERNAL_TRIGGER_ADC_3_4_TIM_3_TRGO = INJECTED_EXTERNAL_TRIGGER_12,
        INJECTED_EXTERNAL_TRIGGER_13 = 13,
        INJECTED_EXTERNAL_TRIGGER_ADC_1_2_TIM_3_CC_1_TIM_20_CC_4 = INJECTED_EXTERNAL_TRIGGER_13,
        INJECTED_EXTERNAL_TRIGGER_ADC_3_4_TIM_2_TRGO = INJECTED_EXTERNAL_TRIGGER_13,
        INJECTED_EXTERNAL_TRIGGER_14 = 14,
        INJECTED_EXTERNAL_TRIGGER_ADC_1_2_TIM_6_TRGO = INJECTED_EXTERNAL_TRIGGER_14,
        INJECTED_EXTERNAL_TRIGGER_ADC_3_4_TIM_7_TRGO = INJECTED_EXTERNAL_TRIGGER_14,
        INJECTED_EXTERNAL_TRIGGER_15 = 15,
        INJECTED_EXTERNAL_TRIGGER_ADC_1_2_TIM_15_TRGO = INJECTED_EXTERNAL_TRIGGER_15,
        INJECTED_EXTERNAL_TRIGGER_ADC_3_4_TIM_15_TRGO_TIM_20_CC_2 = INJECTED_EXTERNAL_TRIGGER_15
    };

    enum InterruptEvent
    {
        INTERRUPT_EVENT_ADC_READY = 0,
        INTERRUPT_EVENT_END_OF_SAMPLING,
        INTERRUPT_EVENT_END_OF_REGULAR_CONVERSION,
        INTERRUPT_EVENT_END_OF_REGULAR_SEQUENCE,
        INTERRUPT_EVENT_OVERRUN,
        INTERRUPT_EVENT_END_OF_INJECTED_CONVERSION,
        INTERRUPT_EVENT_END_OF_INJECTED_SEQUENCE,
        INTERRUPT_EVENT_ANALOG_WATCHDOG_1,
        INTERRUPT_EVENT_ANALOG_WATCHDOG_2,
        INTERRUPT_EVENT_ANALOG_WATCHDOG_3,
        INTERRUPT_EVENT_INJECTED_CONTEXT_QUEUE_OVERFLOW
    };

    enum DmaMode
    {
        DMA_MODE_ONE_SHOT = 0,
        DMA_MODE_CIRCULAR
    };

    enum SampleTime
    {
        SAMPLE_TIME_1_5_CLOCK_CYCLES = 0,
        SAMPLE_TIME_2_5_CLOCK_CYCLES,
        SAMPLE_TIME_4_5_CLOCK_CYCLES,
        SAMPLE_TIME_7_5_CLOCK_CYCLES,
        SAMPLE_TIME_19_5_CLOCK_CYCLES,
        SAMPLE_TIME_61_5_CLOCK_CYCLES,
        SAMPLE_TIME_181_5_CLOCK_CYCLES,
        SAMPLE_TIME_601_5_CLOCK_CYCLES
    };

    enum DualMode
    {
        DUAL_MODE_DISABLED = 0,
        DUAL_MODE_COMBINED_REGULAR_SIMULTANEOUS_AND_INJECTED_SIMULTANEOUS,
        DUAL_MODE_COMBINED_REGULAR_SIMULTANEOUS_AND_ALTERNATE_TRIGGER,
        DUAL_MODE_COMBINED_INTERLEAVED_AND_INJECTED_SIMULTANEOUS,
        DUAL_MODE_INJECTED_SIMULTANEOUS_ONLY,
        DUAL_MODE_REGULAR_SIMULTANEOUS_ONLY,
        DUAL_MODE_INTERLEAVED_ONLY,
        DUAL_MODE_ALTERNATE_TRIGGER_ONLY
    };

    enum ClockMode
	{
    	CLOCK_MODE_ASYNCHRONOUS_DIVIDE_BY_1 = 0,
		CLOCK_MODE_ASYNCHRONOUS_DIVIDE_BY_2,
		CLOCK_MODE_ASYNCHRONOUS_DIVIDE_BY_4,
		CLOCK_MODE_ASYNCHRONOUS_DIVIDE_BY_6,
		CLOCK_MODE_ASYNCHRONOUS_DIVIDE_BY_8,
		CLOCK_MODE_ASYNCHRONOUS_DIVIDE_BY_10,
		CLOCK_MODE_ASYNCHRONOUS_DIVIDE_BY_12,
		CLOCK_MODE_ASYNCHRONOUS_DIVIDE_BY_16,
		CLOCK_MODE_ASYNCHRONOUS_DIVIDE_BY_32,
		CLOCK_MODE_ASYNCHRONOUS_DIVIDE_BY_64,
		CLOCK_MODE_ASYNCHRONOUS_DIVIDE_BY_128,
		CLOCK_MODE_ASYNCHRONOUS_DIVIDE_BY_256,
		CLOCK_MODE_SYNCHRONOUS_HCLK_DIVIDE_BY_1,
		CLOCK_MODE_SYNCHRONOUS_HCLK_DIVIDE_BY_2,
		CLOCK_MODE_SYNCHRONOUS_HCLK_DIVIDE_BY_4,
	};

    typedef ErrorTemplate<ErrorCode> Error;

    typedef Callback<> EndOfRegularConversionInterruptCallback;

    typedef Callback<> EndOfRegularSequenceInterruptCallback;

    typedef Callback<> EndOfInjectedConversionInterruptCallback;

    typedef Callback<> EndOfInjectedSequenceInterruptCallback;

    typedef Callback<> AnalogWatchdogInterruptCallback;

    struct CommonConfig
    {
    	ClockMode clockMode;
    	DualMode dualMode;
    	bool vBatEnabled;
    	bool temperatureSensorEnabled;
    	bool vRefintEnabled;
    };

    struct Config
    {
        Resolution resolution;
        Alignment alignment;
        bool dmaEnabled;
        DmaMode dmaMode;
    };

    struct RegularSequenceConfig
    {
        Array<ChannelId> sequenceChannels;
        ExternalTriggerMode externalTriggerMode;
        RegularExternalTrigger externalTrigger;
        bool discontinuousModeEnabled;
        uint32_t discontinuousSequenceLength;
    };

    struct InjectedSequenceConfig
    {
        Array<ChannelId> sequenceChannels;
        ExternalTriggerMode externalTriggerMode;
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

    AdcSTM32F30x(const Id id);

    AdcSTM32F30x(const Id id, AdcSTM32F30x& slaveAdc);

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------
    
    virtual ~AdcSTM32F30x();

    //--------------------------------------------------------------------------
    // Public static methods
    //--------------------------------------------------------------------------

    static void setCommonConfig(const CommonId id, const CommonConfig config);
    
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

    void setDmaChannel(DmaChannelSTM32F30x& dmaChannel);

    Error readChannelVoltageV(const ChannelId channelId, VoltageV& voltageV);

    VoltageV readChannelVoltageVFast(const ChannelId channelId);

    Error readAllVoltages();

    void setEndOfRegularConversionInterruptCallback(
                             EndOfRegularConversionInterruptCallback& callback);

    void setEndOfRegularSequenceInterruptCallback(
                               EndOfRegularSequenceInterruptCallback& callback);

    void setEndOfInjectedConversionInterruptCallback(
                            const uint8_t conversionIndex,
                            EndOfInjectedConversionInterruptCallback& callback);

    void setEndOfInjectedSequenceInterruptCallback(
                              EndOfInjectedSequenceInterruptCallback& callback);

    void setAnalogWatchdogInterruptCallback(
                                     AnalogWatchdogInterruptCallback& callback);

    void setInjectedExternalTrigger(
                         const InjectedExternalTrigger injectedExternalTrigger);

    bool setChannelOffsetValue(const ChannelId channelId,
                               const uint16_t offsetValue);

    bool setChannelOffsetVoltage(const ChannelId channelId,
                                 const VoltageV offsetVoltageV);

    void startChannelOffsetCalibration(const ChannelId channelId,
                                       const uint32_t nSamples);

    bool isChannelOffsetCalibrationComplete(const ChannelId channelId);

    void startRegularConversion();

    bool isRegularConversionStarted();

    void stopRegularConversion();

    bool isRegularConversionStopComplete();

    void startInjectedConversion();

    bool isInjectedConversionStarted();

    void stopInjectedConversion();

    bool isInjectedConversionStopComplete();

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    // Constants

    static const ProcessorSTM32F30x::Peripheral myPeripheralMap[];
    
    static const InterruptSTM32F30x::Id myInterruptIdMap[];

    static const uint32_t myResolutionMap[];

    static const float myResolutionMaxValueMap[];

    // Variables

    static ADC_TypeDef* myAdcMap[];

    static CommonConfig myAdc1And2CommonConfig;
    static CommonConfig myAdc3And4CommonConfig;

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------
    
    const Id myId;
    
    ADC_TypeDef* myAdc;

    InterruptSTM32F30x myInterrupt;

    DmaChannelSTM32F30x* myDmaChannel;

    Config myConfig;

    RegularSequenceConfig myRegularSequenceConfig;

    InjectedSequenceConfig myInjectedSequenceConfig;

    ChannelConfig myChannelConfigs[18];

    float myLevelsToVolts;

    volatile int16_t myRegularValues[18];

    uint8_t myRegularSequenceLength;

    volatile uint8_t myCurrentValueIndex;

    uint8_t myRegularChannelToValueMap[18];

    uint8_t myInjectedChannelToValueMap[18];

    EndOfRegularConversionInterruptCallback*
                                      myEndOfRegularConversionInterruptCallback;

    EndOfRegularSequenceInterruptCallback*
                                        myEndOfRegularSequenceInterruptCallback;

    EndOfInjectedConversionInterruptCallback*
                                 myEndOfInjectedConversionInterruptCallbacks[4];

    EndOfInjectedSequenceInterruptCallback*
                                       myEndOfInjectedSequenceInterruptCallback;

    AnalogWatchdogInterruptCallback* myAnalogWatchdogInterruptCallback;

    uint8_t myInjectedSequenceLength;

    volatile uint8_t myCurrentInjectedConversionIndex;

    uint8_t myOffsetValueCount;

    volatile uint8_t myChannelToOffsetValueIndexMap[18];

    volatile bool myIsOffsetCalibrationRunningMap[4];

    volatile uint32_t myOffsetCalibrationTotalNSamplesMap[4];

    volatile uint32_t myOffsetCalibrationCurrentNSamplesMap[4];

    volatile uint64_t myOffsetValueSumMap[4];

    volatile int16_t myInjectedValueMap[4];

    volatile int32_t myValueSumMap[18];

    ConversionType myChannelConversionTypeMap[18];

    AdcSTM32F30x* myMasterAdc;

    AdcSTM32F30x* mySlaveAdc;

    //--------------------------------------------------------------------------
    // Private methods implemented from Module
    //--------------------------------------------------------------------------

    Module::Error driverSetEnabled(const bool enabled);

    //--------------------------------------------------------------------------
    // Private inline methods
    //--------------------------------------------------------------------------

    inline void privateSetEnabled(const bool enabled);

    inline void setResolution(const Resolution resolution);

    inline void setAlignment(const Alignment alignment);

    inline void setRegularSequenceLength(const uint32_t sequenceLength);

    inline void setRegularSequenceConversionChannel(const uint32_t conversion,
                                                    const ChannelId channelId);

    inline void setInjectedDiscontinuousModeEnabled(const bool enabled);

    inline void setInjectedSequenceLength(const uint32_t sequenceLength);

    inline uint32_t getInjectedSequenceLengthBits(
                                                 const uint32_t sequenceLength);

    inline void setInjectedSequenceConversionChannel(const uint32_t conversion,
                                                     const ChannelId channelId);

    inline uint32_t getInjectedSequenceConversionChannelBits(
                                                     const uint32_t conversion,
                                                     const ChannelId channelId);

    inline void setRegularExternalTriggerMode(
                                 const ExternalTriggerMode externalTriggerMode);

    inline void setRegularExternalTrigger(
                           const RegularExternalTrigger regularExternalTrigger);

    inline void setInjectedExternalTriggerMode(
                                 const ExternalTriggerMode externalTriggerMode);

    inline uint32_t getInjectedExternalTriggerModeBits(
                                 const ExternalTriggerMode externalTriggerMode);

    inline uint32_t getInjectedExternalTriggerBits(
                         const InjectedExternalTrigger injectedExternalTrigger);

    inline void setDmaEnabled(const bool enabled);

    inline void setDmaMode(const DmaMode dmaMode);

    inline void setInterruptEventEnabled(const InterruptEvent interruptEvent,
                                         const bool enabled);

    inline bool isInterruptEventEnabled(const InterruptEvent interruptEvent);

    inline bool isInterruptEventPending(const InterruptEvent interruptEvent);

    inline void setChannelSampleTime(const ChannelId channelId,
                                     const SampleTime sampleTime);

    inline bool isReady();

    inline bool isConversionComplete();

    inline bool isSequenceComplete();

    inline int16_t getRegularData();

    inline int16_t getInjectedData(const uint32_t conversionIndex);

    inline void setOffsetValue(const uint32_t offsetIndex,
                               const uint16_t offsetValue);

    inline void setOffsetChannel(const uint32_t offsetIndex,
    						     const ChannelId channelId);

    inline void readyInterruptEventHandler();

    inline void endOfSamplingInterruptEventHandler();

    inline void endOfRegularConversionInterruptEventHandler();

    inline void endOfRegularSequenceInterruptEventHandler();

    inline void overrunInterruptEventHandler();

    inline void endOfInjectedConversionInterruptEventHandler();

    inline void endOfInjectedSequenceInterruptEventHandler();

    inline void analogWatchdog1InterruptEventHandler();

    inline void analogWatchdog2InterruptEventHandler();

    inline void analogWatchdog3InterruptEventHandler();

    inline void injectedContextQueueOverflowInterruptEventHandler();

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------

    void interruptHandler();
};

}; // namespace Plat4m

#endif // PLAT4M_ADC_STM32F30X_H
