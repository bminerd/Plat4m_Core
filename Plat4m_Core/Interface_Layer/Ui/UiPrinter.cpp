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
 * @file UiPrinter.cpp
 * @author Ben Minerd
 * @date 9/20/2013
 * @brief UiPrinter class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <UiItem.h>
#include <UiPrinter.h>
#include <UiText.h>
#include <UiRectangle.h>
#include <UiImage.h>

/*------------------------------------------------------------------------------
 * Public constructors and destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
UiPrinter::UiPrinter(Display& display) :
    myDisplay(display),
    myPrintItem(NULL_POINTER)
{
}

/*------------------------------------------------------------------------------
 * Public methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void UiPrinter::printFrame()
{
    if (IS_VALID_POINTER(myPrintItem))
    {
        Display::Frame& frame = myDisplay.getFrame();
        memset(frame.data, 0, frame.width * (frame.height / 8));
        
        myPrintItem->print();
        myDisplay.writeFrame();
        
        myPrintItem = NULL_POINTER;
    }
}

//------------------------------------------------------------------------------
unsigned int UiPrinter::getFrameWidth()
{
    return myDisplay.getFrame().width;
}

//------------------------------------------------------------------------------
unsigned int UiPrinter::getFrameHeight()
{
    return myDisplay.getFrame().height;
}

//------------------------------------------------------------------------------
void UiPrinter::schedulePrint(UiItem* item)
{
    // FOR NOW!
    if (IS_NULL_POINTER(myPrintItem))
    {
        myPrintItem = item;
    }
}

/*------------------------------------------------------------------------------
 * Public virtual methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
UiPrinter::Error UiPrinter::printLine(UiLine& line)
{
    // If reached here, subclass didn't implement this method!
    return ERROR_ITEM_INVALID;
}

//------------------------------------------------------------------------------
UiPrinter::Error UiPrinter::printRectangle(UiRectangle& rectangle)
{
    // If reached here, subclass didn't implement this method!
    return ERROR_ITEM_INVALID;
}

//------------------------------------------------------------------------------
UiPrinter::Error UiPrinter::printImage(UiImage& image)
{
    // If reached here, subclass didn't implement this method!
    return ERROR_ITEM_INVALID;
}

/*------------------------------------------------------------------------------
 * Protected methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Display::Frame& UiPrinter::getFrame()
{
    return myDisplay.getFrame();
}