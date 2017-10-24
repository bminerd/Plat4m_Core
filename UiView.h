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
/// @file UiView.h
/// @author Ben Minerd
/// @date 11/18/2013
/// @brief UiView class header file.
///

#ifndef UI_VIEW_H
#define UI_VIEW_H

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <UiItem.h>
#include <Ui.h>
#include <UiPrinter.h>
#include <UiLayout.h>

namespace Plat4m
{

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

class UiView : public UiItem
{
public:
    
    /*--------------------------------------------------------------------------
     * Public constructors and destructors
     *------------------------------------------------------------------------*/
    
    UiView(Ui& ui,
           UiPrinter& printer,
           UiView* view,
           UiItem* parent,
           UiItem& referenceItem,
           const int width,
           const int height);
    
    /*--------------------------------------------------------------------------
     * Public virtual methods
     *------------------------------------------------------------------------*/
    
    virtual bool isItemInView(UiItem& item);
    
    /*--------------------------------------------------------------------------
     * Public methods
     *------------------------------------------------------------------------*/

    const UiItem& getViewAreaItem();
    
    void setReferenceItem(UiItem& item);
    
    UiItem& getReferenceItem();
    
    UiItem* getFocusItem();
    
    void setFocusItem(UiItem* item); // Ui could send actions directly here?
    
    void moveItemIntoView(UiItem& item);
    
protected:
    
    /*--------------------------------------------------------------------------
     * Protected methods
     *------------------------------------------------------------------------*/
    
    UiItem& getMyViewAreaItem();
    
    Ui& getMyUi();
    
private:
    
    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/
    
    UiItem myViewAreaItem;
    Ui& myUi;
    UiItem* myReferenceItem;
    UiItem* myFocusItem;
    
    /*--------------------------------------------------------------------------
     * Private virtual methods
     *------------------------------------------------------------------------*/
    
    virtual void driverSetFocusItem(UiItem* item);
    
    virtual void driverMoveItemIntoView(UiItem& item);
};

}; // namespace Plat4m

#endif // UI_VIEW_H
