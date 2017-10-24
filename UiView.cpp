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
 * Copyright (c) 2016 Benjamin Minerd
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

///
/// @file UiView.cpp
/// @author Ben Minerd
/// @date 11/18/2013
/// @brief UiView class source file.
///

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <UiView.h>

using Plat4m::UiView;
using Plat4m::UiItem;
using Plat4m::Ui;

/*------------------------------------------------------------------------------
 * Public constructors and destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
UiView::UiView(Ui& ui,
               UiPrinter& printer,
               UiView* view,
               UiItem* parent,
               UiItem& rootItem,
               const int width,
               const int height) :
    UiItem(printer, view, parent),
    myViewAreaItem(printer, this, this),
    myUi(ui),
    myReferenceItem(&rootItem),
    myFocusItem(NULL_POINTER)
{
    setWidth(width);
    setHeight(height);
    
    myViewAreaItem.setWidth(width);
    myViewAreaItem.setHeight(height);
    
    addChild(&rootItem);
}

/*------------------------------------------------------------------------------
 * Public virtual methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
bool UiView::isItemInView(UiItem& item)
{
    int x = item.getAbsoluteX();
    int y = item.getAbsoluteY();
    int width = item.getWidth();
    int height = item.getHeight();

    int viewX = myViewAreaItem.getAbsoluteX();
    int viewY = myViewAreaItem.getAbsoluteY();
    int viewWidth = myViewAreaItem.getWidth();
    int viewHeight = myViewAreaItem.getHeight();

    if (((x + width) < viewX) || (x > (viewX + viewWidth)))
    {
        return false;
    }

    if (((y + height) < viewY) || (y > (viewY + viewHeight)))
    {
        return false;
    }

    return true;
}

/*------------------------------------------------------------------------------
 * Public methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
const UiItem& UiView::getViewAreaItem()
{
    return myViewAreaItem;
}

//------------------------------------------------------------------------------
void UiView::setReferenceItem(UiItem& item)
{
    myReferenceItem = &item;
}

//------------------------------------------------------------------------------
UiItem& UiView::getReferenceItem()
{
    return (*myReferenceItem);
}

//------------------------------------------------------------------------------
UiItem* UiView::getFocusItem()
{
    return myUi.getFocusItem();
}

//------------------------------------------------------------------------------
void UiView::setFocusItem(UiItem* item)
{
    item->setVisible(true);
    myUi.setFocusItem(item);
    
    driverSetFocusItem(item);
    
//    moveItemIntoView(*item);
}

//------------------------------------------------------------------------------
void UiView::moveItemIntoView(UiItem& item)
{
    int x = item.getAbsoluteX();
    int width = item.getWidth();
    int viewX = myViewAreaItem.getAbsoluteX();
    int viewWidth = myViewAreaItem.getWidth();
    int dX = viewX - x;
    int dWidth = viewWidth - width;
    
    int referenceX = myReferenceItem->getAbsoluteX();
    int referenceWidth = myReferenceItem->getWidth();
    
    if (dX < 0) // Item is right of left view edge
    {
        if (((width - dX) > viewWidth) ||
            ((referenceX < 0) && (referenceX != x)))
        {
            dX += dWidth;
        }
        else
        {
            dX = 0;
        }
    }
    
    int y = item.getAbsoluteY();
    int height = item.getHeight();
    int viewY = myViewAreaItem.getAbsoluteY();
    int viewHeight = myViewAreaItem.getHeight();
    int dY = viewY - y;
    int dHeight = viewHeight - height;
    
    int referenceY = myReferenceItem->getAbsoluteY();
    int referenceHeight = myReferenceItem->getHeight();
    
    if (dY < 0) // Needs to shift up
    {
        if (((height - dY) > viewHeight) ||
            ((referenceY < 0) && (referenceY != y)))
        {
            dY += dHeight;
        }
        else
        {
            dY = 0;
        }
    }
    
    // Check to see if item actually needs to move!
    
    myReferenceItem->shiftXY(dX, dY);
    
    driverMoveItemIntoView(item);
}

/*------------------------------------------------------------------------------
 * Protected methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
UiItem& UiView::getMyViewAreaItem()
{
    return myViewAreaItem;
}

//------------------------------------------------------------------------------
Ui& UiView::getMyUi()
{
    return myUi;
}

/*------------------------------------------------------------------------------
 * Private virtual methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void UiView::driverSetFocusItem(UiItem* item)
{
    // Not implemented by a subclass, do nothing
}

//------------------------------------------------------------------------------
void UiView::driverMoveItemIntoView(UiItem& item)
{
    // Not implemented by a subclass, do nothing
}
