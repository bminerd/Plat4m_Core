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
 * @file UiScrollView.h
 * @author Ben Minerd
 * @date 12/2/2013
 * @brief UiScrollView class.
 */

#ifndef _UI_SCROLL_VIEW_H_
#define _UI_SCROLL_VIEW_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <UiView.h>
#include <Ui.h>
#include <UiPrinter.h>
#include <UiLine.h>
#include <UiIconButton.h>

/*------------------------------------------------------------------------------
 * Namespace
 *----------------------------------------------------------------------------*/

namespace Plat4m
{

/*------------------------------------------------------------------------------
 * Class
 *----------------------------------------------------------------------------*/

class UiScrollView : public UiView
{
public:
    
    /*--------------------------------------------------------------------------
     * Public constructors and destructors
     *------------------------------------------------------------------------*/
    
    UiScrollView(Ui& ui,
                 UiPrinter& printer,
                 UiView* view,
                 UiItem* parent,
                 UiItem& rootItem,
                 const int width,
                 const int height);
    
    /*--------------------------------------------------------------------------
     * Public methods
     *------------------------------------------------------------------------*/
    
    bool isItemInView(UiItem& item);
         
private:
    
    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/
    
    bool myIsScrolling;
    
    UiLine myScrollBarLine;
    
    UiIconButton myUpArrowButton;
    
    UiIconButton myDownArrowButton;
    
    /*--------------------------------------------------------------------------
     * Private implemented methods
     *------------------------------------------------------------------------*/
    
    bool driverHandleAction(const UiInput::Action action);
    
    void driverSetFocusItem(UiItem* item);
    
    void driverMoveItemIntoView(UiItem& item);
};

}; // namespace Plat4m

#endif // _UI_SCROLL_VIEW_H_
