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
/// @file AdcSTM32F4xx.h
/// @author Ben Minerd
/// @date 4/10/13
/// @brief AdcSTM32F4xx class header file.
///

#ifndef ADC_STM32F4XX_H
#define ADC_STM32F4XX_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m.h>
#include <ErrorTemplate.h>
#include <Array.h>
#include <InterruptSTM32F4xx.h>
#include <DmaStreamSTM32F4xx.h>

#include <stm32f4xx.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class AdcSTM32F4xx : public Module
{
public:
    
    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------
    
    enum ErrorCode
    {
        ERROR_CODE_NONE = 0
    };

    /**
     * @brief Enumeration of ADC IDs.
     */
    enum Id
    {
        ID_1 = 0,
        ID_2,
        ID_3
    };
    
    /**
     * @brief Enumeration of ADC channels.
     */
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
        CHANNEL_ID_16
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

    enum ExternalTriggerMode
    {
        EXTERNAL_TRIGGER_MODE_DISABLED = 0,
        EXTERNAL_TRIGGER_MODE_RISING_EDGE,
        EXTERNAL_TRIGGER_MODE_FALLING_EDGE,
        EXTERNAL_TRIGGER_MODE_RISING_FALLING_EDGE
    };

    enum ExternalTrigger
    {
        EXTERNAL_TRIGGER_0                                = 0,
        EXTERNAL_TRIGGER_ADC_1_2_TIM_1_CC_1               = EXTERNAL_TRIGGER_0,
        EXTERNAL_TRIGGER_ADC_3_4_TIM_3_CC_1               = EXTERNAL_TRIGGER_0,
        EXTERNAL_TRIGGER_1                                = 1,
        EXTERNAL_TRIGGER_ADC_1_2_TIM_1_CC_2               = EXTERNAL_TRIGGER_1,
        EXTERNAL_TRIGGER_ADC_3_4_TIM_2_CC_3               = EXTERNAL_TRIGGER_1,
        EXTERNAL_TRIGGER_2                                = 2,
        EXTERNAL_TRIGGER_ADC_1_2_TIM_1_CC_3_TIM_20_TRGO   = EXTERNAL_TRIGGER_2,
        EXTERNAL_TRIGGER_ADC_3_4_TIM_1_CC_3               = EXTERNAL_TRIGGER_2,
        EXTERNAL_TRIGGER_3                                = 3,
        EXTERNAL_TRIGGER_ADC_1_2_TIM_2_CC_2_TIM_20_TRGO_2 = EXTERNAL_TRIGGER_3,
        EXTERNAL_TRIGGER_ADC_3_4_TIM_8_CC_1               = EXTERNAL_TRIGGER_3,
        EXTERNAL_TRIGGER_4                                = 4,
        EXTERNAL_TRIGGER_ADC_1_2_TIM_3_TRGO               = EXTERNAL_TRIGGER_4,
        EXTERNAL_TRIGGER_ADC_3_4_TIM_8_TRGO               = EXTERNAL_TRIGGER_4,
        EXTERNAL_TRIGGER_5                                = 5,
        EXTERNAL_TRIGGER_ADC_1_2_TIM_4_CC_4_TIM_20_CC_1   = EXTERNAL_TRIGGER_5,
        EXTERNAL_TRIGGER_ADC_3_4_EXTI_2_TIM_20_TRGO       = EXTERNAL_TRIGGER_5,
        EXTERNAL_TRIGGER_6                                = 6,
        EXTERNAL_TRIGGER_ADC_1_2_EXTI_11                  = EXTERNAL_TRIGGER_6,
        EXTERNAL_TRIGGER_ADC_3_4_TIM_4_CC_1_TIM_20_TRGO_2 = EXTERNAL_TRIGGER_6,
        EXTERNAL_TRIGGER_7                                = 7,
        EXTERNAL_TRIGGER_ADC_1_2_TIM_8_TRGO               = EXTERNAL_TRIGGER_7,
        EXTERNAL_TRIGGER_ADC_3_4_TIM_2_TRGO               = EXTERNAL_TRIGGER_7,
        EXTERNAL_TRIGGER_8                                = 8,
        EXTERNAL_TRIGGER_ADC_1_2_TIM_8_TRGO_2             = EXTERNAL_TRIGGER_8,
        EXTERNAL_TRIGGER_ADC_3_4_TIM_8_TRGO_2             = EXTERNAL_TRIGGER_8,
        EXTERNAL_TRIGGER_9                                = 9,
        EXTERNAL_TRIGGER_ADC_1_2_TIM_1_TRGO               = EXTERNAL_TRIGGER_9,
        EXTERNAL_TRIGGER_ADC_3_4_TIM_1_TRGO               = EXTERNAL_TRIGGER_9,
        EXTERNAL_TRIGGER_10                               = 10,
        EXTERNAL_TRIGGER_ADC_1_2_TIM_1_TRGO_2             = EXTERNAL_TRIGGER_10,
        EXTERNAL_TRIGGER_ADC_3_4_TIM_1_TRGO_2             = EXTERNAL_TRIGGER_10,
        EXTERNAL_TRIGGER_11                               = 11,
        EXTERNAL_TRIGGER_ADC_1_2_TIM_2_TRGO               = EXTERNAL_TRIGGER_11,
        EXTERNAL_TRIGGER_ADC_3_4_TIM_3_TRGO               = EXTERNAL_TRIGGER_11,
        EXTERNAL_TRIGGER_12                               = 12,
        EXTERNAL_TRIGGER_ADC_1_2_TIM_4_TRGO               = EXTERNAL_TRIGGER_12,
        EXTERNAL_TRIGGER_ADC_3_4_TIM_4_TRGO               = EXTERNAL_TRIGGER_12,
        EXTERNAL_TRIGGER_13                               = 13,
        EXTERNAL_TRIGGER_ADC_1_2_TIM_6_TRGO_TIM_20_CC_2   = EXTERNAL_TRIGGER_13,
        EXTERNAL_TRIGGER_ADC_3_4_TIM_7_TRGO               = EXTERNAL_TRIGGER_13,
        EXTERNAL_TRIGGER_14                               = 14,
        EXTERNAL_TRIGGER_ADC_1_2_TIM_15_TRGO              = EXTERNAL_TRIGGER_14,
        EXTERNAL_TRIGGER_ADC_3_4_TIM_15_TRGO              = EXTERNAL_TRIGGER_14,
        EXTERNAL_TRIGGER_15                               = 15,
        EXTERNAL_TRIGGER_ADC_1_2_TIM_3_CC_4_TIM_20_CC_3   = EXTERNAL_TRIGGER_15,
        EXTERNAL_TRIGGER_ADC_3_4_TIM_2_CC_1_TIM_20_CC_1   = EXTERNAL_TRIGGER_15
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
        INTERRUPT_EVENT_INJETED_CONTEXT_QUEUE_OVERFLOW
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

    typedef ErrorTemplate<ErrorCode> Error;

    struct Config
    {
        Resolution resolution;
        Alignment alignment;
        Array<ChannelId> sequenceChannels;
        ExternalTriggerMode externalTriggerMode;
        ExternalTrigger externalTrigger;
        bool dmaEnabled;
        DmaMode dmaMode;
    };

    struct ChannelConfig
    {
        SampleTime sampleTime;
    };


    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    AdcSTM32F4xx(const Id id);

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~AdcSTM32F4xx();
    
    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------
    
    ADC_TypeDef* getAdc();

    void setSTM32F4zxConfig(const Config& config);

    Config getSTM32F4xxConfig() const;

    void setSTM32F4xxChannelConfig(const ChannelId channelId,
                                   const ChannelConfig& channelConfig);

    ChannelConfig getSTM32F4xxChannelConfig(const ChannelId channelId) const;

    void setDmaStream(DmaStreamSTM32F4xx& dmaStream,
                      DmaStreamSTM32F4xx::TransferCompleteCallback& callback);

    Error readChannelVoltage(const ChannelId channelId, float& voltage);

    float readChannelVoltageFast(const ChannelId channelId);

    Error readAllVoltages();
    
private:
    
    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    // Constants

    static const ProcessorSTM32F4xx::Peripheral myPeripheralMap[];

    static const InterruptSTM32F4xx::Id myInterruptIdMap[];

    static const uint32_t myResolutionMap[];

    static const float myResolutionMaxValueMap[];

    // Variables

    static ADC_TypeDef* myAdcMap[];

    static uint16_t myUserMap[4];

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------
    
    const Id myId;
    
    ADC_TypeDef* myAdc;
    
    InterruptSTM32F4xx myInterrupt;
    
    DmaStreamSTM32F4xx* myDmaStream;

    Config myConfig;

    ChannelConfig myChannelConfigs[16];

    float myLevelsToVolts;

    uint16_t myValues[16];

    uint8_t myCurrentValueIndex;

    uint8_t myChannelToValueMap[16];
    
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

    inline void setSequenceLength(const uint32_t sequenceLength);

    inline void setSequenceConversionChannel(const uint32_t conversion,
                                             const ChannelId channelId);

    inline void setExternalTriggerMode(
                                 const ExternalTriggerMode externalTriggerMode);

    inline void setExternalTrigger(const ExternalTrigger externalTrigger);

    inline void setDmaEnabled(const bool enabled);

    inline void setDmaMode(const DmaMode dmaMode);

    inline void setInterruptEventEnabled(const InterruptEvent interruptEvent,
                                         const bool enabled);

    inline bool isInterruptEventEnabled(const InterruptEvent interruptEvent);

    inline bool isInterruptEventPending(const InterruptEvent interruptEvent);

    inline void setChannelSampleTime(const ChannelId channelId,
                                     const SampleTime sampleTime);

    inline bool isReady();

    inline void startRegularConversion();

    inline bool isConversionComplete();

    inline bool isSequenceComplete();

    inline uint16_t getRegularData();

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

#endif // ADC_STM32F4XX_H
