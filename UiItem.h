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
/// @file UiItem.h
/// @author Ben Minerd
/// @date 7/10/2013
/// @brief UiItem class header file.
///

#ifndef UI_ITEM_H
#define UI_ITEM_H

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <UiPrinter.h>
#include <List.h>
#include <UiInput.h>

/*------------------------------------------------------------------------------
 * Namespace
 *----------------------------------------------------------------------------*/

namespace Plat4m
{

/*------------------------------------------------------------------------------
 * Forward class declarations
 *----------------------------------------------------------------------------*/

class UiView;

/*------------------------------------------------------------------------------
 * Class
 *----------------------------------------------------------------------------*/

class UiItem
{
public:
    
    /*--------------------------------------------------------------------------
     * Public enumerations
     *------------------------------------------------------------------------*/
    
    enum HorizontalAlignment
    {
        HORIZONTAL_ALIGNMENT_ABSOLUTE,
        HORIZONTAL_ALIGNMENT_CENTER,
        HORIZONTAL_ALIGNMENT_LEFT,
        HORIZONTAL_ALIGNMENT_RIGHT
    };
    
    enum VerticalAlignment
    {
        VERTICAL_ALIGNMENT_ABSOLUTE,
        VERTICAL_ALIGNMENT_CENTER,
        VERTICAL_ALIGNMENT_TOP,
        VERTICAL_ALIGNMENT_BOTTOM
    };
    
    /*--------------------------------------------------------------------------
     * Public constructors
     *------------------------------------------------------------------------*/

    UiItem(UiPrinter& printer, UiView* view, UiItem* parent);

    /*--------------------------------------------------------------------------
     * Public virtual destructors
     *------------------------------------------------------------------------*/
    
    virtual ~UiItem();
    
    /*--------------------------------------------------------------------------
     * Public methods
     *------------------------------------------------------------------------*/
    
    UiItem* getParent();
    
    void setParent(UiItem* parent);
    
    UiView* getView();

    void addChild(UiItem* item);
    
    void print();
    
    void update();
    
    void updateChild(UiItem& item);
    
    int getX() const;
         
    void setX(const int x);
    
    void shiftX(const int dX);
    
    int getY() const;
    
    void setY(const int y);
    
    void shiftY(const int dY);
    
    void setXY(const int x, const int y);
    
    void shiftXY(const int dX, const int dY);
    
    int getAbsoluteX() const;
    
    int getAbsoluteY() const;
    
    int getWidth() const;
    
    void setWidth(const int width, const bool update = true);
    
    int getHeight() const;
    
    void setHeight(const int height, const bool update = true);
    
    void setWidthHeight(const int width,
                        const int height,
                        const bool update = true);
    
    HorizontalAlignment getHorizontalAlignment() const;
    
    void setHorizontalAlignment(const HorizontalAlignment horizontalAlignment);
    
    VerticalAlignment getVerticalAlignment() const;
    
    void setVerticalAlignment(const VerticalAlignment verticalAlignment);
    
    int getHorizontalAlignmentOffset() const;
    
    void setHorizontalAlignmentOffset(const int horizontalAlignmentOffset);
    
    int getVerticalAlignmentOffset() const;
    
    void setVerticalAlignmentOffset(const int verticalAlignmentOffset);
    
    void setVisible(const bool visible);
    
    bool isVisible() const;
    
    bool hasChanged() const;
    
    bool handleAction(const UiInput::Action action);
    
    void setFocus();
    
    bool hasFocus();
    
    void moveIntoView();
    
    void setNotifyParentOnUpdate(const bool notify);

protected:
    
    /*--------------------------------------------------------------------------
     * Protected methods
     *------------------------------------------------------------------------*/
    
    UiPrinter& getPrinter();
    
    List<UiItem*>& getChildList();
    
private:
    
    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/
    
    UiPrinter& myPrinter;
    
    UiView* myView;
    
    UiItem* myParent;
    
    List<UiItem*> myChildList;
    
    int myX;
    
    int myY;
    
    int myWidth;
    
    int myHeight;
    
    HorizontalAlignment myHorizontalAlignment;
    
    VerticalAlignment myVerticalAlignment;
    
    int myHorizontalAlignmentOffset;
    
    int myVerticalAlignmentOffset;
    
    bool myIsVisible;
    
    bool myNotifyParentOnUpdate;

    /*--------------------------------------------------------------------------
     * Private virtual methods
     *------------------------------------------------------------------------*/
    
    virtual void driverPrint();
    
    virtual void driverAddChild(UiItem* item);
    
    virtual void driverUpdate();
    
    virtual bool driverHandleAction(const UiInput::Action action);
    
    virtual void driverSetVisible(const bool visible);
    
    // virtual bool driverIsItemInView();
    
    /*--------------------------------------------------------------------------
     * Private methods
     *------------------------------------------------------------------------*/
    
    void setXPrivate(const int x);
    
    void setYPrivate(const int y);
    
    void setWidthPrivate(const int width);
    
    void setHeightPrivate(const  int height);
};

}; // namespace Plat4m

#endif // UI_ITEM_H
