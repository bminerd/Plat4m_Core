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
/// @file UiInput.h
/// @author Ben Minerd
/// @date 7/10/2013
/// @brief UiInput class header file.
///

#ifndef PLAT4M_UI_INPUT_H
#define PLAT4M_UI_INPUT_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m.h>
#include <Module.h>
#include <Callback.h>
#include <Array.h>
#include <Buffer.h>
#include <UiAction.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class UiInput : public Module
{
public:
    
    //--------------------------------------------------------------------------
    // Public enumerations
    //--------------------------------------------------------------------------
    
    enum Error
    {
        ERROR_NONE
    };
    
    enum Action
    {
        ACTION_NONE = 0,
        ACTION_LEFT,
        ACTION_LEFT_PRESSED,    // For now!
        ACTION_LEFT_RELEASED,   // For now!
        ACTION_RIGHT,
        ACTION_RIGHT_PRESSED,   // For now!
        ACTION_RIGHT_RELEASED,  // For now!
        ACTION_UP,
        ACTION_UP_PRESSED,      // For now!
        ACTION_UP_RELEASED,     // For now!
        ACTION_DOWN,
        ACTION_DOWN_PRESSED,    // For now!
        ACTION_DOWN_RELEASED,   // For now!
        ACTION_SELECT
    };
    
    //--------------------------------------------------------------------------
    // Public structures
    //--------------------------------------------------------------------------
    
    struct State
    {
        bool isActive;
        uint32_t timeStampMs;
    };
         
    struct Event
    {
        Id id;
        uint32_t timeMs;
        
        struct Behavior
        {
            Action action;
            
            struct Map
            {
                Behavior* behaviors;
                const uint32_t nBehaviors;
            };
        };
    };
    
    struct EventTag
    {
        UiInput* input;
        Event event;
    };
    
    //--------------------------------------------------------------------------
    // Public typedefs
    //--------------------------------------------------------------------------
    
    typedef Callback<void, const Event&> EventCallback;
    
    //--------------------------------------------------------------------------
    // Public pure virtual methods
    //--------------------------------------------------------------------------
    
    virtual void poll(const uint32_t timeMs, Array<Event>& events) = 0;
    
    //--------------------------------------------------------------------------
    // Public virtual methods
    //--------------------------------------------------------------------------
    
    virtual Error setEventCallback(EventCallback& eventCallback);
    
    virtual Error setActionMap(const UiAction::Map& actionMap);
    
    virtual void setEventAction(const Id eventId, const Action action);
    
    virtual Action getEventAction(const Id eventId);
    
    virtual void handleEvent(const Event& event);
    
protected:
    
    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------

    UiInput();

    UiInput(Event::Behavior* eventBehaviors,
            const uint32_t nEventBehaviors);

    //--------------------------------------------------------------------------
    // Protected virtual destructors
    //--------------------------------------------------------------------------

    virtual ~UiInput();

    //--------------------------------------------------------------------------
    // Protected methods
    //--------------------------------------------------------------------------
    
    void bufferEvent(const Event& event);
    
private:
    
    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------
    
    EventCallback* myEventCallback;
    
    UiAction::Map* myActionMap;
    
    Event::Behavior* myEventBehaviors;
    
    const uint32_t myNEventBehaviors;
};

}; // namespace Plat4m

#endif // PLAT4M_UI_INPUT_H
