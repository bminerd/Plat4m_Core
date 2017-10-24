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
/// @file UiPrinterColor.cpp
/// @author Ben Minerd
/// @date 9/20/2013
/// @brief UiPrinterColor class source file.
///

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <UiPrinterColor.h>
#include <UiText.h>
#include <UiLine.h>
#include <UiRectangle.h>
#include <UiImage.h>
#include <UiView.h>

using Plat4m::UiPrinterColor;
using Plat4m::UiPrinter;

/*------------------------------------------------------------------------------
 * Public constructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
UiPrinterColor::UiPrinterColor(Display& display,  DisplaySTLCD& displaySTLCD) :
    UiPrinter(display),
    myDisplaySTLCD(displaySTLCD)
{
}

/*------------------------------------------------------------------------------
 * Public implemented methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
UiPrinter::Error UiPrinterColor::printText(UiText& text)
{
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
UiPrinter::Error UiPrinterColor::printLine(UiLine& line)
{
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
UiPrinter::Error UiPrinterColor::printRectangle(UiRectangle& rectangle)
{
    Display& display = getDisplay();
    const UiItem& viewAreaItem = rectangle.getView()->getViewAreaItem();
    int viewX = viewAreaItem.getAbsoluteX();
    int viewY = viewAreaItem.getAbsoluteY();
    int viewWidth = viewAreaItem.getWidth();
    int viewHeight = viewAreaItem.getHeight();
    
    int x = rectangle.getAbsoluteX();
    int y = rectangle.getAbsoluteY();
    int width = rectangle.getWidth();
    int height = rectangle.getHeight();
    int lineWidth = rectangle.getLineWidth();
    
    int startX = x;
    int endX   = x + width - 1;
    int startY = y;
    int endY   = y + height - 1;

    int displayWidth  = display.getWidth();
    int displayHeight = display.getHeight();

    // Check for left and right clipping

    if (startX < 0)
    {
        x = 0;
    }

    if (startX < viewX)
    {
        startX = viewX;
    }

    if (endX > (displayWidth - 1))
    {
        endX = (displayWidth - 1);
    }

    if (endX > (viewX + viewWidth))
    {
        endX = (viewX + viewWidth);
    }

    // Check for top and bottom clipping
    // TODO: This currently just sets X,Y to min viewable value

    if (startY < 0)
    {
        y = 0;
    }

    if (startY < viewY)
    {
        startY = viewY;
    }

    if (endY > (displayHeight - 1))
    {
        endX = (displayWidth - 1);
    }

    if (endX > (viewX + viewWidth))
    {
        endX = (viewX + viewWidth);
    }

    // Top line

    uint16_t lineColor = rectangle.getLineColor();

    myDisplaySTLCD.writeRegister(0x21, startX);

    for (int i = 0; i < lineWidth; i++)
    {
        myDisplaySTLCD.writeRegister(0x20, startY + i);
        myDisplaySTLCD.setGraphicsMemory();

        for (int j = startX; j <= endX; j++)
        {
            myDisplaySTLCD.writeToGraphicsMemory(lineColor);
        }
    }

    // Bottom line

    myDisplaySTLCD.writeRegister(0x21, startX);

    for (int i = 0; i < lineWidth; i++)
    {
        myDisplaySTLCD.writeRegister(0x20, endY - i);
        myDisplaySTLCD.setGraphicsMemory();

        for (int j = startX; j <= endX; j++)
        {
            myDisplaySTLCD.writeToGraphicsMemory(lineColor);
        }
    }

    // Left line

    for (int i = (startY + 1); i <= (endY - 1); i++)
    {
        myDisplaySTLCD.writeRegister(0x20, i);
        myDisplaySTLCD.writeRegister(0x21, startX);

        myDisplaySTLCD.setGraphicsMemory();

        for (int j = 0; j < lineWidth; j++)
        {
            myDisplaySTLCD.writeToGraphicsMemory(lineColor);
        }
    }

    // Right line

    for (int i = (startY + 1); i <= (endY - 1); i++)
    {
        myDisplaySTLCD.writeRegister(0x20, i);
        myDisplaySTLCD.writeRegister(0x21, endX - lineWidth + 1);

        myDisplaySTLCD.setGraphicsMemory();

        for (int j = 0; j < lineWidth; j++)
        {
            myDisplaySTLCD.writeToGraphicsMemory(lineColor);
        }
    }
    
    // Print fill

    if (rectangle.getFill() == UiRectangle::FILL_SOLID)
    {
        uint16_t fillColor = rectangle.getFillColor();

        startY += lineWidth;
        startX += lineWidth;
        endX   -= lineWidth;
        endY   -= lineWidth;

//        myDisplaySTLCD.setGraphicsMemoryWindow(startX, startY, endX, endY);
//        myDisplaySTLCD.setGraphicsMemory();

        for (int i = startY; i <= endY; i++)
        {
            myDisplaySTLCD.writeRegister(32, i);
            myDisplaySTLCD.writeRegister(33, startX);
            myDisplaySTLCD.setGraphicsMemory();

            for (int j = startX; j <= endX; j++)
            {
                myDisplaySTLCD.writeToGraphicsMemory(fillColor);
            }
        }

//        myDisplaySTLCD.setGraphicsMemoryWindow(0, 0, 319, 239);
    }

    return ERROR_NONE;
}

//------------------------------------------------------------------------------
UiPrinter::Error UiPrinterColor::printImage(UiImage& image)
{
    return ERROR_NONE;
}
