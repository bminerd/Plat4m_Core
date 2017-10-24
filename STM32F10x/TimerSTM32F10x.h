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
/// @file TimerSTM32F10x.h
/// @author Ben Minerd
/// @date 12/16/2016
/// @brief TimerSTM32F10x class header file.
///

#ifndef PLAT4M_TIMER_STM32F10X_H
#define PLAT4M_TIMER_STM32F10X_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m.h>
#include <Module.h>
#include <ErrorTemplate.h>
#include <Callback.h>
#include <GpioPinSTM32F10x.h>
#include <InterruptSTM32F10x.h>
#include <ProcessorSTM32F10x.h>

#include <stm32f10x.h>

#include <stdint.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class TimerSTM32F10x : public Module
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
        ID_17
    };

    enum ChannelId
    {
        CHANNEL_ID_1 = 0,
        CHANNEL_ID_2,
        CHANNEL_ID_3,
        CHANNEL_ID_4
    };

    enum ComplementaryChannelId
    {
        COMPLEMENTARY_CHANNEL_ID_1 = 0,
        COMPLEMENTARY_CHANNEL_ID_2,
        COMPLEMENTARY_CHANNEL_ID_3
    };

    enum Remap
    {
        REMAP_0 = 0,
        REMAP_1,
        REMAP_2,
        REMAP_3
    };

    enum Mode
    {
        MODE_COUNTING_UP = 0,
        MODE_COUNTING_DOWN,
        MODE_COUNTING_UP_DOWN,
        MODE_INPUT_CAPTURE,
        MODE_OUTPUT_COMPARE,
        MODE_PWM_INPUT,
        MODE_PWM_OUTPUT,
        MODE_PULSE
    };

    enum OutputCompareMode
    {
        OUTPUT_COMPARE_MODE_FROZEN                   = 0,
        OUTPUT_COMPARE_MODE_ACTIVE_ON_MATCH          = 1,
        OUTPUT_COMPARE_MODE_INACTIVE_ON_MATCH        = 2,
        OUTPUT_COMPARE_MODE_TOGGLE                   = 3,
        OUTPUT_COMPARE_MODE_FORCE_INACTIVE           = 4,
        OUTPUT_COMPARE_MODE_FORCE_ACTIVE             = 5,
        OUTPUT_COMPARE_MODE_PWM_MODE_1               = 6,
        OUTPUT_COMPARE_MODE_PWM_MODE_2               = 7
    };

    enum Polarity
    {
        POLARITY_HIGH = 0,
        POLARITY_LOW  = 1
    };

    enum Resolution
    {
        RESOLUTION_MIN = 0,
        RESOLUTION_MAX,
        RESOLUTION_LEVELS
    };

    enum Alignment
    {
        ALIGNMENT_EDGE           = 0,
        ALIGNMENT_CENTER_DOWN    = 1,
        ALIGNMENT_CENTER_UP      = 2,
        ALIGNMENT_CENTER_UP_DOWN = 3
    };

    enum Direction
    {
        DIRECTION_UP   = 0,
        DIRECTION_DOWN = 1
    };

    enum Interrupt
    {
        INTERRUPT_TIM_1_BRK = 0,
        INTERRUPT_TIM_1_BRK_TIM_15     = INTERRUPT_TIM_1_BRK,
        INTERRUPT_TIM_1_BRK_TIM_9      = INTERRUPT_TIM_1_BRK,
        INTERRUPT_TIM_1_UP,
        INTERRUPT_TIM_1_UP_TIM_16      = INTERRUPT_TIM_1_UP,
        INTERRUPT_TIM_1_UP_TIM_10      = INTERRUPT_TIM_1_UP,
        INTERRUPT_TIM_1_TRG_COM,
        INTERRUPT_TIM_1_TRG_COM_TIM_17 = INTERRUPT_TIM_1_TRG_COM,
        INTERRUPT_TIM_1_TRG_COM_TIM_11 = INTERRUPT_TIM_1_TRG_COM,
        INTERRUPT_TIM_1_CC,
        INTERRUPT_TIM_2,
        INTERRUPT_TIM_3,
        INTERRUPT_TIM_4,
        INTERRUPT_TIM_5,
        INTERRUPT_TIM_6,
        INTERRUPT_TIM_6_DAC            = INTERRUPT_TIM_6,
        INTERRUPT_TIM_7,
        INTERRUPT_TIM_8_BRK,
        INTERRUPT_TIM_8_BRK_TIM_12     = INTERRUPT_TIM_8_BRK,
        INTERRUPT_TIM_8_UP,
        INTERRUPT_TIM_8_UP_TIM_13      = INTERRUPT_TIM_8_UP,
        INTERRUPT_TIM_8_TRG_COM,
        INTERRUPT_TIM_8_TRG_COM_TIM_14 = INTERRUPT_TIM_8_TRG_COM,
        INTERRUPT_TIM_8_CC
    };

    enum InterruptEvent
    {
        INTERRUPT_EVENT_UPDATE = 0,
        INTERRUPT_EVENT_CAPTURE_COMPARE_CHANNEL_1,
        INTERRUPT_EVENT_CAPTURE_COMPARE_CHANNEL_2,
        INTERRUPT_EVENT_CAPTURE_COMPARE_CHANNEL_3,
        INTERRUPT_EVENT_CAPTURE_COMPARE_CHANNEL_4,
        INTERRUPT_EVENT_COMMUTATION,
        INTERRUPT_EVENT_TRIGGER,
        INTERRUPT_EVENT_BREAK,
        INTERRUPT_EVENT_CAPTURE_COMPARE_OVERCAPTURE_CHANNEL_1,
        INTERRUPT_EVENT_CAPTURE_COMPARE_OVERCAPTURE_CHANNEL_2,
        INTERRUPT_EVENT_CAPTURE_COMPARE_OVERCAPTURE_CHANNEL_3,
        INTERRUPT_EVENT_CAPTURE_COMPARE_OVERCAPTURE_CHANNEL_4
    };

    enum MasterMode
    {
        MASTER_MODE_RESET = 0,
        MASTER_MODE_ENABLE,
        MASTER_MODE_UPDATE,
        MASTER_MODE_COMPARE_PULSE,
        MASTER_MODE_COMPARE_OC1REF,
        MASTER_MODE_COMPARE_OC2REF,
        MASTER_MODE_COMPARE_OC3REF,
        MASTER_MODE_COMPARE_OC4REF
    };

    enum SlaveMode
    {
        SLAVE_MODE_DISABLED = 0,
        SLAVE_MODE_ENCODER_1,
        SLAVE_MODE_ENCODER_2,
        SLAVE_MODE_ENCODER_3,
        SLAVE_MODE_RESET,
        SLAVE_MODE_GATED,
        SLAVE_MODE_TRIGGER,
        SLAVE_MODE_EXTERNAL_CLOCK_1
    };

    enum InputSelection
    {
        INPUT_SELECTION_CH_1_TO_TI_1 = 0,
        INPUT_SELECTION_CH_1_2_3_TO_TI_1
    };

    enum CaptureCompareSelection
    {
        CAPTURE_COMPARE_SELECTION_OUTPUT = 0,
        CAPTURE_COMPARE_SELECTION_INPUT_IC_1_MAPPED_ON_TI_1,
        CAPTURE_COMPARE_SELECTION_INPUT_IC_1_MAPPED_ON_TI_2,
        CAPTURE_COMPARE_SELECTION_INPUT_IC_1_MAPPED_ON_TRC
    };

    enum InputCaptureSelection
    {
        INPUT_CAPTURE_SELECTION_IC_1_MAPPED_ON_TI_1 = 1,
        INPUT_CAPTURE_SELECTION_IC_1_MAPPED_ON_TI_2,
        INPUT_CAPTURE_SELECTION_IC_1_MAPPED_ON_TRC
    };

    enum TriggerSelection
    {
        TRIGGER_SELECTION_INTERNAL_TRIGGER_0 = 0,
        TRIGGER_SELECTION_TIM_1_FROM_TIM_5_TRGO = TRIGGER_SELECTION_INTERNAL_TRIGGER_0,
        TRIGGER_SELECTION_TIM_8_FROM_TIM_1_TRGO = TRIGGER_SELECTION_INTERNAL_TRIGGER_0,
        TRIGGER_SELECTION_INTERNAL_TRIGGER_1,
        TRIGGER_SELECTION_TIM_1_FROM_TIM_2_TRGO = TRIGGER_SELECTION_INTERNAL_TRIGGER_1,
        TRIGGER_SELECTION_TIM_8_FROM_TIM_2_TRGO = TRIGGER_SELECTION_INTERNAL_TRIGGER_1,
        TRIGGER_SELECTION_INTERNAL_TRIGGER_2,
        TRIGGER_SELECTION_TIM_1_FROM_TIM_3_TRGO = TRIGGER_SELECTION_INTERNAL_TRIGGER_2,
        TRIGGER_SELECTION_TIM_8_FROM_TIM_4_TRGO = TRIGGER_SELECTION_INTERNAL_TRIGGER_2,
        TRIGGER_SELECTION_INTERNAL_TRIGGER_3,
        TRIGGER_SELECTION_TIM_1_FROM_TIM_4_TRGO = TRIGGER_SELECTION_INTERNAL_TRIGGER_3,
        TRIGGER_SELECTION_TIM_8_FROM_TIM_5_TRGO = TRIGGER_SELECTION_INTERNAL_TRIGGER_3,
        TRIGGER_SELECTION_TI1_EDGE_DETECTOR,
        TRIGGER_SELECTION_FILTERED_TIMER_INPUT_1,
        TRIGGER_SELECTION_FILTERED_TIMER_INPUT_2,
        TRIGGER_SELECTION_EXTERNAL_TRIGGER_INPUT
    };

    typedef ErrorTemplate<ErrorCode> Error;

    typedef Callback<> UpdateInterruptCallback;

    typedef Callback<> BreakInterruptCallback;

    typedef Callback<> CaptureCompareInterruptCallback;

    typedef Callback<> TriggerInterruptCallback;

    typedef Callback<> CommutationInterruptCallback;

    struct Config
    {
        Mode mode;
        FrequencyHz frequencyHz;
        Resolution resolution;
        FloatingPointType maxFrequencyErrorPercent;
        uint32_t resolutionLevels;
        Direction direction;
        Alignment alignment;
        bool autoReloadPreloadEnabled;
        uint8_t repetitionCount;
    };

    struct InputCaptureConfig
    {
        InputCaptureSelection selection;
    };

    struct OutputCompareConfig
    {
        OutputCompareMode mode;
        Polarity polarity;
        bool preloadEnabled;
    };

    struct OutputCompareComplementaryConfig
    {
        Polarity polarity;
    };

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    TimerSTM32F10x(const Id id, const Remap remap);

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    Id getId() const;

    TIM_TypeDef* getTimer();

    void setUpdateInterruptCallback(
                              UpdateInterruptCallback& updateInterruptCallback);

    void setBreakInterruptCallback(
                                BreakInterruptCallback& breakInterruptCallback);

    void setChannelCaptureCompareInterruptCallback(
              const ChannelId channelId,
              CaptureCompareInterruptCallback& captureCompareInterruptCallback);

    void setTriggerInterruptCallback(
                            TriggerInterruptCallback& triggerInterruptCallback);

    void setCommutationInterruptCallback(
                    CommutationInterruptCallback& commutationInterruptCallback);

    Error setConfig(const Config& config);

    uint32_t getPeriod() const;

    void setChannelCaptureCompareEnabled(const ChannelId channelId,
                                         const bool enabled);

    void setComplementaryChannelCaptureCompareEnabled(
                            const ComplementaryChannelId complementaryChannelId,
                            const bool enabled);

    void setChannelOutputCompareMode(const ChannelId channelId,
                                     const OutputCompareMode outputCompareMode);

    void setChannelOutputComparePreloadEnabled(const ChannelId channelId,
                                               const bool enabled);

    void setChannelPolarity(const ChannelId channelId, const Polarity polarity);

    void setComplementaryChannelPolarity(
                            const ComplementaryChannelId complementaryChannelId,
                            const Polarity polarity);

    void setChannelCaptureCompareValue(const ChannelId channelId,
                                       const uint32_t value);

    void generateUpdateEvent();

    void generateCaptureCompareControlUpdate();

    void setMasterMode(const MasterMode masterMode);

    void setSlaveMode(const SlaveMode slaveMode);

    void setInputSelection(const InputSelection inputSelection);

    void setInterruptEventEnabled(const InterruptEvent interruptEvent,
                                  const bool enabled);

    void setChannelCaptureCompareSelection(
                         const ChannelId channelId,
                         const CaptureCompareSelection captureCompareSelection);

    void setChannelOutputCompareConfig(
                                 const ChannelId channelId,
                                 const OutputCompareConfig outputCompareConfig);

    void setChannelInputCaptureConfig(
                                   const ChannelId channelId,
                                   const InputCaptureConfig inputCaptureConfig);

    void setPrescaler(const uint32_t prescaler);

    void setClockDivision(const uint32_t clockDivision);

    void setPeriod(const uint32_t period);

    void setTriggerSelection(const TriggerSelection triggerSelection);

    uint32_t getChannelCaptureCompareValue(const ChannelId channelId);

    void setCounterEnabled(const bool enabled);

    void setRepetitionCount(const uint8_t repetitionCount);

    void setMainOutputEnabled(const bool enabled);

    uint32_t getCounterValue();

    void setCaptureComparePreloadEnabled(const bool enabled);

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    // Constants

    static const ProcessorSTM32F10x::Peripheral myPeripheralMap[];

    static const uint16_t myCounterModeMap[];

    static const uint16_t myPolarityMap[];

    static const InterruptSTM32F10x::Id myInterruptIdMap[];

    static const uint32_t myInterruptEventMap[];

    // Variables

    static TIM_TypeDef* myTimerMap[];

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------
    
    const Id myId;
    
    const Remap myRemap;

    UpdateInterruptCallback* myUpdateInterruptCallback;

    BreakInterruptCallback* myBreakInterruptCallback;

    CaptureCompareInterruptCallback* myCaptureCompareInterruptCallbacks[4];

    TriggerInterruptCallback* myTriggerInterruptCallback;

    CommutationInterruptCallback* myCommutationInterruptCallback;

    TIM_TypeDef* myTimer;
    
    InterruptSTM32F10x* myGlobalInterrupt;

    InterruptSTM32F10x* myBreakInterrupt;

    InterruptSTM32F10x* myUpdateInterrupt;

    InterruptSTM32F10x* myCaptureCompareInterrupt;

    InterruptSTM32F10x* myTriggerCommutationInterrupt;

    FrequencyHz myInputClockFrequencyHz;
    
    FrequencyHz myOutputClockFrequencyHz;
    
    uint32_t myPeriod;
    
    Config myConfig;

    //--------------------------------------------------------------------------
    // Private methods implemented from Module
    //--------------------------------------------------------------------------
    
    Module::Error driverSetEnabled(const bool enabled);

    //--------------------------------------------------------------------------
    // Private inline methods
    //--------------------------------------------------------------------------

    inline void initialize();

    inline void setDirection(const Direction Direction);

    inline void setAlignment(const Alignment alignment);

    inline void setAutoReloadPreloadEnabled(const bool enabled);

    inline bool isInterruptEventEnabled(const InterruptEvent interruptEvent);

    inline bool isInterruptEventPending(const InterruptEvent interruptEvent);

    inline void clearPendingInterruptEvent(const InterruptEvent interruptEvent);

    inline void setRemap(const Remap remap);

    inline void setCounterValue(const uint16_t counterValue);

    inline void basicTimerInterruptHandler();

    inline void generalPurposeTimerInterruptHandler();

    inline void captureCompareInterruptHandler();

    inline void triggerCommutationInterruptHandler();

    inline void updateInterruptEventHandler();

    inline void captureCompareChannel1InterruptEventHandler();

    inline void captureCompareChannel2InterruptEventHandler();

    inline void captureCompareChannel3InterruptEventHandler();

    inline void captureCompareChannel4InterruptEventHandler();

    inline void commutationInterruptEventHandler();

    inline void triggerInterruptEventHandler();

    inline void breakInterruptEventHandler();

    inline void captureCompareChannel1OvercaptureInterruptEventHandler();

    inline void captureCompareChannel2OvercaptureInterruptEventHandler();

    inline void captureCompareChannel3OvercaptureInterruptEventHandler();

    inline void captureCompareChannel4OvercaptureInterruptEventHandler();
};

}; // namespace Plat4m

#endif // PLAT4M_TIMER_STM32F10X_H
