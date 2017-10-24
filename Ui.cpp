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
 * @file Ui.h
 * @author Ben Minerd
 * @date 6/28/2013
 * @brief Ui class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Ui.h>
#include <System.h>
#include <Plat4m.h>
#include <ArrayN.h>
#include <UiItem.h>

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

static const uint32_t inputTaskTimeDelayMs = 10;

/*------------------------------------------------------------------------------
 * Public constructors and destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Ui::Ui() :
    myInputTask(Plat4m::createCallback(this, &Ui::inputTaskCallback),
                inputTaskTimeDelayMs),
    myEventTask(Plat4m::createCallback(this, &Ui::eventTaskCallback)),
    myFocusItem(NULL_POINTER)
{
}

/*------------------------------------------------------------------------------
 * Public methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Ui::Error Ui::addInput(UiInput* uiInput)
{
    myInputList.append(uiInput);

    return ERROR_NONE;
}

//------------------------------------------------------------------------------
Ui::Error Ui::addState(UiState* uiState)
{
    myStateList.append(uiState);

    return ERROR_NONE;
}

//------------------------------------------------------------------------------
UiItem* Ui::getFocusItem()
{
    return myFocusItem;
}

//------------------------------------------------------------------------------
void Ui::setFocusItem(UiItem* focusItem)
{
    myFocusItem = focusItem;
}

//------------------------------------------------------------------------------
bool Ui::isFocusItem(UiItem& item)
{
    UiItem* itemPointer = &item;

    return (myFocusItem == itemPointer);
}

/*------------------------------------------------------------------------------
 * Private virtual methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Ui::Error Ui::enable(const bool enable)
{
    Error error =  driverEnable(enable);

    myInputTask.enable(enable);
    myEventTask.enable(enable);

    return error;
}

/*------------------------------------------------------------------------------
 * Private methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void Ui::inputTaskCallback()
{
    const uint32_t timeMs = System::timeGetMs();

    List<UiInput*>::Iterator iterator = myInputList.iterator();

    while (iterator.hasCurrent())
    {
        UiInput* input = iterator.current();
        ArrayN<UiInput::Event, 10> inputEvents;

        input->poll(timeMs, inputEvents);
        int size = inputEvents.getSize();

        if (size > 0)
        {
            for (int i = 0; i < size; i++)
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
void Ui::eventTaskCallback()
{
    int size = myInputEventTagBuffer.count();

    for (int i = 0; i < size; i++)
    {
        UiInput::EventTag inputEventTag;
        myInputEventTagBuffer.read(inputEventTag);

        if (IS_VALID_POINTER(myFocusItem))
        {
            UiInput::Action action =
                    inputEventTag.input->getEventAction(inputEventTag.event.id);

            // Clean this up!
            if (action != UiInput::ACTION_NONE)
            {
                if (!myFocusItem->handleAction(action))
                {
                    inputEventTag.input->handleEvent(inputEventTag.event);
                }
            }
            else
            {
                inputEventTag.input->handleEvent(inputEventTag.event);
            }
        }
        else
        {
            inputEventTag.input->handleEvent(inputEventTag.event);
        }
    }
}
