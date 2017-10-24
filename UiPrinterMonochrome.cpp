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
/// @file UiPrinterMonochrome.cpp
/// @author Ben Minerd
/// @date 9/20/2013
/// @brief UiPrinterMonochrome class source file.
///

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <UiPrinterMonochrome.h>
#include <UiText.h>
#include <UiLine.h>
#include <UiRectangle.h>
#include <UiImage.h>
#include <UiView.h>

using Plat4m::UiPrinterMonochrome;
using Plat4m::UiPrinter;

/*------------------------------------------------------------------------------
 * Public constructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
UiPrinterMonochrome::UiPrinterMonochrome(Display& display) :
    UiPrinter(display)
{
}

/*------------------------------------------------------------------------------
 * Public implemented methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
UiPrinter::Error UiPrinterMonochrome::printText(UiText& text)
{
    Display::Frame& displayFrame = getFrame();
    const UiItem& viewAreaItem = text.getView()->getViewAreaItem();
    int viewX = viewAreaItem.getAbsoluteX();
    int viewY = viewAreaItem.getAbsoluteY();
    int viewWidth = viewAreaItem.getWidth();
    int viewHeight = viewAreaItem.getHeight();
    
    // Clear data around item?
    
    const tFont& font = text.getFont();
    int x = text.getAbsoluteX();
    int y = text.getAbsoluteY();
    int xDataIndex = x;
    int yDataIndex = (y / 8);
    int yOffset = y % 8;
    
    // Text clipped by top of view?
    
    if (y < 0)
    {
        yDataIndex--;
        yOffset += 8;
    }
    
    int curXDataIndex = xDataIndex;
    int curYDataIndex = yDataIndex;
    const char* string = text.getText();
    
    while ((*string) != '\0')
    {
        if ((*string) == '\n')
        {
            curXDataIndex = xDataIndex;
            yDataIndex++;
            string++;
        }
        
        unsigned int charIndex = (*string) - 0x20;
        unsigned int charWidth = font.chars[charIndex].image->width;
        unsigned int charDataWidth = charWidth;
        unsigned int charHeight = font.chars[charIndex].image->height;
        unsigned int charDataHeight = ((charHeight - 1) / 8) + 1;
        
        for (int i = 0; i < charDataWidth; i++)
        {
            // Check to see if left or right edge is clipped
            // TODO: Make function to check these boundaries
            if ((curXDataIndex >= viewX) &&
                (curXDataIndex < (viewX + viewWidth)))
            {
                for (int j = 0; j < charDataHeight; j++)
                {
                    curYDataIndex = yDataIndex + j;
                    
                    uint8_t data =
                            font.chars[charIndex].
                                image->data[((i * charDataHeight) + j)];
                    
                    // Check to see if top or bottom edge is clipped
                    if ((curYDataIndex >= 0) &&
                        (curYDataIndex < (displayFrame.height / 8)))
                    {
                        
                        writeToDisplayFrame(displayFrame,
                                            (curXDataIndex * (displayFrame.height / 8)) + curYDataIndex,
                                            data << yOffset);
                    }
                    
                    if ((yOffset > 0)               &&
                        ((curYDataIndex + 1) >= 0)  &&
                        ((curYDataIndex + 1) < (displayFrame.height / 8)))
                    {   
                        writeToDisplayFrame(displayFrame,
                                            (curXDataIndex * (displayFrame.height / 8)) + curYDataIndex + 1,
                                            data >> (8 - yOffset));
                    }
                }
            }
            
            curXDataIndex++;
        }
        
        string++;
    }
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
UiPrinter::Error UiPrinterMonochrome::printLine(UiLine& line)
{
    Display::Frame& displayFrame = getFrame();
    const UiItem& viewAreaItem = line.getView()->getViewAreaItem();
    int viewX = viewAreaItem.getAbsoluteX();
    int viewY = viewAreaItem.getAbsoluteY();
    int viewWidth = viewAreaItem.getWidth();
    int viewHeight = viewAreaItem.getHeight();
    
    int x = line.getAbsoluteX();
    int y = line.getAbsoluteY();
    int xDataIndex = x;
    int yDataIndex = (y / 8);
    int yTopOffset = y % 8;
    int height = line.getHeight();
    int dataWidth = line.getWidth();
    int dataHeight = ((yTopOffset + height - 1) / 8) + 1;
    int yBottomOffset = ((yDataIndex + dataHeight) * 8) - (y + height);
    int thickness = line.getThickness();
    
    uint8_t topCornerMask = (~(255 << height)) << yTopOffset;
    uint8_t bottomCornerMask = ~(255 >> height);
    bottomCornerMask = bottomCornerMask >> yBottomOffset;
    
    uint8_t lineMask = (~(255 << thickness)) << yTopOffset;
    
    if ((xDataIndex >= 0) && (xDataIndex < displayFrame.width))
    {
        // Vertical
        for (int i = yDataIndex; i < (yDataIndex + dataHeight); i++)
        {
            for (int j = 0; j < thickness; j++)
            {
                uint8_t mask;
                
                if (i == yDataIndex)
                {
                    mask = topCornerMask;
                }
                else if (i == (yDataIndex + dataHeight - 1))
                {
                    mask = bottomCornerMask;
                }
                else
                {
                    mask = 255;
                }
                
                if ((i >= 0) && (i < (displayFrame.height / 8)))
                {
                    writeToDisplayFrame(displayFrame,
                                        ((xDataIndex + j) * (displayFrame.height / 8)) + i,
                                        mask);
                }
            }
        }
        
        // Horizontal
        for (int i = xDataIndex; i < (xDataIndex + dataWidth - 1); i++)
        {
            if ((i >= 0)                    &&
                (i < displayFrame.width)    &&
                (i >= viewX)                &&
                (i < (viewX + viewWidth)))
            {
                if ((yDataIndex >= 0) && (yDataIndex < (displayFrame.height / 8)))
                {
                    // Top
                    writeToDisplayFrame(displayFrame,
                                        (i * (displayFrame.height / 8)) + yDataIndex,
                                        lineMask);
                }
            }
        }
    }
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
UiPrinter::Error UiPrinterMonochrome::printRectangle(UiRectangle& rectangle)
{
    Display::Frame& displayFrame = getFrame();
    const UiItem& viewAreaItem = rectangle.getView()->getViewAreaItem();
    int viewX = viewAreaItem.getAbsoluteX();
    int viewY = viewAreaItem.getAbsoluteY();
    int viewWidth = viewAreaItem.getWidth();
    int viewHeight = viewAreaItem.getHeight();
    
    int x = rectangle.getAbsoluteX();
    int y = rectangle.getAbsoluteY();
    int xDataIndex = x;
    int yDataIndex = (y / 8);
    int yTopOffset = y % 8;
    int width = rectangle.getWidth();
    int height = rectangle.getHeight();
    int dataWidth = width;
    int dataHeight = ((yTopOffset + height - 1) / 8) + 1;
    int yBottomOffset = ((yDataIndex + dataHeight) * 8) - (y + height);
    int thickness = rectangle.getThickness();
    
    uint8_t lineMask = ~(255 << thickness);
    uint8_t topLineMask = lineMask << yTopOffset;
    uint8_t bottomLineMask = lineMask << (8 - thickness - yBottomOffset);
    uint8_t topCornerMask = (~(255 << height)) << yTopOffset;
    uint8_t bottomCornerMask = ~(255 >> height);
    bottomCornerMask = bottomCornerMask >> yBottomOffset;
    
    // Top and bottom
    for (int i = xDataIndex + 1; i < (xDataIndex + dataWidth - 1); i++)
    {
        if ((i >= 0)                    &&
            (i < displayFrame.width)    &&
            (i >= viewX)                &&
            (i < (viewX + viewWidth)))
        {
            if ((yDataIndex >= 0) && (yDataIndex < (displayFrame.height / 8)))
            {
                // Top
                writeToDisplayFrame(displayFrame,
                                    (i * (displayFrame.height / 8)) + yDataIndex,
                                    topLineMask);
            }
            
            if (((yDataIndex + dataHeight - 1) >= 0) &&
                ((yDataIndex + dataHeight - 1) < (displayFrame.height / 8)))
            {
                // Bottom
                writeToDisplayFrame(displayFrame,
                                    (i * (displayFrame.height / 8)) +
                                    yDataIndex + dataHeight - 1,
                                    bottomLineMask);
            }
        }
    }
    
    // Left and right
    for (int i = yDataIndex; i < (yDataIndex + dataHeight); i++)
    {
        if ((i >= 0) && (i < (displayFrame.height / 8)))
        {
            uint8_t mask;
                
            if (i == yDataIndex)
            {
                mask = topCornerMask;
            }
            else if (i == (yDataIndex + dataHeight - 1))
            {
                mask = bottomCornerMask;
            }
            else
            {
                mask = 255;
            }
            
            for (int j = 0; j < thickness; j++)
            {
                if ((xDataIndex >= 0)                   &&
                    (xDataIndex < displayFrame.width)   &&
                    (xDataIndex >= viewX)               &&
                    (xDataIndex < (viewX + viewWidth)))
                {
                    // Left
                    writeToDisplayFrame(displayFrame,
                                        ((xDataIndex + j) *
                                         (displayFrame.height / 8)) + i,
                                        mask);
                }
                
                if (((xDataIndex + dataWidth - j - 1) >= 0)                 &&
                    ((xDataIndex + dataWidth - j - 1) < displayFrame.width) &&
                    ((xDataIndex + dataWidth - j - 1) >= viewX)             &&
                    ((xDataIndex + dataWidth - j - 1) < (viewX + viewWidth)))
                {
                    // Right
                    writeToDisplayFrame(displayFrame,
                                        ((xDataIndex + dataWidth - j - 1) * (displayFrame.height / 8)) + i,
                                        mask);
                }
            }
        }
    }
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
UiPrinter::Error UiPrinterMonochrome::printImage(UiImage& image)
{
    Display::Frame& displayFrame = getFrame();
    
    int x = image.getAbsoluteX();
    int y = image.getAbsoluteY();
    int xDataIndex = x;
    int yDataIndex = (y / 8);
    int yOffset = y % 8;
    int height = image.getHeight();
    int dataWidth = image.getWidth();
    int dataHeight = ((height - 1) / 8) + 1;
    
    for (int i = 0; i < dataWidth; i++)
    {
        int curXDataIndex = xDataIndex + i;
        
        // Check to see if left or right edge is clipped
        if ((curXDataIndex >= 0) && (curXDataIndex < displayFrame.width))
        {
            for (int j = 0; j < dataHeight; j++)
            {
                int curYDataIndex = yDataIndex + j;
                uint8_t data = image.getImage().data[((i * dataHeight) + j)];
                
                // Check to see if top or bottom edge is clipped
                if ((curYDataIndex >= 0) &&
                    (curYDataIndex < (displayFrame.height / 8)))
                {
                    
                    writeToDisplayFrame(displayFrame,
                                        (curXDataIndex *
                                         (displayFrame.height / 8)) +
                                        curYDataIndex,
                                        data << yOffset);
                }
                
                if ((yOffset > 0)   &&
                    ((curYDataIndex + 1) >= 0)  &&
                    ((curYDataIndex + 1) < (displayFrame.height / 8)))
                {   
                    writeToDisplayFrame(displayFrame,
                                        (curXDataIndex *
                                         (displayFrame.height / 8)) +
                                        curYDataIndex + 1,
                                        data >> (8 - yOffset));
                }
            }
        }
    }
    
    return ERROR_NONE;
}
        
/*------------------------------------------------------------------------------
 * Private methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void UiPrinterMonochrome::writeToDisplayFrame(Display::Frame& displayFrame,
                                              const unsigned int index,
                                              const uint8_t byte)
{
    if (index < displayFrame.size)
    {
        displayFrame.data[index] |= byte;
    }
    else
    {
        int a = 0;
    }
}
