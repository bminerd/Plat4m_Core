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
 * @file Menu.cpp
 * @author Ben Minerd
 * @date 9/4/2013
 * @brief Menu class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Menu.h>
#include <UiLink.h>
#include <UiCallback.h>
#include <UiView.h>

/*------------------------------------------------------------------------------
 * Public constructors and destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Menu::Menu(UiPrinter& printer,
           UiView* view,
           UiItem* parent,
           const char* name,
           const tFont& font,
           ExitCallback& exitCallback,
           const int width) :
    UiList(printer, view, parent, width),
    myName(name),
    myFont(font),
    myParentMenu(NULL_POINTER),
    myExitCallback(&exitCallback)
{
}

//------------------------------------------------------------------------------
Menu::Menu(UiPrinter& printer,
           UiView* view,
           UiItem* parent,
           const char* name,
           const tFont& font,
           Menu& parentMenu,
           const int width) :
    UiList(printer, view, parent, width),
    myName(name),
    myFont(font),
    myParentMenu(&parentMenu),
    myExitCallback(NULL_POINTER)
{
}

/*------------------------------------------------------------------------------
 * Public methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Menu& Menu::addSubMenu(const char* name)
{
    Menu* menu = new Menu(getPrinter(),
                          getView(),
                          this,
                          name,
                          myFont,
                          *this,
                          getWidth());
    UiText* text = new UiText(getPrinter(), getView(), this, myFont, name);
    UiCallback* callback = new UiCallback(getPrinter(),
                                          getView(),
                                          this,
                                          *text,
                                          Plat4m::createCallback(menu, &Menu::enter));
    UiList::addItem(callback);
    
    menu->setY(callback->getY());
    menu->setVisible(false);
    
    mySubMenuList.append(menu);
    
    List<Menu*>::Iterator iterator = mySubMenuList.iterator();
    
    while (iterator.hasCurrent())
    {
        Menu* menu = iterator.current();
        menu->setX(getWidth() + 4);
        
        iterator.next();
    }
    
    return *menu;
}

//------------------------------------------------------------------------------
UiSelectableItem& Menu::addItem(const char* name, UiItem& linkItem)
{
    UiText* text = new UiText(getPrinter(), getView(), this, myFont, name);
    UiLink* link = new UiLink(getPrinter(),
                              getView(),
                              this,
                              *text,
                              linkItem);
    
    addChild(&linkItem);
    linkItem.setX(getWidth() + 4);
    linkItem.setVisible(false);
    
    UiList::addItem(link);
    
    return *link;
}

//------------------------------------------------------------------------------
void Menu::addExitItem(const char* name)
{
    UiText* text = new UiText(getPrinter(), getView(), this, myFont, name);
    UiCallback* callback = new UiCallback(
                                     getPrinter(),
                                     getView(),
                                     this,
                                     *text,
                                     Plat4m::createCallback(this, &Menu::exit));
    
    UiList::addItem(callback);
}

//------------------------------------------------------------------------------
void Menu::enter()
{
    Menu* parent = myParentMenu;
    
    if (IS_VALID_POINTER(parent))
    {
        while (IS_VALID_POINTER(parent->myParentMenu))
        {
            parent = parent->myParentMenu;
        }
        
        getView()->setReferenceItem(*parent);
    }
    
    setFocus();
    moveIntoView();
    getView()->setReferenceItem(*this);
    moveCurrentItemIntoView();
}

//------------------------------------------------------------------------------
void Menu::exit()
{
    reset();
    moveCurrentItemIntoView();
    setVisible(false);
    
    if (IS_VALID_POINTER(myParentMenu))
    {
        Menu* parent = myParentMenu;

        while (IS_VALID_POINTER(parent->myParentMenu))
        {
            parent = parent->myParentMenu;
        }
        
        getView()->setReferenceItem(*parent);
        
        myParentMenu->setFocus();
        myParentMenu->moveCurrentItemIntoView();
        
        getView()->setReferenceItem(*myParentMenu);
        myParentMenu->moveCurrentItemIntoView();
    }
    else if (IS_VALID_POINTER(myExitCallback))
    {
        myExitCallback->call();
    }
}