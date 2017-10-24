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
/// @file UiPrinter.h
/// @author Ben Minerd
/// @date 9/20/2013
/// @brief UiPrinter class header file.
///

#ifndef UI_PRINTER_H
#define UI_PRINTER_H

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <Display.h>

/*------------------------------------------------------------------------------
 * Namespace
 *----------------------------------------------------------------------------*/

namespace Plat4m
{

/*------------------------------------------------------------------------------
 * Forward class declarations
 *----------------------------------------------------------------------------*/

class UiItem;
class UiText;
class UiRectangle;
class UiLine;
class UiImage;

/*------------------------------------------------------------------------------
 * Class
 *----------------------------------------------------------------------------*/

class UiPrinter
{
public:
    
    /*--------------------------------------------------------------------------
     * Public enumerations
     *------------------------------------------------------------------------*/
    
    enum Error
    {
        ERROR_NONE,
        ERROR_ITEM_INVALID
    };
    
    /*--------------------------------------------------------------------------
     * Public methods
     *------------------------------------------------------------------------*/
    
    void printFrame();
    
    unsigned int getFrameWidth();
    
    unsigned int getFrameHeight();
    
    void schedulePrint(UiItem* item);
    
    /*--------------------------------------------------------------------------
     * Public virtual methods
     *------------------------------------------------------------------------*/
    
    virtual Error printText(UiText& text) = 0;
    
    virtual Error printLine(UiLine& line);

    virtual Error printRectangle(UiRectangle& rectangle);
    
    virtual Error printImage(UiImage& image);
    
protected:
    
    /*--------------------------------------------------------------------------
     * Protected constructors
     *------------------------------------------------------------------------*/

    UiPrinter(Display& display);

    /*--------------------------------------------------------------------------
     * Public virtual destructors
     *------------------------------------------------------------------------*/

    virtual ~UiPrinter();

    /*--------------------------------------------------------------------------
     * Protected methods
     *------------------------------------------------------------------------*/
    
    Display& getDisplay();

    Display::Frame& getFrame();
    
private:
    
    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/
    
    Display& myDisplay;
    
    UiItem* myPrintItem;
};

}; // namespace Plat4m

#endif // UI_PRINTER_H
