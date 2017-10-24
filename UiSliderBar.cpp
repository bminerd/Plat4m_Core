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
 * Copyright (c) 2014 Benjamin Minerd
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
 * @file UiSliderBar.cpp
 * @author Ben Minerd
 * @date 3/11/2014
 * @brief UiSliderBar class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <UiSliderBar.h>
#include <UiView.h>

// For now
#include <icons.h>

using Plat4m::UiSliderBar;

/*------------------------------------------------------------------------------
 * Public constructors and destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
UiSliderBar::UiSliderBar(UiPrinter& printer,
                         UiView* view,
                         UiItem* parent,
                         const unsigned int nValues) :
    UiItem(printer, view, parent),
    myNValues(nValues),
    myIncreaseButton(printer,
                     view,
                     this,
                     buttonIncrease2Unpressed,
                     buttonIncrease2Pressed),
    myDecreaseButton(printer,
                     view,
                     this,
                     buttonDecrease2Unpressed,
                     buttonDecrease2Pressed),
    myAxisLine(printer, view, this),
    myValueLine(printer, view, this)
{
    // For now
    setWidth(96);
    setHeight(32);
    
    myDecreaseButton.setHorizontalAlignment(HORIZONTAL_ALIGNMENT_LEFT);
    myDecreaseButton.setVerticalAlignment(VERTICAL_ALIGNMENT_CENTER);
    
    myIncreaseButton.setHorizontalAlignment(HORIZONTAL_ALIGNMENT_RIGHT);
    myIncreaseButton.setVerticalAlignment(VERTICAL_ALIGNMENT_CENTER);
    
    myAxisLine.setThickness(1);
    myAxisLine.setStartPoint(myDecreaseButton.getWidth() + 4, getHeight() / 2);
    myAxisLine.setEndPoint(getWidth() - myIncreaseButton.getWidth() - 4,
                           getHeight() / 2);
    
    myValueLine.setThickness(2);
    myValueLine.setStartPoint(myDecreaseButton.getWidth() + 24, 8);
    myValueLine.setEndPoint(myDecreaseButton.getWidth() + 24, 24);
}

/*------------------------------------------------------------------------------
 * Private implemented methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
bool UiSliderBar::driverHandleAction(const UiInput::Action action)
{
    switch (action)
    {
        case UiInput::ACTION_UP_PRESSED:
        {
            myDecreaseButton.setPressed(true);
            myValueLine.shiftX(-2);
            
            return true;
        }
        case UiInput::ACTION_UP_RELEASED:
        {
            myDecreaseButton.setPressed(false);
            
            return true;
        }
        case UiInput::ACTION_DOWN_PRESSED:
        {
            myIncreaseButton.setPressed(true);
            myValueLine.shiftX(2);
            
            return true;
        }
        case UiInput::ACTION_DOWN_RELEASED:
        {
            myIncreaseButton.setPressed(false);
            
            return true;
        }
        case UiInput::ACTION_SELECT:
        {
            // For now
            setVisible(false);
            getParent()->setFocus();
            getParent()->moveIntoView();
            getView()->setReferenceItem(*getParent());
            
            return true;
        }
        default:
        {
            break;
        }
    }
    
    return false;
}
