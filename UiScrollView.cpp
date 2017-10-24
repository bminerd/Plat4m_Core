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
 * @file UiScrollView.cpp
 * @author Ben Minerd
 * @date 12/2/2013
 * @brief UiScrollView class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <UiScrollView.h>
#include <icons.h>

using Plat4m::UiScrollView;

/*------------------------------------------------------------------------------
 * Public constructors and destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
UiScrollView::UiScrollView(Ui& ui,
                           UiPrinter& printer,
                           UiView* view,
                           UiItem* parent,
                           UiItem& rootItem,
                           const int width,
                           const int height) :
    UiView(ui, printer, view, parent, rootItem, width, height),
    myIsScrolling(false),
    myScrollBarLine(printer,
                    this,
                    this,
                    (width - 1),
                    0, 
                    (width - 1),
                    (height - 1)),
    myUpArrowButton(printer,
                    this,
                    this,
                    Button_Up_Arrow_Unpressed,
                    Button_Up_Arrow_Pressed),
    myDownArrowButton(printer,
                      this,
                      this,
                      Button_Down_Arrow_Unpressed,
                      Button_Down_Arrow_Pressed)
{
    myUpArrowButton.setHorizontalAlignment(HORIZONTAL_ALIGNMENT_ABSOLUTE);
    myUpArrowButton.setVerticalAlignment(VERTICAL_ALIGNMENT_ABSOLUTE);
    myUpArrowButton.setX(0);
    myUpArrowButton.setY(8);
    
    myDownArrowButton.setHorizontalAlignment(HORIZONTAL_ALIGNMENT_ABSOLUTE);
    myDownArrowButton.setVerticalAlignment(VERTICAL_ALIGNMENT_ABSOLUTE);
    myDownArrowButton.setX(0);
    myDownArrowButton.setY(16);
    
    getMyViewAreaItem().setX(myUpArrowButton.getWidth());
    getMyViewAreaItem().setWidth(width -
                                 myUpArrowButton.getWidth() -
                                 myScrollBarLine.getThickness() - 1);
    
    myScrollBarLine.setVisible(false);
    myUpArrowButton.setVisible(false);
    myDownArrowButton.setVisible(false);
}

/*------------------------------------------------------------------------------
 * Public implemented methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
bool UiScrollView::isItemInView(UiItem& item)
{
    if ((&item == &myScrollBarLine)   ||
        (&item == &myUpArrowButton)   ||
        (&item == &myDownArrowButton))
    {
        return true;
    }
    
    return UiView::isItemInView(item);
}

/*------------------------------------------------------------------------------
 * Private implemented methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
bool UiScrollView::driverHandleAction(const UiInput::Action action)
{
    switch (action)
    {
        case UiInput::ACTION_UP:
        {
            myUpArrowButton.setPressed(true);
            
            return true;
        }
        case UiInput::ACTION_UP_PRESSED:
        {
            myUpArrowButton.setPressed(true);
            
            return true;
        }
        case UiInput::ACTION_UP_RELEASED:
        {
            myUpArrowButton.setPressed(false);
            
            return true;
        }
        case UiInput::ACTION_DOWN:
        {
            myDownArrowButton.setPressed(true);
            
            return true;
        }
        case UiInput::ACTION_DOWN_PRESSED:
        {
            myDownArrowButton.setPressed(true);
            
            return true;
        }
        case UiInput::ACTION_DOWN_RELEASED:
        {
            myDownArrowButton.setPressed(false);
            
            return true;
        }
        default:
        {
            break;
        }
    }
    
//    return false;
    return true;
}

//------------------------------------------------------------------------------
void UiScrollView::driverSetFocusItem(UiItem* item)
{
    // Check for null pointer?
    
    int height = (getHeight() * getHeight()) / item->getHeight();
    
    if (height < getHeight())
    {
        myScrollBarLine.setVisible(true);
        myUpArrowButton.setVisible(true);
        myDownArrowButton.setVisible(true);
        
        myScrollBarLine.setStartPoint(getWidth() - 1, 0);
        myScrollBarLine.setEndPoint(getWidth() - 1, height);
        
        getMyViewAreaItem().setX(myUpArrowButton.getWidth());
        getMyViewAreaItem().setWidth(getWidth() -
                                     myUpArrowButton.getWidth() -
                                     myScrollBarLine.getThickness() - 1);
        
        myIsScrolling = true;
    }
    else
    {
        myScrollBarLine.setVisible(false);
        myUpArrowButton.setVisible(false);
        myDownArrowButton.setVisible(false);
        
        getMyViewAreaItem().setX(0);
        getMyViewAreaItem().setWidth(getWidth() - myScrollBarLine.getThickness() - 1);
        
        myIsScrolling = false;
    }
}

//------------------------------------------------------------------------------
void UiScrollView::driverMoveItemIntoView(UiItem& item)
{
    if (myIsScrolling)
    {
        int itemY = getReferenceItem().getAbsoluteY();
        int itemHeight = getReferenceItem().getHeight();
        
        int y = -(itemY * getHeight()) / itemHeight;
        myScrollBarLine.setY(y);
    }
}
