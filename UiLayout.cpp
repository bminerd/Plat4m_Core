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
 * @file UiLayout.cpp
 * @author Ben Minerd
 * @date 11/4/2013
 * @brief UiLayout class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <UiLayout.h>

using Plat4m::UiLayout;

/*------------------------------------------------------------------------------
 * Public constructors and destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
UiLayout::UiLayout(UiPrinter& printer, UiView* view, UiItem* parent) :
    UiItem(printer, view, parent)
{
}

/*------------------------------------------------------------------------------
 * Private implemented methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void UiLayout::driverAddChild(UiItem* item)
{
}

//------------------------------------------------------------------------------
void UiLayout::driverUpdate()
{
    List<UiItem*>& childList = getChildList();
    
    if (!(childList.isEmpty()))
    {
        List<UiItem*>::Iterator iterator = childList.iterator();
        
        unsigned int width = 0;
        unsigned int height = 0;
        
        while (iterator.hasCurrent())
        {
            UiItem* item = iterator.current();
            
            int itemX = item->getX();
            int itemY = item->getY();
            
            if (itemX < 0)
            {
                width += itemX * -1;
            }
            
            if ((itemX + item->getWidth()) > width)
            {
                width += (itemX + item->getWidth()) - width;
            }
            
            if (itemY < 0)
            {
                height += itemY * -1;
            }
            
            if ((itemY + item->getHeight()) > height)
            {
                height += (itemY + item->getHeight()) - height;
            }
            
            iterator.next();
        }
        
        setWidthHeight(width, height, false);
    }
}
