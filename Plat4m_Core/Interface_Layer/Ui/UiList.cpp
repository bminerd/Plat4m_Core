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
 * @file UiList.cpp
 * @author Ben Minerd
 * @date 11/5/2013
 * @brief UiList class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <UiList.h>
#include <UiView.h>

/*------------------------------------------------------------------------------
 * Public constructors and destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
UiList::UiList(UiPrinter& printer,
               UiView* view,
               UiItem* parent,
               const int width) :
    UiItem(printer, view, parent),
    myIsFixedWidth(width != -1),
    myItemOverlap(0),
    myCurrentItemIndex(0),
    myItemLayout(printer, view, this),
    myBoundaryRectangle(printer, view, this),
    myHighlightRectangle(printer, view, this),
    myItemListIterator(myItemList.iterator())
{
    myItemLayout.setX(myHighlightRectangle.getThickness() + 1);
    
    myHighlightRectangle.setHorizontalAlignment(HORIZONTAL_ALIGNMENT_ABSOLUTE);
    myHighlightRectangle.setVerticalAlignment(VERTICAL_ALIGNMENT_ABSOLUTE);
    
    // For now
    myBoundaryRectangle.setVisible(false);
//    myHighlightRectangle.setVisible(false);
    
    if (myIsFixedWidth)
    {
        myHighlightRectangle.setWidth(width);
        setWidth(width);
    }
}

/*------------------------------------------------------------------------------
 * Public methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void UiList::addItem(UiItem* item)
{
//     item->setHorizontalAlignment(HORIZONTAL_ALIGNMENT_LEFT);
    item->setHorizontalAlignment(HORIZONTAL_ALIGNMENT_CENTER);
     item->setVerticalAlignment(VERTICAL_ALIGNMENT_ABSOLUTE);
     
     unsigned int nItems = myItemList.size();
     
     item->setY(myItemLayout.getHeight() - (myItemOverlap * nItems));
     
     myItemLayout.addChild(item);
     
     myItemList.append(item);
     
     if (nItems == 0)
     {
         // Allow for different sized items? (set height when switching)
         myItemListIterator.first();
         myHighlightRectangle.setHeight(item->getHeight()); // For now
     }
     
     if (!myIsFixedWidth &&
         (item->getWidth() > myHighlightRectangle.getWidth()))
     {
         myHighlightRectangle.setWidth(item->getWidth() +
                                       myHighlightRectangle.getThickness() * 2 +
                                       2);
     }
     
     myBoundaryRectangle.setWidthHeight(myHighlightRectangle.getWidth(),
                                        myItemLayout.getHeight());
     setWidthHeight(myBoundaryRectangle.getWidth(),
                    myBoundaryRectangle.getHeight());
     
     update();
}

//------------------------------------------------------------------------------
List<UiItem*> UiList::getItems() const
{
    return myItemList;
}

//------------------------------------------------------------------------------
int UiList::getItemOverlap() const
{
    return myItemOverlap;
}

//------------------------------------------------------------------------------
void UiList::setItemOverlap(const int overlap)
{
    if (overlap != myItemOverlap)
    {
        myItemOverlap = overlap;
        update();
    }
}

//------------------------------------------------------------------------------
void UiList::reset()
{
    int itemIncrement = (getHeight() / myItemList.size());
    
    myHighlightRectangle.setY(0);
    
    myCurrentItemIndex = 0;
    myItemListIterator.first();
    
    UiItem* item = myItemListIterator.current();
    myHighlightRectangle.setHeight(item->getHeight());
}

//------------------------------------------------------------------------------
bool UiList::nextItem()
{
    if (myCurrentItemIndex == (myItemList.size() - 1))
    {
        return false;
    }
    
    myCurrentItemIndex++;
    myItemListIterator.next();
    
    int itemIncrement = (getHeight() / myItemList.size());
    int y = itemIncrement * myCurrentItemIndex;
    myHighlightRectangle.setY(y);
    
    UiItem* item = myItemListIterator.current();
    myHighlightRectangle.setHeight(item->getHeight());
    
    return true;
}

//------------------------------------------------------------------------------
bool UiList::previousItem()
{
    if (myCurrentItemIndex == 0)
    {
        return false;
    }
    
    myCurrentItemIndex--;
    myItemListIterator.previous();
    
    int itemIncrement = (getHeight() / myItemList.size());
    int y = itemIncrement * myCurrentItemIndex;
    myHighlightRectangle.setY(y);
    
    UiItem* item = myItemListIterator.current();
    myHighlightRectangle.setHeight(item->getHeight());
    
    return true;
}

//------------------------------------------------------------------------------
void UiList::moveCurrentItemIntoView()
{
//    UiItem* item = myItemListIterator.current();
//    item->moveIntoView();
    myHighlightRectangle.moveIntoView();
    update();
}

//------------------------------------------------------------------------------
unsigned int UiList::getCurrentItemIndex()
{
    return myCurrentItemIndex;
}

/*------------------------------------------------------------------------------
 * Private implemented methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
bool UiList::driverHandleAction(const UiInput::Action action)
{
    switch (action)
    {
        case UiInput::ACTION_UP_PRESSED:
        {
            previousItem();
            moveCurrentItemIntoView();
            
            return true;
        }
        case UiInput::ACTION_DOWN_PRESSED:
        {
            nextItem();
            moveCurrentItemIntoView();
            
            return true;
        }
        case UiInput::ACTION_SELECT:
        {
            UiItem* item = myItemListIterator.current();
            
            return item->handleAction(action);
        }
        default:
        {
            break;
        }
    }
    
    return false;
}