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
 * @file Button.cpp
 * @author Ben Minerd
 * @date 7/23/2013
 * @brief Button class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Button.h>
#include <System.h>
#include <MutexLock.h>

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/
    
static const uint32_t debounceTimeMs = 10;

static const Button::Sequence::State pressSequenceStates[] =
{
    // First state (inactive)
    {
        0,
        0,
        false
    },
    // Second state (active)
    {
        0,
        0,
        true
    }
};

static const Button::Sequence::State releaseSequenceStates[] =
{
    // First state (active)
    {
        0,
        0,
        true
    },
    // Second state (inactive)
    {
        0,
        0,
        false
    }
};

static const Button::Sequence::State holdSequenceStates[] =
{
    // First state (inactive)
    {
        0,
        0,
        false
    },
    // Second state (active)
    {
        1000,
        0,
        true
    }
};

static const Button::Sequence::State singleTapSequenceStates[] =
{
    // First state (inactive)
    {
        0,
        0,
        false
    },
    // Second state (active)
    {
        0,
        999,
        true
    },
    // Third state (inactive)
    {
        0,
        0,
        false
    }
};

static const Button::Sequence::State doubleTapSequenceStates[] =
{
    // First state (inactive)
    {
        0,
        0,
        false
    },
    // Second state (active)
    {
        0,
        1999,
        true
    },
    // Third state (inactive)
    {
        0,
        250,
        false
    },
    // Fourth state (active)
    {
        0,
        1999,
        true
    },
    // Fifth state (inactive)
    {
        0,
        0,
        false
    }
};

static const Button::Sequence sequences[] =
{
    // Button::Event::ID_PRESS
    {
        pressSequenceStates,
        ARRAY_SIZE(pressSequenceStates),
        false
    },
    // Button::Event::ID_RELEASE
    {
        releaseSequenceStates,
        ARRAY_SIZE(releaseSequenceStates),
        false
    },
    // Button::Event::ID_HOLD
    {
        holdSequenceStates,
        ARRAY_SIZE(holdSequenceStates),
        false
    },
    // Button::Event::ID_SINGLE_TAP
    {
        singleTapSequenceStates,
        ARRAY_SIZE(singleTapSequenceStates),
        true
    },
    // Button::Event::ID_DOUBLE_TAP
    {
        doubleTapSequenceStates,
        ARRAY_SIZE(doubleTapSequenceStates),
        false
    }
};

/*------------------------------------------------------------------------------
 * Public constructors and destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Button::Button(EnableLine& enableLine) :
    UiInput(myEventBehaviors, ARRAY_SIZE(myEventBehaviors)),
    myEnableLine(enableLine)
{
    // Initialize state log
    for (int i = 0; i < ARRAY_SIZE(myStateLog); i++)
    {
        myStateLog[i].isActive      = false;
        myStateLog[i].timeStampMs   = 0;
    }
    
    // Initialize event behavior map
    for (int i = 0; i < ARRAY_SIZE(myEventBehaviorMap); i++)
    {
        myEventBehaviorMap[i].isEnabled                 = false;
        myEventBehaviorMap[i].mode                      = Event::MODE_SINGLE;
        myEventBehaviorMap[i].persistentDelayMs         = 50;
        myEventBehaviorMap[i].lastPersistentTimestampMs = 0;
        myEventBehaviorMap[i].nStatesSinceEvent         = 0;
    }
}

/*------------------------------------------------------------------------------
 * Public implemented methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void Button::poll(const uint32_t timeMs, Array<UiInput::Event>& events)
{
    bool isActive;
    
    if (myEnableLine.isActive(isActive) != EnableLine::ERROR_NONE)
    {
        // Error
    }
    
    if (((isActive != myStateLog[0].isActive) &&
        ((timeMs - myStateLog[0].timeStampMs) >= debounceTimeMs))   ||
         (myStateLog[0].timeStampMs == 0))
    {
        // Push all old events back one slot
        for (int i = ARRAY_SIZE(myStateLog) - 1; i > 0; i--)
        {
            myStateLog[i] = myStateLog[(i - 1)];
        }

        myStateLog[0].isActive      = isActive;
        myStateLog[0].timeStampMs   = timeMs;
        
        for (int i = 0; i < ARRAY_SIZE(myEventBehaviorMap); i++)
        {
            myEventBehaviorMap[i].nStatesSinceEvent++;
        }
    }
    
    matchSequence(timeMs, events);
}

/*------------------------------------------------------------------------------
 * Public methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Button::Error Button::isActive(bool& isActive)
{    
    if (myEnableLine.isActive(isActive) != EnableLine::ERROR_NONE)
    {
        // Error
    }
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
Button::Error Button::enableEvent(const Event::Id id, const bool enable)
{
    myEventBehaviorMap[id].isEnabled = enable;
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
Button::Error Button::setEventMode(const Event::Id id, const Event::Mode mode)
{
    myEventBehaviorMap[id].mode = mode;
    
    return ERROR_NONE;
}

/*------------------------------------------------------------------------------
 * Private implemented methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
UiInput::Error Button::driverEnable(const bool enable)
{
    myEnableLine.enable(enable);
    
    return ERROR_NONE;
}

/*------------------------------------------------------------------------------
 * Private methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void Button::matchSequence(const uint32_t timeMs, Array<UiInput::Event>& events)
{
    for (int i = 0; i < ARRAY_SIZE(sequences); i++)
    {
        const Sequence& sequence = sequences[i];
        int logIndex = 0;
        int j;
        
        if ((myEventBehaviorMap[i].nStatesSinceEvent >= sequence.nStates) ||
            ((myEventBehaviorMap[i].mode == Event::MODE_PERSISTENT) &&
             ((timeMs - myEventBehaviorMap[i].lastPersistentTimestampMs) >=
              myEventBehaviorMap[i].persistentDelayMs)))
        {
            for (j = sequence.nStates - 1; j >= 0; j--)
            {
                // If levels don't match, exit loop
                if (sequence.states[j].isActive != myStateLog[logIndex].isActive)
                {
                    break;
                }

                // Most recent event
                if (logIndex == 0)
                {
                    // If event hasn't been active for minimum time,
                    // exit the loop (the most recent event is
                    // compared against system time)
                    if ((myStateLog[logIndex].timeStampMs +
                         sequence.states[j].minDurationMs)  > timeMs)
                    {
                        break;
                    }
                }
                // Older events
                else
                {
                    // If event hasn't been active for minimum time,
                    // exit the loop (older events are compared
                    // against previous)
                    if (((myStateLog[logIndex].timeStampMs +
                          sequence.states[j].minDurationMs) >=
                          myStateLog[logIndex - 1].timeStampMs)    ||
                         (((myStateLog[logIndex].timeStampMs +
                            sequence.states[j].maxDurationMs) <=
                            myStateLog[logIndex - 1].timeStampMs)  &&
                         (sequence.states[j].maxDurationMs != 0)))
                    {
                        break;
                    }
                }

                logIndex++;
            }
            
            // If reached last state of sequence, the log pattern matches the
            // sequence
            if (j == -1)
            {
                UiInput::Event event;
                event.id        = (Event::Id) i;
                event.timeMs    = timeMs;
                
                events.append(event);
                
                if ((myEventBehaviorMap[i].nStatesSinceEvent == 0) &&
                    (myEventBehaviorMap[i].mode == Event::MODE_PERSISTENT))
                {
                    myEventBehaviorMap[i].lastPersistentTimestampMs = timeMs;
                }
                
                if (sequence.isRepeatable)
                {
                    myEventBehaviorMap[i].nStatesSinceEvent = 1;
                }
                else
                {
                    myEventBehaviorMap[i].nStatesSinceEvent = 0;
                }
            }
        }
    }
}
