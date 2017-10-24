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
/// @file UiItem.cpp
/// @author Ben Minerd
/// @date 7/10/2013
/// @brief UiItem class source file.
///

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <UiItem.h>

// Match forward class declarations
#include <UiView.h>

using Plat4m::UiItem;
using Plat4m::UiView;
using Plat4m::UiPrinter;

/*------------------------------------------------------------------------------
 * Public constructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
UiItem::UiItem(UiPrinter& printer, UiView* view, UiItem* parent) :
    myPrinter(printer),
    myView(view),
    myParent(parent),
    myX(0),
    myY(0),
    myWidth(0),
    myHeight(0),
    myHorizontalAlignment(HORIZONTAL_ALIGNMENT_ABSOLUTE),
    myVerticalAlignment(VERTICAL_ALIGNMENT_ABSOLUTE),
    myHorizontalAlignmentOffset(0),
    myVerticalAlignmentOffset(0),
    myIsVisible(true),
    myNotifyParentOnUpdate(true)
{
    if (isValidPointer(myParent))
    {
        myParent->addChild(this);
    }
}

/*------------------------------------------------------------------------------
 * Public virtual destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
UiItem::~UiItem()
{
}

/*------------------------------------------------------------------------------
 * Public methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
UiItem* UiItem::getParent()
{
    return myParent;
}

//------------------------------------------------------------------------------
void UiItem::setParent(UiItem* parent)
{
    if (isValidPointer(parent))
    {
        myParent = parent;
    }
}

//------------------------------------------------------------------------------
UiView* UiItem::getView()
{
    return myView;
}

//------------------------------------------------------------------------------
void UiItem::addChild(UiItem* item)
{
    if (isValidPointer(item))
    {
        item->setParent(this);
        myChildList.append(item);
        update();
    }
}

//------------------------------------------------------------------------------
void UiItem::update()
{
    if (isValidPointer(myParent))
    {
        if (myNotifyParentOnUpdate)
        {
            myParent->update();
        }

        myParent->updateChild(*this);
    }
    
    driverUpdate();
    
    // FOR NOW!
    if (isValidPointer(myView))
    {
        myView->update();
    }
    else
    {
        myPrinter.schedulePrint(this);
    }
}

//------------------------------------------------------------------------------
void UiItem::updateChild(UiItem& item)
{
    int x = 0;
    int y = 0;
    
    switch (item.getHorizontalAlignment())
    {
        case UiItem::HORIZONTAL_ALIGNMENT_ABSOLUTE:
        {
            x = item.getX();
            
            break;
        }
        case UiItem::HORIZONTAL_ALIGNMENT_CENTER:
        {
            x = (myWidth / 2) - (item.getWidth() / 2);
            x += item.getHorizontalAlignmentOffset();
            
            break;
        }
        case UiItem::HORIZONTAL_ALIGNMENT_LEFT:
        {
            x = 0;
            x += item.getHorizontalAlignmentOffset();
            
            break;
        }
        case UiItem::HORIZONTAL_ALIGNMENT_RIGHT:
        {
            x = myWidth - item.getWidth();
            x += item.getHorizontalAlignmentOffset();
            
            break;
        }
    }
    
    item.setXPrivate(x);
    
    switch (item.getVerticalAlignment())
    {
        case UiItem::VERTICAL_ALIGNMENT_ABSOLUTE:
        {
            y = item.getY();
            
            break;
        }
        case UiItem::VERTICAL_ALIGNMENT_CENTER:
        {
            y = (myHeight / 2) - (item.getHeight() / 2);
            y += item.getVerticalAlignmentOffset();
            
            break;
        }
        case UiItem::VERTICAL_ALIGNMENT_TOP:
        {
            y = 0;
            y += item.getVerticalAlignmentOffset();
            
            break;
        }
        case UiItem::VERTICAL_ALIGNMENT_BOTTOM:
        {
            y = myHeight - item.getHeight();
            y += item.getVerticalAlignmentOffset();
            
            break;
        }
    }
    
    item.setYPrivate(y);
}

//------------------------------------------------------------------------------
void UiItem::print()
{
    UiItem* parent = getParent();
    
    while (isValidPointer(parent))
    {
        if (!parent->isVisible())
        {
            return;
        }
        
        parent = parent->getParent();
    }
    
    if (isVisible())
    {
        if (isValidPointer(getView()) && (getView()->isItemInView(*this)))
        {
            driverPrint();
        }
        
        List<UiItem*>::Iterator iterator = myChildList.iterator();
        
        while (iterator.hasCurrent())
        {
            UiItem* item = iterator.current();
            item->print();
            
            iterator.next();
        }
    }
}

//------------------------------------------------------------------------------
int UiItem::getX() const
{
    return myX;
}

//------------------------------------------------------------------------------
void UiItem::setX(const int x)
{
    if (x != myX)
    {
        setXPrivate(x);
        update();
    }
}

//------------------------------------------------------------------------------
void UiItem::shiftX(const int dX)
{
    setX(getX() + dX);
}

//------------------------------------------------------------------------------
int UiItem::getY() const
{
    return myY;
}

//------------------------------------------------------------------------------
void UiItem::setY(const int y)
{
    if (y != myY)
    {
        setYPrivate(y);
        update();
    }
}

//------------------------------------------------------------------------------
void UiItem::shiftY(const int dY)
{
    setY(getY() + dY);
}

//------------------------------------------------------------------------------
void UiItem::setXY(const int x, const int y)
{
    if ((x != myX) || (y != myY))
    {
        setXPrivate(x);
        setYPrivate(y);
        update();
    }
}

//------------------------------------------------------------------------------
void UiItem::shiftXY(const int dX, const int dY)
{
    setXY(getX() + dX, getY() + dY);
}

//------------------------------------------------------------------------------
int UiItem::getAbsoluteX() const
{
    int x = myX;
    
    UiItem* item = myParent;
    
    while (isValidPointer(item))
    {
        x += item->getX();
        item = item->getParent();
    }
    
    return x;
}

//------------------------------------------------------------------------------
int UiItem::getAbsoluteY() const
{
    int y = myY;
    
    UiItem* item = myParent;
    
    while (isValidPointer(item))
    {
         y += item->getY();
         item = item->getParent();
    }
    
    return y;
}

//------------------------------------------------------------------------------
int UiItem::getWidth() const
{
    return myWidth;
}

//------------------------------------------------------------------------------
void UiItem::setWidth(const int width, const bool update)
{
    if (width != myWidth)
    {
        setWidthPrivate(width);
        
        if (update)
        {
            this->update();
        }
    }
}

//------------------------------------------------------------------------------
int UiItem::getHeight() const
{
    return myHeight;
}

//------------------------------------------------------------------------------
void UiItem::setHeight(const int height, const bool update)
{
    if (height != myHeight)
    {
        setHeightPrivate(height);
        
        if (update)
        {
            this->update();
        }
    }
}

//------------------------------------------------------------------------------
void UiItem::setWidthHeight(const int width,
                            const int height,
                            const bool update)
{
    if ((width != myWidth) || (height != myHeight))
    {
        setWidthPrivate(width);
        setHeightPrivate(height);
        
        if (update)
        {
            this->update();
        }
    }
}

//------------------------------------------------------------------------------
UiItem::HorizontalAlignment UiItem::getHorizontalAlignment() const
{
    return myHorizontalAlignment;
}

//------------------------------------------------------------------------------
void UiItem::setHorizontalAlignment(
                                  const HorizontalAlignment horizontalAlignment)
{
    if (horizontalAlignment != myHorizontalAlignment)
    {
        myHorizontalAlignment = horizontalAlignment;
        update();
    }
}

//------------------------------------------------------------------------------
UiItem::VerticalAlignment UiItem::getVerticalAlignment() const
{
    return myVerticalAlignment;
}

//------------------------------------------------------------------------------
void UiItem::setVerticalAlignment(const VerticalAlignment verticalAlignment)
{
    if (verticalAlignment != myVerticalAlignment)
    {
        myVerticalAlignment = verticalAlignment;
        update();
    }
}

//------------------------------------------------------------------------------
int UiItem::getHorizontalAlignmentOffset() const
{
    return myHorizontalAlignmentOffset;
}

//------------------------------------------------------------------------------
void UiItem::setHorizontalAlignmentOffset(const int horizontalAlignmentOffset)
{
    if (horizontalAlignmentOffset != myHorizontalAlignmentOffset)
    {
        myHorizontalAlignmentOffset = horizontalAlignmentOffset;
        update();
    }
}

//------------------------------------------------------------------------------
int UiItem::getVerticalAlignmentOffset() const
{
    return myVerticalAlignmentOffset;
}

//------------------------------------------------------------------------------
void UiItem::setVerticalAlignmentOffset(const int verticalAlignmentOffset)
{
    if (verticalAlignmentOffset != myVerticalAlignmentOffset)
    {
        myVerticalAlignmentOffset = verticalAlignmentOffset;
        update();
    }
}

//------------------------------------------------------------------------------
void UiItem::setVisible(const bool visible)
{
    if (visible != myIsVisible)
    {
        myIsVisible = visible;
        update();
    }
}

//------------------------------------------------------------------------------
bool UiItem::isVisible() const
{
    return myIsVisible;
}

//------------------------------------------------------------------------------
bool UiItem::handleAction(const UiInput::Action action)
{
    if (driverHandleAction(action))
    {
        if (isValidPointer(myView))
        {
            myView->handleAction(action);
        }
        
        return true;
    }
    
    UiItem* parent = getParent();
    
    while (isValidPointer(parent))
    {
        if (parent->driverHandleAction(action))
        {
            if (isValidPointer(myView))
            {
                myView->handleAction(action);
            }
            
            return true;
        }
        
        parent = parent->getParent();
    }
    
    return false;
}

//------------------------------------------------------------------------------
void UiItem::setFocus()
{
    if (isValidPointer(myView))
    {
        myView->setFocusItem(this);
    }
}

//------------------------------------------------------------------------------
bool UiItem::hasFocus()
{
    return (myView->getFocusItem() == this);
}

//------------------------------------------------------------------------------
void UiItem::moveIntoView()
{
    myView->moveItemIntoView(*this);
}

//------------------------------------------------------------------------------
void UiItem::setNotifyParentOnUpdate(const bool notify)
{
    myNotifyParentOnUpdate = notify;
}

/*------------------------------------------------------------------------------
 * Protected methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
UiPrinter& UiItem::getPrinter()
{
    return myPrinter;
}

//------------------------------------------------------------------------------
List<UiItem*>& UiItem::getChildList()
{
    return myChildList;
}

/*------------------------------------------------------------------------------
 * Private virtual methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void UiItem::driverPrint()
{
    // Do nothing
}

//------------------------------------------------------------------------------
void UiItem::driverAddChild(UiItem* item)
{
    // Do nothing
}

//------------------------------------------------------------------------------
void UiItem::driverUpdate()
{
    // Do nothing
}

//------------------------------------------------------------------------------
bool UiItem::driverHandleAction(const UiInput::Action action)
{
    // Do nothing
    return false;
}

//------------------------------------------------------------------------------
void UiItem::driverSetVisible(const bool visible)
{
    // Do nothing
}

/*------------------------------------------------------------------------------
 * Private methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void UiItem::setXPrivate(const int x)
{
    myX = x;
}

//------------------------------------------------------------------------------
void UiItem::setYPrivate(const int y)
{
    myY = y;
}

//------------------------------------------------------------------------------
void UiItem::setWidthPrivate(const int width)
{
    myWidth = width;
}

//------------------------------------------------------------------------------
void UiItem::setHeightPrivate(const int height)
{
    myHeight = height;
}
