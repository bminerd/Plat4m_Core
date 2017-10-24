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
/// @file Ui.h
/// @author Ben Minerd
/// @date 6/28/2013
/// @brief Ui class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Ui.h>
#include <System.h>
#include <ArrayN.h>
#include <CallbackMethod.h>

using Plat4m::Ui;
using Plat4m::Module;
using Plat4m::Thread;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const Plat4m::TimeMs Ui::myInputThreadPeriodMs = 10;

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Thread& Ui::getInputThread()
{
    return myInputThread;
}

//------------------------------------------------------------------------------
Thread& Ui::getEventThread()
{
    return myEventThread;
}

//------------------------------------------------------------------------------
Ui::Error Ui::addInput(UiInput& uiInput)
{
	UiInput* input = &uiInput;
    myInputList.append(input);

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
Ui::Error Ui::addState(UiState& uiState)
{
	UiState* state = &uiState;
    myStateList.append(state);

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Protected constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Ui::Ui() :
    myInputThread(System::createThread(
                                 createCallback(this, &Ui::inputThreadCallback),
                                 myInputThreadPeriodMs)),
    myEventThread(System::createThread(
                                 createCallback(this, &Ui::eventThreadCallback),
                                 10))
{
}

//------------------------------------------------------------------------------
// Protected virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Ui::~Ui()
{
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error Ui::interfaceSetEnabled(const bool enabled)
{
    myInputThread.setEnabled(enabled);
    myEventThread.setEnabled(enabled);

    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void Ui::inputThreadCallback()
{
    const uint32_t timeMs = System::getTimeMs();

    List<UiInput*>::Iterator iterator = myInputList.iterator();

    while (iterator.hasCurrent())
    {
        UiInput* input = iterator.current();
        ArrayN<UiInput::Event, 10> inputEvents;

        input->poll(timeMs, inputEvents);
        uint32_t size = inputEvents.getSize();

        if (size > 0)
        {
            for (uint32_t i = 0; i < size; i++)
            {
                UiInput::EventTag inputEventTag;
                inputEventTag.input = input;
                inputEventTag.event = inputEvents[i];

                myInputEventTagBuffer.write(inputEventTag);
            }
        }

        iterator.next();
    }
}

//------------------------------------------------------------------------------
void Ui::eventThreadCallback()
{
    uint32_t size = myInputEventTagBuffer.count();

    for (uint32_t i = 0; i < size; i++)
    {
        UiInput::EventTag inputEventTag;
        myInputEventTagBuffer.read(inputEventTag);

        inputEventTag.input->handleEvent(inputEventTag.event);
    }
}
