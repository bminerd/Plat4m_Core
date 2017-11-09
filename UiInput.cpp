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
/// @file UiInput.cpp
/// @author Ben Minerd
/// @date 7/10/2013
/// @brief UiInput class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <UiInput.h>
#include <System.h>

using Plat4m::UiInput;

//------------------------------------------------------------------------------
// Public virtual methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
UiInput::Error UiInput::setEventCallback(EventCallback& eventCallback)
{
    if (isNullPointer(myEventCallback))
    {
        myEventCallback = &eventCallback;
    }
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
UiInput::Error UiInput::setActionMap(const UiAction::Map& actionMap)
{
//    myActionMap = &actionMap;
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
void UiInput::setEventAction(const Id eventId, const Action action)
{
    if (isValidPointer(myEventBehaviors))
    {
        myEventBehaviors[eventId].action = action;
    }
}

//------------------------------------------------------------------------------
UiInput::Action UiInput::getEventAction(const Id eventId)
{
    if (isValidPointer(myEventBehaviors))
    {
        return myEventBehaviors[eventId].action;
    }
    
    return ACTION_NONE;
}

//------------------------------------------------------------------------------
void UiInput::handleEvent(const Event& event)
{
    if (isValidPointer(myActionMap))
    {
        myActionMap->actions[event.id]->call();
    }
    else if (isValidPointer(myEventCallback))
    {
        myEventCallback->call(event);
    }
}

//------------------------------------------------------------------------------
// Protected constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
UiInput::UiInput() :
	Module(),
    myEventCallback(0),
    myActionMap(0),
    myEventBehaviors(0),
    myNEventBehaviors(0)
{
}

//------------------------------------------------------------------------------
UiInput::UiInput(Event::Behavior* eventBehaviors,
                 const uint32_t nEventBehaviors) :
	Module(),
    myEventCallback(0),
    myActionMap(0),
    myEventBehaviors(eventBehaviors),
    myNEventBehaviors(nEventBehaviors)
{
    for (uint32_t i = 0; i < myNEventBehaviors; i++)
    {
        myEventBehaviors[i].action = ACTION_NONE;
    }
}

//------------------------------------------------------------------------------
// Protected virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
UiInput::~UiInput()
{
}
