/*------------------------------------------------------------------------------
 *       _______    __                           ___
 *      ||  ___ \  || |             __          //  |
 *      || |  || | || |   _______  || |__      //   |    _____  ___
 *      || |__|| | || |  // ___  | ||  __|    // _  |   ||  _ \/ _ \
 *      ||  ____/  || | || |  || | || |      // /|| |   || |\\  /\\ \
 *      || |       || | || |__|| | || |     // /_|| |_  || | || | || |
 *      || |       || |  \\____  | || |__  //_____   _| || | || | || |
 *      ||_|       ||_|       ||_|  \\___|       ||_|   ||_| ||_| ||_|
 *
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2013 Benjamin Minerd
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *----------------------------------------------------------------------------*/

/**
 * @file Button.h
 * @author Ben Minerd
 * @date 7/10/2013
 * @brief Button class.
 */

#ifndef _BUTTON_H_
#define _BUTTON_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <EnableLine.h>
#include <UiInput.h>
#include <Callback.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

#define BUTTON_STATE_LOG_COUNT      4
#define BUTTON_EVENT_COUNT          5

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

class Button : public UiInput
{
public:
    
    /*--------------------------------------------------------------------------
     * Public structures
     *------------------------------------------------------------------------*/
         
    struct Event
    {
        enum Id
        {
            ID_PRESS = 0,
            ID_RELEASE,
            ID_HOLD,
            ID_SINGLE_TAP,
            ID_DOUBLE_TAP
        };
        
        enum Mode
        {
            MODE_SINGLE,
            MODE_PERSISTENT
        };
        
        struct Behavior
        {
            bool isEnabled;
            Mode mode;
            uint32_t persistentDelayMs;
            uint32_t lastPersistentTimestampMs;
            unsigned int nStatesSinceEvent;
        };
    };
    
    struct Sequence
    {
        struct State
        {
            bool isActive;
            uint32_t minDurationMs;
            uint32_t maxDurationMs;
        };
        
        const State* states;
        const unsigned int nStates;
        const bool isRepeatable;
    };
    
    struct ActionMap
    {
        
    };
    
    /*--------------------------------------------------------------------------
     * Public constructors and destructors
     *------------------------------------------------------------------------*/
    
    Button(EnableLine& enableLine);
    
    /*--------------------------------------------------------------------------
     * Public implemented methods
     *------------------------------------------------------------------------*/
    
    void poll(const uint32_t timeMs, Array<UiInput::Event>& events);
    
    /*--------------------------------------------------------------------------
     * Public methods
     *------------------------------------------------------------------------*/
    
    Error isActive(bool& isActive);
    
    Error enableEvent(const Event::Id id, const bool enable);
    
    Error setEventMode(const Event::Id id, const Event::Mode mode);
    
private:
    
    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/
    
    UiInput::Event::Behavior myEventBehaviors[BUTTON_EVENT_COUNT];
    
    State myStateLog[BUTTON_STATE_LOG_COUNT];
    
    Event::Behavior myEventBehaviorMap[BUTTON_EVENT_COUNT];
    
    EnableLine& myEnableLine;
    
    /*--------------------------------------------------------------------------
     * Private implemented methods
     *------------------------------------------------------------------------*/
    
    Error driverEnable(const bool enable);
    
    /*--------------------------------------------------------------------------
     * Private methods
     *------------------------------------------------------------------------*/
    
    void matchSequence(const uint32_t timeMs, Array<UiInput::Event>& events);
};

#endif // _BUTTON_H_