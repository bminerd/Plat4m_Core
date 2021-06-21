/**
 * Copyright (c) 2021, SignalQuest LLC
 */

///
/// @file TimerNRF5340.h
/// @author Ben Minerd
/// @date 1/6/2021
/// @brief TimerNRF5340 class header file.
///

#ifndef PLAT4M_TIMER_NRF5340_H
#define PLAT4M_TIMER_NRF5340_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <stdint.h>

#include <Plat4m_Core/Module.h>
#include <Plat4m_Core/ErrorTemplate.h>
#include <Plat4m_Core/Callback.h>
#include <NRF5340/GpioPinNRF5340.h>
#include <NRF5340/InterruptNRF5340.h>
#include <NRF5340/ProcessorNRF5340.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class TimerNRF5340 : public Module
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
        ID_0 = 0,
        ID_1,
        ID_2,
        ID_3,
        ID_4
    };

    enum Channel
    {
        CHANNEL_0 = 0,
        CHANNEL_1,
        CHANNEL_2,
        CHANNEL_3,
        CHANNEL_4,
        CHANNEL_5
    };

    enum Mode
    {
        MODE_TIMER = 0,
        MODE_COUNTER,
        MODE_LOW_POWER_COUNTER
    };

    enum BitMode
    {
        BIT_MODE_16 = 0,
        BIT_MODE_8,
        BIT_MODE_24,
        BIT_MODE_32
    };

    enum Resolution
    {
        RESOLUTION_MIN = 0,
        RESOLUTION_MAX,
        RESOLUTION_LEVELS
    };

    enum InterruptEvent
    {
        INTERRUPT_EVENT_COMPARE_0 = 0,
        INTERRUPT_EVENT_COMPARE_1,
        INTERRUPT_EVENT_COMPARE_2,
        INTERRUPT_EVENT_COMPARE_3,
        INTERRUPT_EVENT_COMPARE_4,
        INTERRUPT_EVENT_COMPARE_5
    };

    enum Task
    {
        TASK_START = 0,
        TASK_STOP,
        TASK_COUNT,
        TASK_CLEAR,
        TASK_CAPTURE_0,
        TASK_CAPTURE_1,
        TASK_CAPTURE_2,
        TASK_CAPTURE_3,
        TASK_CAPTURE_4,
        TASK_CAPTURE_5
    };

    typedef ErrorTemplate<ErrorCode> Error;

    typedef Callback<> CompareInterruptCallback;

    struct Config
    {
        Mode mode;
        FrequencyHz frequencyHz;
        Resolution resolution;
        RealNumber maxFrequencyErrorPercent;
        uint32_t resolutionLevels;
    };

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    TimerNRF5340(const Id id);

    TimerNRF5340(const Id id,
                  CompareInterruptCallback& compareInterruptCallback);

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~TimerNRF5340();

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    Id getId() const;

    NRF_TIMER_Type* getTimer();

    InterruptNRF5340& getInterrupt();

    void setCompareChannelInterruptCallback(
                            const Channel channel,
                            CompareInterruptCallback& compareInterruptCallback);

    Error setConfig(const Config& config);

    void setChannelCaptureCompareValue(const Channel channel,
                                       const uint32_t value);

    uint32_t getChannelCaptureCompareValue(const Channel channel);

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    // Constants

    static const InterruptNRF5340::Id myInterruptIdMap[];

    // Variables

    static NRF_TIMER_Type* myTimerMap[];

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------
    
    const Id myId;

    const uint8_t myChannelCount;

    bool myIsConfigured;

    InterruptNRF5340 myInterrupt;

    CompareInterruptCallback* myCompareInterruptCallbacks[6];

    NRF_TIMER_Type* myTimer;

    FrequencyHz myInputClockFrequencyHz;

    FrequencyHz myOutputClockFrequencyHz;

    uint32_t myPeriod;

    BitMode myBitMode;

    Config myConfig;

    //--------------------------------------------------------------------------
    // Private methods implemented from Module
    //--------------------------------------------------------------------------
    
    Module::Error driverSetEnabled(const bool enabled);

    //--------------------------------------------------------------------------
    // Private inline methods
    //--------------------------------------------------------------------------

    inline void initialize();

    inline void setMode(const Mode mode);

    inline void setPrescaler(const uint32_t prescaler);

    inline void setBitMode(const BitMode bitMode);

    inline void setInterruptEventEnabled(const InterruptEvent interruptEvent,
                                         const bool enabled);

    inline bool isInterruptEventEnabled(const InterruptEvent interruptEvent);

    inline bool isInterruptEventPending(const InterruptEvent interruptEvent);

    inline void clearPendingInterruptEvent(const InterruptEvent interruptEvent);

    inline void triggerTask(const Task task);

    inline void interruptHandler();
};

}; // namespace Plat4m

#endif // PLAT4M_TIMER_NRF5340_H
